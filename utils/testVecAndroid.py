# Example from David Farrow, vecpy, (2015), GitHub repository, https://github.com/undefx/vecpy.git
# Modified by Yongshan Ding in May, 2016

#Import VecPy
from vecandroid.runtime import *
from vecandroid.compiler_constants import *

#Define the kernel
def volume(radius, volume):
  volume = (4/3 * math.pi) * (radius ** 3)

#Generate some data
def data():
  array = get_array('f', 10)
  for i in range(len(array)): array[i] = (.1 + i/10)
  return array
radii, volumes = data(), data()

#Call VecPy to generate the native module
path = '/Users/yongshanding/AndroidStudioProjects/vecAndroid/apitemplate/src/main/' #TODO: Modify this if neccessary
vectorize(volume, Options(Architecture.neon, DataType.float), path)

