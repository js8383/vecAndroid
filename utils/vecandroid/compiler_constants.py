# Compiler from David Farrow, vecpy, (2015), GitHub repository, https://github.com/undefx/vecpy.git
# Modified by Yongshan Ding in May, 2016

#Supported architectures
class Architecture:
  #Plain C++ code
  generic = {'level': 100, 'name': 'Generic', 'size': 1, 'flag': ''  }
  #C++ with ARM NEON intrinsics
  neon    = {'level': 200, 'name': 'NEON'   , 'size': 4, 'flag': ''  }

  #Utility functions
  def is_generic(arch):
    return (arch['level'] // 100) == 1
  def is_arm(arch):
   return (arch['level'] // 100) == 2

#Supported data types
class DataType:
  float  = 'float'
  uint32 = 'unsigned int'

  #Utility functions
  def is_floating(type):
    return type in (DataType.float,)
  def is_integral(type):
    return type in (DataType.uint32,)

#Available language bindings
class Binding:
  all    = 'java' #'*all*'
  java   = 'java'

#Compile time options
class Options:
  def __init__(self, arch, type, bindings=(Binding.java,), threads=None, java_package='com_vecandroid_api'):
    if arch is None or type is None or bindings is None or len(bindings) == 0:
      raise Exception('Invalid options')
    #Target architecture
    self.arch = arch
    #Kernel data type
    self.type = type
    #Language API bindings
    self.bindings = bindings
    #Number of threads to spawn
    self.threads = threads
    #Java package name
    self.java_package = java_package
  def show(self, path=0):
    print('=' * 40)
    print('VecAndroid options')
    print('-' * 40)
    print('Data Type:         ' + self.type)
    print('Threads:           ' + str(self.threads))
    print('Architecture:      ' + self.arch['name'])
    #print('Language Bindings: ' + ','.join(self.bindings))
    if Binding.all in self.bindings or Binding.java in self.bindings:
      print('Java Package:      ' + str(self.java_package))
    if path == 0:
      print('Save path:         ' + '(Not specified.)')
    else:
      print('Save path:         ' + path)
    print('=' * 40)

#Indent amount
def get_indent(level):
  return ' ' * (4 * level)

#Source code formatter
class Formatter:
  def __init__(self):
    self.level = 0
    self.code = ''
  def section(self, title):
    width = 78
    left = (width - len(title)) // 2
    right = width - len(title) - left
    self.append('/' + ('*' * width) + '*')
    self.append('*' + (' ' * left) + title + (' ' * right) + '*')
    self.append('*' + ('*' * width) + '/')
  def __iadd__(self, other):
    self.append(other)
    return self
  def append(self, code, end='\n'):
    self.code += get_indent(self.level) + code + end
  def indent(self):
    self.level += 1
  def unindent(self):
    self.level -= 1
    if self.level < 0:
      raise Exception('Negative indent')
  def get_code(self):
    if self.level != 0:
      raise Exception('Still indented')
    return self.code
