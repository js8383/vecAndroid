# Inspired by David Farrow, vecpy, (2015), GitHub repository, https://github.com/undefx/vecpy.git
# Created by Yongshan Ding in May, 2016

# ARM NEON support
from neoncompile.kernel import *
from neoncompile.compiler_constants import *

class Compiler_Arm:

  def compile_kernel(k, options):
    src = Formatter()
    src.section('Target Architecture: %s (%s)'%(options.arch['name'], options.type))
    #Set some basic parameters
    size = options.arch['size']
    #Select an appropriate translator for the target architecture and data type
    if options.arch == Architecture.neon:
      if DataType.is_floating(options.type):
        trans = Compiler_Arm.NEON_Float(src, size)
      else:
        trans = Compiler_Arm.NEON_UInt32(src, size)
    else:
      raise Exception('Architecture not supported (%s)'%(options.arch['name']))
    vecType = trans.type
    #Includes
    src += '//Includes'
    src += '#include <stdint.h>'
    src += '#include <math.h>'
    src += '#include <arm_neon.h>'
    src += '#include "neon_more.h"'
    src += ''
    #Function header
    src += '//Kernel function: %s'%(k.name)
    src += 'void %s_vector(KernelArgs* args) {'%(k.name)
    src += ''
    src.indent()
    #Target-dependent setup
    src += '//Setup'
    trans.setup()
    src += ''
    #Uniforms
    src += '//Uniforms'
    for arg in k.get_arguments(uniform=True):
      trans.set('const %s %s'%(vecType, arg.name), 'args->%s'%(arg.name))
    src += ''
    #Fuses
    src += '//Fuses'
    for arg in k.get_arguments(fuse=True):
      trans.set('%s %s_written'%(vecType, arg.name), '0')
    src += ''
    #Literals
    src += '//Literals'
    for var in k.get_literals():
      if DataType.is_floating(options.type):
        value = str(var.value)
      else:
        value = '0x%08x'%(var.value)
      trans.set('const %s %s'%(vecType, var.name), value)
    src += ''
    #Temporary (stack) variables
    src += '//Stack variables'
    src += '%s %s;'%(vecType, ', '.join(var.name for var in k.get_variables(uniform=False, array=False)))
    vars = ['*%s'%(var.name) for var in k.get_variables(uniform=False, array=True)]
    if len(vars) > 0:
      src += '%s %s;'%(options.type, ', '.join(vars))
    src += ''
    #Begin input loop
    src += '//Loop over input'
    src += 'uint64_t index;'
    src += 'for(index = 0; index < args->N; index += %d) {'%(size)
    src += ''
    #Function body
    src.indent()
    #Inputs
    src += '//Inputs'
    for arg in k.get_arguments(input=True, uniform=False):
      if arg.stride > 1:
        index = 'index * %d'%(arg.stride)
        src += '%s = &args->%s[%s];'%(arg.name, arg.name, index)
      else:
        trans.load(arg.name, '&args->%s[index]'%(arg.name))
    src += ''
    #Core kernel logic
    src += '//Begin kernel logic'
    src += '{'
    src += ''
    Compiler_Arm.compile_block(k.block, src, trans)
    src += ''
    src += '}'
    src += '//End kernel logic'
    src += ''
    #Outputs
    src += '//Outputs'
    for arg in k.get_arguments(output=True, fuse=False):
      trans.store('&args->%s[index]'%(arg.name), arg.name)
    for arg in k.get_arguments(output=True, fuse=True):
      for lane in range(size):
        src += 'if(%s(%s_written, %d)) args->%s[0] = %s(%s, %d);'%(trans.extract, arg.name, lane, arg.name, trans.extract, arg.name, lane)
    src += ''
    #End input loop
    src.unindent()
    src += '}'
    #Function footer
    src.unindent()
    src += '}'
    src += '//End of kernel function'
    src += ''
    return src.get_code()

  def compile_block(block, src, trans):
    src.indent()
    for stmt in block.code:
      if isinstance(stmt, Comment):
        src += '//>>> %s'%(stmt.comment)
      elif isinstance(stmt, Assignment):
        if isinstance(stmt.expr, Variable):
          output = stmt.var.name
          input = stmt.expr.name
          if stmt.vector_only:
            mask = stmt.mask.name
            trans.mask(input, output, mask)
            if stmt.var.is_fuse:
              #Set the fuse's written flag
              trans.mask('MASK_TRUE', output + '_written', mask)
          else:
            src += '%s = %s;'%(stmt.var.name, stmt.expr.name)
            if stmt.var.is_fuse:
              raise Exception('Unexpected fuse')
        elif isinstance(stmt.expr, BinaryOperation):
          op = stmt.expr.op
          var = stmt.var.name
          left = stmt.expr.left.name
          right = stmt.expr.right.name
          if op in trans.operations:
            #Check for bit-shift optimization
            if op in (Operator.shift_left, Operator.shift_right) and stmt.expr.right.value is not None:
              #Optimized shift
              right = '%d'%(stmt.expr.right.value)
              trans.operations[op](var, left, right, True)
            else:
              #Normal binary operation
              trans.operations[op](var, left, right)
          else:
            raise Exception('Unknown binary operator/function (%s)'%(op))
        elif isinstance(stmt.expr, UnaryOperation):
          op = stmt.expr.op
          var = stmt.var.name
          input = stmt.expr.var.name
          if op in trans.operations:
            trans.operations[op](var, input)
          else:
            raise Exception('Unknown unary operator/function (%s)'%(op))
        elif isinstance(stmt.expr, ComparisonOperation):
          op = stmt.expr.op
          var = stmt.var.name
          left = stmt.expr.left.name
          right = stmt.expr.right.name
          if op in trans.operations:
            trans.operations[op](var, left, right)
          else:
            raise Exception('Unknown comparison operator (%s)'%(op))
        elif isinstance(stmt.expr, ArrayAccess):
          var = stmt.var.name
          array = stmt.expr.array.name
          index = stmt.expr.index.name
          stride = stmt.expr.array.stride
          if stmt.expr.is_read:
            trans.array_read(var, array, index, stride)
          else:
            trans.array_write(var, array, index, stride)
        else:
          raise Exception('Bad assignment')
      elif isinstance(stmt, IfElse):
        src += '{'
        Compiler_Arm.compile_block(stmt.if_block, src, trans)
        if len(stmt.else_block.code) != 0:
          src += '}'
          src += '//(else)'
          src += '{'
          Compiler_Arm.compile_block(stmt.else_block, src, trans)
        src += '}'
      elif isinstance(stmt, WhileLoop):
        test = '%s(%s)'%(trans.test, stmt.block.mask.name)
        src += 'while(%s) {'%(test)
        Compiler_Arm.compile_block(stmt.block, src, trans)
        src += '}'
      else:
        raise Exception('Can\'t handle that (%s)'%(stmt.__class__))
    src.unindent()

  ################################################################################
  # Translates kernel operations into vectorized C++ code
  ################################################################################
  class Translator:
    def __init__(self, src, size):
      self.src = src
      self.size = size
      self.operations = {
        #Python arithmetic operators
        Operator.add: self.add,
        Operator.subtract: self.sub,
        Operator.multiply: self.mul,
        Operator.divide: self.div,
        Operator.divide_int: self.floordiv,
        Operator.mod: self.mod,
        Operator.pow: self.pow,
        #Python comparison operators
        Operator.eq: self.eq,
        Operator.ne: self.ne,
        Operator.ge: self.ge,
        Operator.gt: self.gt,
        Operator.le: self.le,
        Operator.lt: self.lt,
        #Python bit operators
        Operator.bit_and: self.bit_and,
        Operator.bit_andnot: self.bit_andnot,
        Operator.bit_or: self.bit_or,
        Operator.bit_xor: self.bit_xor,
        Operator.bit_not: self.bit_not,
        Operator.shift_left: self.shift_left,
        Operator.shift_right: self.shift_right,
        #Python boolean operators
        Operator.bool_and: self.bool_and,
        Operator.bool_or: self.bool_or,
        Operator.bool_not: self.bool_not,
        #Python intrinsics
        'abs': self.abs,
        'max': self.max,
        'min': self.min,
        'round': self.round,
        #Math functions (binary)
        'atan2': self.atan2,
        'copysign': self.copysign,
        'fmod': self.fmod,
        'hypot': self.hypot,
        #'ldexp': self.ldexp,
        'pow': self.pow,
        #Math functions (unary)
        'acos': self.acos,
        'acosh': self.acosh,
        'asin': self.asin,
        'asinh': self.asinh,
        'atan': self.atan,
        'atanh': self.atanh,
        'ceil': self.ceil,
        'cos': self.cos,
        'cosh': self.cosh,
        'erf': self.erf,
        'erfc': self.erfc,
        'exp': self.exp,
        'expm1': self.expm1,
        'fabs': self.fabs,
        #'factorial': self.factorial,
        'floor': self.floor,
        #'frexp': self.frexp,
        'gamma': self.gamma,
        #'isfinite': self.isfinite,
        #'isinf': self.isinf,
        #'isnan': self.isnan,
        'lgamma': self.lgamma,
        'log': self.log,
        'log10': self.log10,
        'log1p': self.log1p,
        'log2': self.log2,
        #'modf': self.modf,
        'sin': self.sin,
        'sinh': self.sinh,
        'sqrt': self.sqrt,
        'tan': self.tan,
        'tanh': self.tanh,
        'trunc': self.trunc,
      }
    #Common operations
    def vector_0_1(self, func, args):
      self.src += '%s(%s);'%(func, args[0])
    def vector_1_1(self, func, args):
      self.src += '%s = %s(%s);'%(args[0], func, args[1])
    def vector_0_2(self, func, args):
      self.src += '%s(%s, %s);'%(func, args[0], args[1])
    def vector_1_2(self, func, args):
      self.src += '%s = %s(%s, %s);'%(args[0], func, args[1], args[2])
    def vector_1_3(self, func, args):
      self.src += '%s = %s(%s, %s, %s);'%(args[0], func, args[1], args[2], args[3])
    def scalar_1_1(self, func, args):
      output, input = args
      for i in range(self.size):
        self.src += '%s[%d] = %s(%s[%d]);'%(output, i, func, input, i)
    def scalar_1_2(self, func, args):
      output, left, right = args
      for i in range(self.size):
        self.src += '%s[%d] = %s(%s[%d], %s[%d]);'%(output, i, func, left, i, right, i)
    def scalar_1_2_op(self, op, args):
      output, left, right = args
      for i in range(self.size):
        #self.src += '%s = %s(%s, %s(%s, %d) %s %s(%s, %d), %d);'%(output, self.insert, output, self.extract, left, i, op, self.extract, right, i, i)
        self.src += '%s = %s(%s(%s, %d) %s %s(%s, %d), %s, %d);'%(output, self.insert, self.extract, left, i, op, self.extract, right, i, output, i)
    def mask_1_2(self, input, output, mask, or_, and_, andnot_):
      if mask == 'MASK_TRUE':
        self.src += '%s = %s;'%(output, input)
      else:
        self.src += '%s = %s(%s(%s, %s), %s(%s, %s));'%(output, or_, and_, mask, input, andnot_, mask, output)
    def error(self):
      raise Exception('Not implemented')
    #Abstract stubs
    #Misc
    def setup(self):
      self.error()
    def set(self, *args):
      self.error()
    def load(self, *args):
      self.error()
    def store(self, *args):
      self.error()
    def mask(self, *args):
      self.error()
    #Python arithmetic operators
    def add(self, *args):
      self.error()
    def sub(self, *args):
      self.error()
    def mul(self, *args):
      self.error()
    def div(self, *args):
      self.error()
    def floordiv(self, *args):
      self.error()
    def mod(self, *args):
      self.error()
    def pow(self, *args):
      self.error()
    #Python comparison operators
    def eq(self, *args):
      self.error()
    def ne(self, *args):
      self.error()
    def ge(self, *args):
      self.error()
    def gt(self, *args):
      self.error()
    def le(self, *args):
      self.error()
    def lt(self, *args):
      self.error()
    #Python bit operators
    def bit_and(self, *args):
      self.error()
    def bit_andnot(self, *args):
      self.error()
    def bit_or(self, *args):
      self.error()
    def bit_xor(self, *args):
      self.error()
    def bit_not(self, *args):
      self.error()
    def shift_left(self, *args):
      self.error()
    def shift_right(self, *args):
      self.error()
    #Python boolean operators
    def bool_and(self, *args):
      self.error()
    def bool_or(self, *args):
      self.error()
    def bool_not(self, *args):
      self.error()
    #Python intrinsics
    def abs(self, *args):
      self.error()
    def max(self, *args):
      self.error()
    def min(self, *args):
      self.error()
    def round(self, *args):
      self.error()
    #Math functions (binary)
    def atan2(self, *args):
      self.error()
    def copysign(self, *args):
      self.error()
    def fmod(self, *args):
      self.error()
    def hypot(self, *args):
      self.error()
    def ldexp(self, *args):
      self.error()
    #Math functions (unary)
    def acos(self, *args):
      self.error()
    def acosh(self, *args):
      self.error()
    def asin(self, *args):
      self.error()
    def asinh(self, *args):
      self.error()
    def atan(self, *args):
      self.error()
    def atanh(self, *args):
      self.error()
    def ceil(self, *args):
      self.error()
    def cos(self, *args):
      self.error()
    def cosh(self, *args):
      self.error()
    def erf(self, *args):
      self.error()
    def erfc(self, *args):
      self.error()
    def exp(self, *args):
      self.error()
    def expm1(self, *args):
      self.error()
    def fabs(self, *args):
      self.error()
    def factorial(self, *args):
      self.error()
    def floor(self, *args):
      self.error()
    def frexp(self, *args):
      self.error()
    def gamma(self, *args):
      self.error()
    def isfinite(self, *args):
      self.error()
    def isinf(self, *args):
      self.error()
    def isnan(self, *args):
      self.error()
    def lgamma(self, *args):
      self.error()
    def log(self, *args):
      self.error()
    def log10(self, *args):
      self.error()
    def log1p(self, *args):
      self.error()
    def log2(self, *args):
      self.error()
    def modf(self, *args):
      self.error()
    def sin(self, *args):
      self.error()
    def sinh(self, *args):
      self.error()
    def sqrt(self, *args):
      self.error()
    def tan(self, *args):
      self.error()
    def tanh(self, *args):
      self.error()
    def trunc(self, *args):
      self.error()

  ################################################################################
  # Translates kernel operations into vectorized C++ code (NEON, 32-bit float)
  ################################################################################
  class NEON_Float(Translator):
    def __init__(self, src, size):
      Compiler_Arm.Translator.__init__(self, src, size)
      self.type = 'float32x4_t'
      self.test = 'vmovemask_f32' #TODO: double check
      self.insert = 'vsetq_lane_f32' #TODO: double check args order changed '_mm_insert_epi32'
      self.extract = 'vgetq_lane_f32'#TODO: same order!! '_mm_extract_epi32'
    #Misc
    def setup(self):
      self.src += 'const %s MASK_FALSE = vdupq_n_f32(0.f);'%(self.type)
      self.src += 'const %s MASK_TRUE = vcvtq_f32_u32(vceqq_f32(MASK_FALSE, MASK_FALSE));'%(self.type) #TODO: output type changed to float.
      #for i in range(self.size):
      #  slots = []
      #  for j in range(self.size):
      #    slots.append('%ff'%(1 if i == j else 0))
      #  slots.reverse()
      #  lane = '_mm_cmpneq_ps(MASK_FALSE, _mm_set_ps(%s))'%(', '.join(slots))
      #  self.src += 'const %s MASK_LANE_%d = %s;'%(self.type, i, lane)
    def set(self, *args):
      self.vector_1_1('vdupq_n_f32', args)
    def load(self, *args):
      self.vector_1_1('vld1q_f32', args)
    def store(self, *args):
      self.vector_0_2('vst1q_f32', args)
    def mask(self, *args):
      #(input, output, mask) = args
      #self.mask_1_2(input, output, mask, '_mm_or_ps', '_mm_and_ps', '_mm_andnot_ps') #TODO
      args = (args[1], args[2], args[0], args[1])
      self.vector_1_3('vbslq_f32', args)
    #Python arithmetic operators
    def add(self, *args):
      self.vector_1_2('vaddq_f32', args)
    def sub(self, *args):
      self.vector_1_2('vsubq_f32', args)
    def mul(self, *args):
      self.vector_1_2('vmulq_f32', args)
    def div(self, *args):
      # self.vector_1_2('_mm_div_ps', args) #TODO: reciprocal step? vrecpsq_f32
      self.scalar_1_2_op('/', args)
    def floordiv(self, *args):
      self.div(*args)
      self.floor(args[0], args[0]) #TODO
    def mod(self, *args):
      self.scalar_1_2('fmod', args) #TODO
    def pow(self, *args):
      self.scalar_1_2('pow', args) #TODO
    #Python comparison operators
    def eq(self, *args):
      #self.vector_1_2('vceqq_f32', args)
      self.src += '%s = vcvtq_f32_u32(vceqq_f32(%s, %s));'%(args[0], args[1], args[2])
      # convert doesn't change the bit pattern
    def ne(self, *args):
      #self.vector_1_2('_mm_cmpneq_ps', args) #TODO
      self.bit_not(args[0], 'vcvtq_f32_u32(vceqq_f32(%s, %s))'%(args[1], args[2]))
      #self.src += '%s = vcvtq_f32_u32(vceqq_f32(%s, %s));'%(args[0], args[1], args[2])
    def ge(self, *args):
      #self.vector_1_2('vcgeq_f32', args)
      self.src += '%s = vcvtq_f32_u32(vcgeq_f32(%s, %s));'%(args[0], args[1], args[2])
    def gt(self, *args):
      #self.vector_1_2('vcgtq_f32', args)
      self.src += '%s = vcvtq_f32_u32(vcgtq_f32(%s, %s));'%(args[0], args[1], args[2])
    def le(self, *args):
      #self.vector_1_2('vcleq_f32', args)
      self.src += '%s = vcvtq_f32_u32(vcleq_f32(%s, %s));'%(args[0], args[1], args[2])
    def lt(self, *args):
      #self.vector_1_2('vcltq_f32', args)
      self.src += '%s = vcvtq_f32_u32(vcltq_f32(%s, %s));'%(args[0], args[1], args[2])
    #TODO: more absolute comparisons?
    #Python bit operators
    def bit_and(self, *args):
      #self.vector_1_2('_mm_and_ps', args)
      self.src += '%s = vcvtq_f32_u32(vandq_u32(vcvtq_u32_f32(%s), vcvtq_u32_f32(%s)));'%(args[0], args[1], args[2])
    def bit_andnot(self, *args):
      #self.vector_1_2('_mm_andnot_ps', args)
      self.src += '%s = vcvtq_f32_u32(vbicq_u32(vcvtq_u32_f32(%s), vcvtq_u32_f32(%s)));'%(args[0], args[1], args[2])
    def bit_or(self, *args):
      #self.vector_1_2('_mm_or_ps', args)
      self.src += '%s = vcvtq_f32_u32(vorrq_u32(vcvtq_u32_f32(%s), vcvtq_u32_f32(%s)));'%(args[0], args[1], args[2])
    def bit_xor(self, *args):
      #self.vector_1_2('_mm_xor_ps', args)
      self.src += '%s = vcvtq_f32_u32(veorq_u32(vcvtq_u32_f32(%s), vcvtq_u32_f32(%s)));'%(args[0], args[1], args[2])
    def bit_not(self, *args):
      args += ('MASK_TRUE',)
      self.bit_xor(*args) #TODO: double check
    #Python boolean operators
    def bool_and(self, *args):
      self.bit_and(*args)
    def bool_or(self, *args):
      self.bit_or(*args)
    def bool_not(self, *args):
      self.bit_not(*args)
    #Python intrinsics
    def abs(self, *args):
      self.vector_1_1('vabsq_f32', args)
    def max(self, *args):
      self.vector_1_2('vmaxq_f32', args)
    def min(self, *args):
      self.vector_1_2('vminq_f32', args)
    #def round(self, *args):
    #  args += ('(_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)',)
    #  self.vector_1_2('_mm_round_ps', args) #TODO

    #Math functions (binary)
    def atan2(self, *args):
      self.scalar_1_2('atan2', args)
    def copysign(self, *args):
      self.scalar_1_2('copysign', args)
    def fmod(self, *args):
      self.scalar_1_2('fmod', args)
    def hypot(self, *args):
      self.scalar_1_2('hypot', args)
    #Math functions (unary)
    def acos(self, *args):
      self.scalar_1_1('acos', args)
    def acosh(self, *args):
      self.scalar_1_1('acosh', args)
    def asin(self, *args):
      self.scalar_1_1('asin', args)
    def asinh(self, *args):
      self.scalar_1_1('asinh', args)
    def atan(self, *args):
      self.scalar_1_1('atan', args)
    def atanh(self, *args):
      self.scalar_1_1('atanh', args)
    def ceil(self, *args):
      args += ('(_MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)',)
      self.vector_1_2('_mm_round_ps', args) #TODO
    def cos(self, *args):
      self.scalar_1_1('cos', args)
    def cosh(self, *args):
      self.scalar_1_1('cosh', args)
    def erf(self, *args):
      self.scalar_1_1('erf', args)
    def erfc(self, *args):
      self.scalar_1_1('erfc', args)
    def exp(self, *args):
      self.scalar_1_1('exp', args)
    def expm1(self, *args):
      self.scalar_1_1('expm1', args)
    def fabs(self, *args):
      #self.scalar_1_1('fabs', args)
      self.vector_1_1('vabsq_f32', args)
    def floor(self, *args):
      args += ('(_MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)',)
      self.vector_1_2('_mm_round_ps', args) #TODO
    def gamma(self, *args):
      self.scalar_1_1('tgamma', args)
    def lgamma(self, *args):
      self.scalar_1_1('lgamma', args)
    def log(self, *args):
      self.scalar_1_1('log', args)
    def log10(self, *args):
      self.scalar_1_1('log10', args)
    def log1p(self, *args):
      self.scalar_1_1('log1p', args)
    def log2(self, *args):
      self.scalar_1_1('log2', args)
    def sin(self, *args):
      self.scalar_1_1('sin', args)
    def sinh(self, *args):
      self.scalar_1_1('sinh', args)
    #def sqrt(self, *args):
    #  self.vector_1_1('_mm_sqrt_ps', args) #TODO: reciprocal square root?
    def tan(self, *args):
      self.scalar_1_1('tan', args)
    def tanh(self, *args):
      self.scalar_1_1('tanh', args)
    #def trunc(self, *args):
    #  args += ('(_MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)',)
    #  self.vector_1_2('_mm_round_ps', args) #TODO

  ################################################################################
  # Translates kernel operations into vectorized C++ code (NEON, 32-bit uint)
  ################################################################################
  class NEON_UInt32(Translator):
    def __init__(self, src, size):
      Compiler_Arm.Translator.__init__(self, src, size)
      self.type = 'uint32x4_t'
      self.test = 'vmovemask_u32' #TODO: double check
      self.insert = 'vsetq_lane_u32' #TODO: double check args order changed '_mm_insert_epi32'
      self.extract = 'vgetq_lane_u32'#TODO: same order!! '_mm_extract_epi32'
      self.all_zeroes = '_mm_testz_si128'#TODO?
    #Misc
    def setup(self):
      self.src += 'const %s MASK_FALSE = vdupq_n_u32(0);'%(self.type)
      self.src += 'const %s MASK_TRUE = vdupq_n_u32(0xffffffff);'%(self.type)
      self.src += 'const %s SIGN_BITS = vdupq_n_u32(0x80000000);'%(self.type)
      for i in range(self.size): #TODO
        slots = []
        for j in range(self.size):
          slots.append('%d'%(1 if i == j else 0))
        slots.reverse()
        lane = '_mm_xor_si128(MASK_TRUE, _mm_cmpeq_epi32(MASK_FALSE, _mm_set_epi32(%s)))'%(', '.join(slots))
        self.src += 'const %s MASK_LANE_%d = %s;'%(self.type, i, lane)
    def set(self, *args):
      self.vector_1_1('vdupq_n_u32', args)
    def load(self, *args): #TODO
      args = (args[0], '(const %s*)(%s)'%(self.type, args[1]))
      self.vector_1_1('vld1q_u32', args)
    def store(self, *args): #TODO
      args = ('(%s*)(%s)'%(self.type, args[0]), args[1])
      self.vector_0_2('vst1q_u32', args)
    def mask(self, *args): #TODO: double check
      #(input, output, mask) = args
      #self.mask_1_2(input, output, mask, '_mm_or_si128', '_mm_and_si128', '_mm_andnot_si128')
      args = (args[1], args[2], args[0], args[1])
      self.vector_1_3('vbslq_u32', args)
    #Python arithmetic operators
    def add(self, *args):
      self.vector_1_2('vaddq_u32', args)
    def sub(self, *args):
      self.vector_1_2('vsubq_u32', args)
    def mul(self, *args):
      self.vector_1_2('vmulq_u32', args)
    def div(self, *args):
      self.scalar_1_2_op('/', args)
    def floordiv(self, *args):
      self.div(*args)
    def mod(self, *args):
      self.scalar_1_2_op('%', args)
    #Python comparison operators
    def eq(self, *args):
      self.vector_1_2('vceqq_u32', args)
    def ne(self, *args): #TODO: double check
      #Not equal
      self.bit_not(args[0], 'vceqq_u32(%s, %s)'%(args[1], args[2]))
    def ge(self, *args):
      #Not less than
      #args = (args[0], '_mm_xor_si128(SIGN_BITS, %s)'%(args[1]), '_mm_xor_si128(SIGN_BITS, %s)'%(args[2]))
      #self.bit_not(args[0], '_mm_cmplt_epi32(%s, %s)'%(args[1], args[2]))
      self.vector_1_2('vcgeq_u32', args)
    def gt(self, *args):
      #args = (args[0], '_mm_xor_si128(SIGN_BITS, %s)'%(args[1]), '_mm_xor_si128(SIGN_BITS, %s)'%(args[2]))
      #self.vector_1_2('_mm_cmpgt_epi32', args)
      self.vector_1_2('vcgtq_u32', args)
    def le(self, *args):
      #Not greater than
      #args = (args[0], '_mm_xor_si128(SIGN_BITS, %s)'%(args[1]), '_mm_xor_si128(SIGN_BITS, %s)'%(args[2]))
      #self.bit_not(args[0], '_mm_cmpgt_epi32(%s, %s)'%(args[1], args[2]))
      self.vector_1_2('vcleq_u32', args)
    def lt(self, *args):
      #args = (args[0], '_mm_xor_si128(SIGN_BITS, %s)'%(args[1]), '_mm_xor_si128(SIGN_BITS, %s)'%(args[2]))
      #self.vector_1_2('_mm_cmplt_epi32', args)
      self.vector_1_2('vcltq_u32', args)
    #Python bit operators
    def bit_and(self, *args):
      self.vector_1_2('vandq_u32', args)
    def bit_andnot(self, *args):
      self.vector_1_2('vbicq_u32', args)
    def bit_or(self, *args):
      self.vector_1_2('vorrq_u32', args)
    def bit_xor(self, *args):
      self.vector_1_2('veorq_u32', args)
    def bit_not(self, *args):
      #args += ('MASK_TRUE',)
      #self.bit_xor(*args)
      self.vector_1_2('vmvnq_u32', args)
    def shift_left(self, *args): #TODO
      if len(args) == 4 and args[3]:
        #Shifting all lanes by the same constant
        self.vector_1_2('_mm_slli_epi32', args[0:3])
      else:
        #Shift each lane separately
        (output, left, right) = args
        for i in range(self.size):
          mask = 'MASK_LANE_%d'%(i)
          input = '_mm_slli_epi32(%s, _mm_extract_epi32(%s, %d))'%(left, right, i)
          self.src += '%s = _mm_or_si128(_mm_and_si128(%s, %s), _mm_andnot_si128(%s, %s));'%(output, mask, input, mask, output)
    def shift_right(self, *args): #TODO
      if len(args) == 4 and args[3]:
        #Shifting all lanes by the same constant
        self.vector_1_2('_mm_srli_epi32', args[0:3])
      else:
        #Shift each lane separately
        (output, left, right) = args
        for i in range(self.size):
          mask = 'MASK_LANE_%d'%(i)
          input = '_mm_srli_epi32(%s, _mm_extract_epi32(%s, %d))'%(left, right, i)
          self.src += '%s = _mm_or_si128(_mm_and_si128(%s, %s), _mm_andnot_si128(%s, %s));'%(output, mask, input, mask, output)
    #Python boolean operators
    def bool_and(self, *args):
      self.bit_and(*args)
    def bool_or(self, *args):
      self.bit_or(*args)
    def bool_not(self, *args):
      self.bit_not(*args)
    #Python intrinsics
    def max(self, *args):
      #args = (args[0], '_mm_xor_si128(SIGN_BITS, %s)'%(args[1]), '_mm_xor_si128(SIGN_BITS, %s)'%(args[2]))
      #self.src += '%s = _mm_xor_si128(SIGN_BITS, _mm_max_epi32(%s, %s));'%(args[0], args[1], args[2])
      self.vector_1_2('vmaxq_u32', args)
    def min(self, *args):
      #args = (args[0], '_mm_xor_si128(SIGN_BITS, %s)'%(args[1]), '_mm_xor_si128(SIGN_BITS, %s)'%(args[2]))
      #self.src += '%s = _mm_xor_si128(SIGN_BITS, _mm_min_epi32(%s, %s));'%(args[0], args[1], args[2])
      self.vector_1_2('vminq_u32', args)
    #Experimental - array access
    def array_read(self, *args): #TODO
      (output, array, index, stride) = args
      for i in range(self.size):
        mask = 'MASK_LANE_%d'%(i)
        input = '_mm_set1_epi32(%s[%d + _mm_extract_epi32(%s, %d)])'%(array, stride * i, index, i)
        self.src += '%s = _mm_or_si128(_mm_and_si128(%s, %s), _mm_andnot_si128(%s, %s));'%(output, mask, input, mask, output)
    def array_write(self, *args): #TODO
      (input, array, index, stride) = args
      for i in range(self.size):
        self.src += '%s[%d + _mm_extract_epi32(%s, %d)] = _mm_extract_epi32(%s, %d);'%(array, stride * i, index, i, input, i)
