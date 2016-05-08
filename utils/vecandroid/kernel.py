# Python vectorization structure from David Farrow, vecpy, (2015), GitHub repository, https://github.com/undefx/vecpy.git
# Modified by Yongshan Ding in May, 2016

"""
The Kernel is a set of operations to be performed on a set of data.
"""


#Holds information about variables
class Variable:
  #A unique variable identifier
  index = 0
  def __init__(self, name=None, is_arg=False, is_uniform=False, is_fuse=False, is_temp=False, is_mask=False, stride=1, value=None):
    if name is None:
      if value is None:
        if is_mask:
          prefix = 'mask'
        else:
          prefix = 'var'
      else:
        prefix = 'lit'
      name = '%s%03d'%(prefix, Variable.index)
    #The variable name
    self.name = name
    #Whether or not the variable is an argument
    self.is_arg = is_arg
    #Whether or not the variable is a uniform (scalar) argument - read-only
    self.is_uniform = is_uniform
    #Whether or not the variable is a fuse (scalar) argument - write-only
    self.is_fuse = is_fuse
    #Whether or not the variable is implicit
    self.is_temp = is_temp
    #Whether or not this variable is a bit mask
    self.is_mask = is_mask
    #The stride of this array variable
    self.stride = stride
    #The value of this literal
    self.value = value
    #A unique identifier for this variable
    self.index = Variable.index
    #Whether or not this argument is read from
    self.is_input = False
    #Whether or not this argument is written to
    self.is_output = False
    #Increment variable identifier
    Variable.index += 1

#Built-in Python binary operators
class Operator:
  add = '+'
  subtract = '-'
  multiply = '*'
  divide = '/'
  divide_int = '//'
  mod = '%'
  pow = '**'
  eq = '=='
  ne = '!='
  gt = '>'
  ge = '>='
  lt = '<'
  le = '<='
  bit_and = '&'
  bit_andnot = '&~'
  bit_or = '|'
  bit_xor = '^'
  bit_not = '~'
  bool_and = '&&'
  bool_or = '||'
  bool_not = '!'
  shift_left = '<<'
  shift_right = '>>'

#Built-in Python functions
class Intrinsic:
  #Functions taking two arguments
  binary_functions = (
    'max',
    'min',
    'pow',
  )
  #Functions taking one argument
  unary_functions = (
    'abs',
    'round',
  )

#Functions from Python's math module
class Math:
  #Functions taking two arguments
  binary_functions = (
    'atan2',
    'copysign',
    'fmod',
    'hypot',
    #'ldexp',
    'pow',
  )
  #Functions taking one argument
  unary_functions = (
    'acos',
    'acosh',
    'asin',
    'asinh',
    'atan',
    'atanh',
    'ceil',
    'cos',
    'cosh',
    'erf',
    'erfc',
    'exp',
    'expm1',
    'fabs',
    #'factorial',
    'floor',
    #'frexp',
    'gamma',
    #'isfinite',
    #'isinf',
    #'isnan',
    'lgamma',
    'log',
    'log10',
    'log1p',
    'log2',
    #'modf',
    'sin',
    'sinh',
    'sqrt',
    'tan',
    'tanh',
    'trunc',
  )

#Represents an operation on two variables
class BinaryOperation:
  def __init__(self, left, op, right):
    #The left-side variable
    self.left = left
    #The operation or function
    self.op = op
    #The right-side variable
    self.right = right

#Represents an operation on one variable
class UnaryOperation:
  def __init__(self, op, var):
    #The operation or function
    self.op = op
    #The variable
    self.var = var

#Represents a comparison of two variables
class ComparisonOperation:
  def __init__(self, left, op, right):
    #The left-side variable
    self.left = left
    #The operation
    self.op = op
    #The right-side variable
    self.right = right

#Represents reading an array element
class ArrayAccess:
  def __init__(self, array, index, is_read):
    #The array
    self.array = array
    #The index
    self.index = index
    #Whether or not data is being read or written
    self.is_read = is_read

#Represents a single statement
class Statement:
  pass

#A list of statements
class Block:
  def __init__(self, mask):
    #Initialize an empty list of statements
    self.code = []
    #The mask used inside of this block
    self.mask = mask
  #Appends a statement to the code block
  def add(self, stmt):
    if isinstance(stmt, list):
      for s in stmt:
        self.add(s)
      return
    #Sanity checks
    if not isinstance(stmt, Statement):
      raise Exception('Can\'t add that (%s) to the code block'%(stmt.__class__))
    #Append the statement to the code block
    self.code.append(stmt)

#Represents an assignment of some expression to a variable
class Assignment(Statement):
  def __init__(self, var, expr, vector_only=False, mask=None):
    #Sanity check
    if var is None:
      raise Exception('var is None')
    #The variable to assign the result to
    self.var = var
    #The expression to be evaluated
    self.expr = expr
    #Whether this is only used in the vector kernel
    self.vector_only = vector_only
    #The vector mask determining which lanes are updated
    self.mask = mask

#Represents an if-else branch
class IfElse(Statement):
  def __init__(self, if_mask, else_mask):
    #The code to execute if the condition is true
    self.if_block = Block(if_mask)
    #The code to execute if the condition is false (optional)
    self.else_block = Block(else_mask)

#Represents a while loop
class WhileLoop(Statement):
  def __init__(self, mask):
    #The code to execute while the condition is true
    self.block = Block(mask)

#Represents a comment to include in the module's source code
class Comment(Statement):
  def __init__(self, comment):
    #The comment string
    self.comment = comment

#Represents an entire kernel function
class Kernel:
  def __init__(self, name):
    #The name of the function
    self.name = name
    #A table of all variables
    self.variables = {}
    #A table of argument variables
    self.arguments = {}
    #A table of literal variables
    self.literals = {}
    #The default docstring
    self.docstring = 'An undocumented (but probably awesome) kernel function.'
    #Literal masks
    self.mask_true = Variable('MASK_TRUE', False, False, False, True, None)
    self.mask_false = Variable('MASK_FALSE', False, False, False, True, None)
    #The kernel's code block
    self.block = Block(self.mask_true)

  #Returns the variable with the given name
  def get_variable(self, name):
    if name in self.variables:
      return self.variables[name]
    else:
      return None

  #Returns the literal with the given value
  def get_literal(self, value):
    if value in self.literals:
      return self.literals[value]
    else:
      return None

  #Adds a new variable to the kernel
  def add_variable(self, var):
    #Add this variable to the variables dictionary
    self.variables[var.name] = var
    if var.is_arg:
      #Add this argument to the arguments dictionary
      self.arguments[var.name] = var
    if var.value is not None:
      #Add this literal to the literals dictionary
      self.literals[var.value] = var
    #Return the variable
    return var

  #Replaces the default docstring
  def set_docstring(self, docstring):
    self.docstring = docstring

  #Returns a list of arguments sorted by order of appearance
  def get_arguments(self, input=None, output=None, uniform=None, fuse=None, array=None):
    args = sorted(list(self.arguments.values()), key=lambda arg: arg.index)
    return [arg for arg in args if (input is None or input == arg.is_input) and (output is None or output == arg.is_output) and (uniform is None or uniform == arg.is_uniform) and (fuse is None or fuse == arg.is_fuse) and (array is None or array == (arg.stride > 1))]

  #Returns a list of literals sorted by value
  def get_literals(self):
    return sorted(list(self.literals.values()), key=lambda lit: lit.value)

  #Returns a list of all variables sorted by order of appearance
  def get_variables(self, mask=None, uniform=None, fuse=None, array=None):
    vars = sorted(list(self.variables.values()), key=lambda var: var.index)
    return [var for var in vars if var.value is None and (mask is None or mask == var.is_mask)  and (uniform is None or uniform == var.is_uniform) and (fuse is None or fuse == var.is_fuse) and (array is None or array == (var.stride > 1))]
