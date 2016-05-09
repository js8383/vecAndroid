# Compilter from David Farrow, vecpy, (2015), GitHub repository, https://github.com/undefx/vecpy.git
# Modified by Yongshan Ding in May, 2016

"""
The Compiler uses the Parser and the Kernel to translate a pure Python function
into a native library. This library can be invoked though several language bindings,
in particular as a native Python module.
"""


import subprocess
import os
from vecandroid.kernel import *
from vecandroid.compiler_constants import *
from vecandroid.compiler_generic import Compiler_Generic
from vecandroid.compiler_arm import Compiler_Arm

class Compiler:

  #Utility functions: output file names

  def get_java_file(k):
    return 'vecandroid_%s_java.h'%(k.name)

  def get_kernel_h_file(k):
    return 'vecandroid_%s_kernel.h'%(k.name)

  def get_kernel_file(k):
    return 'vecandroid_%s_kernel.c'%(k.name)

  def get_core_file(k):
    return 'vecandroid_%s_core.cpp'%(k.name)

  #Generates the core file
  def compile_core(k, options, include_files, path):
    if Architecture.is_generic(options.arch):
      suffix = 'scalar'
    elif Architecture.is_arm(options.arch):
      suffix = 'vector'
    else:
      raise Exception('Target architecture not supported (%s)'%(options.arch['name']))
    src = Formatter()
    src.section('Generated core')
    #Includes
    src += '//Includes'
    src += '#include <pthread.h>'
    src += '#include <stdio.h>'
    src += 'extern "C" {'
    src.indent()
    src += '#include "%s"'%(Compiler.get_kernel_h_file(k))
    src.unindent()
    src += '}'
    src += ''
    #Utility functions
    src += '//Utility functions'
    src += 'static void* threadStart(void* v) {'
    src.indent()
    src += '%s_%s((KernelArgs*)v);'%(k.name, suffix)
    src += 'return NULL;'
    src.unindent()
    src += '}'
    src += 'static bool isAligned(void* data) {'
    src.indent()
    src += 'return reinterpret_cast<uint64_t>(data) %% %dUL == 0UL;'%(options.arch['size'] * 4)
    src.unindent()
    src += '}'
    src += 'static bool checkArgs(KernelArgs* args) {'
    src.indent()
    for arg in k.get_arguments(uniform=False):
      src += 'if(!isAligned(args->%s)) {'%(arg.name)
      src.indent()
      src += 'printf("Array not aligned (%s)\\n");'%(arg.name)
      src += 'return false;'
      src.unindent()
      src += '}'
    src += 'return true;'
    src.unindent()
    src += '}'
    src += ''
    #Unified core for all programming interfaces
    src += '//Unified core function'
    src += 'static bool run(KernelArgs* args) {'
    src.indent()
    src += 'if(!checkArgs(args)) {'
    src.indent()
    src += 'printf("Arguments are invalid\\n");'
    src += 'return false;'
    src.unindent()
    src += '}'
    src += '//Compile-time constants'
    src += 'const uint64_t vectorSize = %d;'%(options.arch['size'])
    src += 'const uint64_t numThreads = %d;'%(options.threads)
    src += '//Division of labor'
    src += 'const uint64_t vectorsPerThread = args->N / (vectorSize * numThreads);'
    src += 'const uint64_t elementsPerThread = vectorsPerThread * vectorSize;'
    src += '//Execute on multiple threads'
    src += 'uint64_t offset = 0;'
    src += 'if(elementsPerThread > 0) {'
    src.indent()
    src += 'pthread_t* threads = new pthread_t[numThreads];'
    src += 'KernelArgs* threadArgs = new KernelArgs[numThreads];'
    src += 'for(uint64_t t = 0; t < numThreads; t++) {'
    src.indent()
    for arg in k.get_arguments():
      if arg.is_uniform:
        src += 'threadArgs[t].%s = args->%s;'%(arg.name, arg.name)
      elif arg.is_fuse:
        src += 'threadArgs[t].%s = &args->%s[0];'%(arg.name, arg.name)
      else:
        offset = 'offset'
        if arg.stride > 1:
          offset = '%s * %d'%(offset, arg.stride)
        src += 'threadArgs[t].%s = &args->%s[%s];'%(arg.name, arg.name, offset)
    src += 'threadArgs[t].N = elementsPerThread;'
    src += 'offset += elementsPerThread;'
    src += 'pthread_create(&threads[t], NULL, threadStart, (void*)&threadArgs[t]);'
    src.unindent()
    src += '}'
    src += 'for(uint64_t t = 0; t < numThreads; t++) {'
    src.indent()
    src += ' pthread_join(threads[t], NULL); '
    src.unindent()
    src += '}'
    src += 'delete [] threads;'
    src += 'delete [] threadArgs;'
    src.unindent()
    src += '}'
    #src += 'printf("Vector: %dx%dx%d=%d | Scalar: %d\\n", vectorsPerThread, vectorSize, numThreads, offset, args->N - offset);'
    src += '//Handle any remaining elements'
    src += 'if(offset < args->N) {'
    src.indent()
    src += 'KernelArgs lastArgs;'
    for arg in k.get_arguments():
      if arg.is_uniform:
        src += 'lastArgs.%s = args->%s;'%(arg.name, arg.name)
      elif arg.is_fuse:
        src += 'lastArgs.%s = &args->%s[0];'%(arg.name, arg.name)
      else:
        offset = 'offset'
        if arg.stride > 1:
          offset = '%s * %d'%(offset, arg.stride)
        src += 'lastArgs.%s = &args->%s[%s];'%(arg.name, arg.name, offset)
    src += 'lastArgs.N = args->N - offset;'
    src += '%s_scalar(&lastArgs);'%(k.name)
    src.unindent()
    src += '}'
    src += 'return true;'
    src.unindent()
    src += '}'
    src += ''
    #Additional includes for each programming language
    src += '//Additional includes for each programming language'
    for file in include_files:
      src += '#include "%s"'%(file)
    src += ''
    #Save code to file
    file_name = Compiler.get_core_file(k)
    jnipath = os.path.join(path, 'jni')
    if not os.path.exists(jnipath):
      os.makedirs(jnipath)
    fullpath = os.path.join(jnipath, file_name)
    with open(fullpath, 'w') as file:
      file.write(src.get_code())
    print('Saved to file: %s'%(fullpath))

  #Generates the Java API
  def compile_java(k, options, path):
    type = options.type
    args = k.get_arguments()
    if DataType.is_floating(type):
      buffer_type = 'FloatBuffer'
      uniform_type = 'float'
    elif DataType.is_integral(type):
      buffer_type = 'IntBuffer'
      uniform_type = 'int'
    else:
      raise Exception('Unsupported data type (%s)'%(type))
    #JNI header file
    src = Formatter()
    src.section('Generated entry point for Java')
    #Includes
    src += '//Includes'
    src += '#include <stdlib.h>'
    src += '#include <jni.h>'
    src += ''
    #Wrapper for the core function
    name_str = 'KernelOps_%s'%(k.name)
    if options.java_package is not None:
      name_str = '%s_%s'%(options.java_package, name_str)
      name_str = '_'.join(name_str.split('.'))
    arg_str = ', '.join('j%s vp_%s'%(uniform_type if arg.is_uniform else 'object', arg.name) for arg in args)
    src += '//Wrapper for the core function'
    src += 'extern "C" JNIEXPORT jboolean JNICALL Java_%s(JNIEnv* env, jclass cls, %s) {'%(name_str, arg_str)
    src.indent()
    src += '//Make sure the buffers are directly allocated'
    src += 'jclass Buffer = env->FindClass("java/nio/Buffer");'
    src += 'jmethodID isDirect = env->GetMethodID(Buffer, "isDirect", "()Z");'
    for arg in k.get_arguments(uniform=False):
      src += 'if(!env->CallBooleanMethod(vp_%s, isDirect)) {'%(arg.name)
      src.indent()
      src += 'printf("Buffer not direct (%s)\\n");'%(arg.name)
      src += 'return false;'
      src.unindent()
      src += '}'
    #Get the number of elements from the length of the first buffer
    src += '//Number of elements to process'
    src += 'jlong N = env->GetDirectBufferCapacity(vp_%s);'%(k.get_arguments(uniform=False, array=False)[0].name)
    src += 'if(N == -1) {'
    src.indent()
    src += 'printf("JVM doesn\'t support direct buffers\\n");'
    src += 'return false;'
    src.unindent()
    src += '}'
    src += '//Check length for all buffers'
    for arg in k.get_arguments(uniform=False):
      num = 'N'
      if arg.stride > 1:
        num = '%s * %d'%(num, arg.stride)
      elif arg.is_fuse:
        num = '1'
      src += 'if(env->GetDirectBufferCapacity(vp_%s) != %s) { '%(arg.name, num)
      src.indent()
      src += 'printf("Java buffer sizes don\'t match (%s)\\n");'%(arg.name)
      src += 'return false;'
      src.unindent()
      src += '}'
    src += '//Extract input arrays from buffers'
    src += 'KernelArgs args;'
    for arg in args:
      if arg.is_uniform:
        src += 'args.%s = vp_%s;'%(arg.name, arg.name)
      else:
        src += 'args.%s = (%s*)env->GetDirectBufferAddress(vp_%s);'%(arg.name, type, arg.name)
    src += 'args.N = N;'
    for arg in k.get_arguments(uniform=False):
      src += 'if(args.%s == NULL) {'%(arg.name)
      src.indent()
      src += 'printf("Error retrieving Java buffer (%s)\\n");'%(arg.name)
      src += 'return false;'
      src.unindent()
      src += '}'
    src += '//Run the kernel'
    src += 'return run(&args);'
    src.unindent()
    src += '}'
    src += ''
    #Aligned buffer allocation
    name_str = 'KernelOps_allocate'
    if options.java_package is not None:
      name_str = '%s_%s'%(options.java_package, name_str)
      name_str = '_'.join(name_str.split('.'))
    src += '//Aligned allocation'
    src += 'extern "C" JNIEXPORT jobject JNICALL Java_%s(JNIEnv* env, jclass cls, jlong N) {'%(name_str)
    src.indent()
    src += '//Allocate space'
    src += 'void* buffer = memalign(32, (size_t)N);'
    src += '//if(result != 0) {'
    src.indent()
    src += '//printf("Error allocating buffer (%d)\\n", result);'
    src += '//return NULL;'
    src.unindent()
    src += '//}'
    src += '//Instantiate a java ByteBuffer'
    src += 'jobject byteBuffer = env->NewDirectByteBuffer(buffer, N);'
    src += 'if(byteBuffer == NULL) { '
    src.indent()
    src += 'printf("Error instantiating direct ByteBuffer (not supported by JVM)\\n");'
    src += 'return NULL;'
    src.unindent()
    src += '}'
    src += 'return byteBuffer;'
    src.unindent()
    src += '}'
    src += ''
    #Aligned buffer free
    name_str = 'KernelOps_free'
    if options.java_package is not None:
      name_str = '%s_%s'%(options.java_package, name_str)
      name_str = '_'.join(name_str.split('.'))
    src += '//Free'
    src += 'extern "C" JNIEXPORT jboolean JNICALL Java_%s(JNIEnv* env, jclass cls, jobject buffer) {'%(name_str)
    src.indent()
    src += '//Make sure the buffer is directly allocated'
    src += 'jclass Buffer = env->FindClass("java/nio/Buffer");'
    src += 'jmethodID isDirect = env->GetMethodID(Buffer, "isDirect", "()Z");'
    src += 'if(!env->CallBooleanMethod(buffer, isDirect)) {'
    src.indent()
    src += 'printf("Buffer not direct\\n");'
    src += 'return false;'
    src.unindent()
    src += '}'
    src += '//Free the memory'
    src += 'free(env->GetDirectBufferAddress(buffer));'
    src += 'return true;'
    src.unindent()
    src += '}'
    src += ''
    #Save code to file
    file_name = Compiler.get_java_file(k)
    jnipath = os.path.join(path, 'jni')
    if not os.path.exists(jnipath):
      os.makedirs(jnipath)
    fullpath = os.path.join(jnipath, file_name)
    with open(fullpath, 'w') as file:
      file.write(src.get_code())
    print('Saved to file: %s'%(fullpath))

    ##########################
    # API for java: KernelOps
    ##########################
    src = Formatter()
    src.section('Generated API: KernelOps')
    #Package
    if options.java_package is not None:
      # src += 'package %s;'%(options.java_package)
      src += 'package com.vecandroid;' #TODO: Edit this if necessary
    #Imports
    src += 'import java.nio.*;'
    src += '//import java.util.Arrays;'
    src += ''
    #VecAndroid class
    arg_str = ', '.join('%s %s'%(uniform_type if arg.is_uniform else buffer_type, arg.name) for arg in args)
    src += 'public class KernelOps implements VecAndroidApi.Kernel {'
    src.indent()
    #Helper functions to allocate and free aligned direct buffers
    #JNI wrapper
    src += '//JNI wrappers'
    src += 'public static native boolean %s(%s);'%(k.name, arg_str)
    src += 'private static native ByteBuffer allocate(long N);'
    src += 'private static native boolean free(Buffer buffer);'
    src += '//Helper functions to allocate and free aligned direct buffers'
    src += 'public static %s newBuffer(long N) {'%(buffer_type)
    src.indent()
    src += 'return allocate(N * %d).order(ByteOrder.nativeOrder()).as%s();'%(4, buffer_type)
    src.unindent()
    src += '}'
    src += 'public static boolean deleteBuffer(Buffer buffer) {'
    src.indent()
    src += 'return free(buffer);'
    src.unindent()
    src += '}'
    src += ''
    src += '//Import kernel function library'
    src += 'public KernelOps() { System.loadLibrary("%s"); }'%(k.name)
    src += ''
    src += '//Testing section (implementing functions in VecAndroidApi)'
    src += '@Override'
    src += 'public String foo() {'
    src.indent()
    src += 'long b = 3L;'
    src += 'FloatBuffer b1 = newBuffer(b);'
    src += 'FloatBuffer b2 = newBuffer(b);'
    src += 'float[] b11 = new float[] {1.f, 2.f, 3.f};'
    src += 'float[] b12 = new float[] {1.f, 2.f, 3.f};'
    src += 'b1.put(b11);'
    src += 'b2.put(b12);'
    src += 'b1.position(0);'
    src += 'b2.position(0);'
    src += 'volume1(b1, b2);' #function name
    src += 'return(Float.toString(b2.get(2)));'
    src.unindent()
    src += '}'
    src.unindent()
    src += '}'
    src += ''
    #Save code to file
    file_name = 'KernelOps.java'
    javapath = os.path.join(path, 'java/com/vecandroid')
    if not os.path.exists(javapath):
      os.makedirs(javapath)
    fullpath = os.path.join(javapath, file_name)
    with open(fullpath, 'w') as file:
      file.write(src.get_code())
    print('Saved to file: %s'%(fullpath))

  #Generates the kernel
  def compile_kernel_h(k, options, path):
    src = Formatter()
    src.section('Generated kernel header: %s'%(k.name))
    src += ''
    src += '#ifndef VECANDROID_VOLUME_KERNEL_H'
    src += '#define VECANDROID_VOLUME_KERNEL_H'
    src += ''
    #The KernelArgs struct
    src += '//Kernel arguments'
    src += 'typedef struct {'
    src.indent()
    for arg in k.get_arguments():
      src += '%s%s %s;'%(options.type, '*' if not arg.is_uniform else '', arg.name)
    src += 'uint64_t N;'
    src.unindent()
    src += '} KernelArgs;'
    src += ''
    #Generate an architecture-specific kernel
    src += 'void %s_scalar(KernelArgs* args);'%(k.name)
    src += ''
    if Architecture.is_arm(options.arch):
      src += 'void %s_vector(KernelArgs* args);'%(k.name)
    else:
      raise Exception('Target architecture not implemented (%s)'%(options.arch['name']))
    src += ''
    src += '#endif'
    #Save code to file
    file_name = Compiler.get_kernel_h_file(k)
    jnipath = os.path.join(path, 'jni')
    if not os.path.exists(jnipath):
      os.makedirs(jnipath)
    fullpath = os.path.join(jnipath, file_name)
    with open(fullpath, 'w') as file:
      file.write(src.get_code())
    print('Saved to file: %s'%(fullpath))


  #Generates the kernel
  def compile_kernel(k, options, path):
    src = Formatter()
    src.section('Generated kernel: %s'%(k.name))
    src += '//Integer types'
    src += '#include <stdint.h>'
    src += '#include "vecandroid_volume1_kernel.h"'
    src += ''
    #Generate an architecture-specific kernel
    src += Compiler_Generic.compile_kernel(k, options)
    if Architecture.is_arm(options.arch):
      src += Compiler_Arm.compile_kernel(k, options)
    elif not Architecture.is_generic(options.arch):
      raise Exception('Target architecture not supported (%s)'%(options.arch['name']))
    #Save code to file
    file_name = Compiler.get_kernel_file(k)
    jnipath = os.path.join(path, 'jni')
    if not os.path.exists(jnipath):
      os.makedirs(jnipath)
    fullpath = os.path.join(jnipath, file_name)
    with open(fullpath, 'w') as file:
      file.write(src.get_code())
    print('Saved to file: %s'%(fullpath))

  #Generates all files and compiles the module
  def compile(kernel, options, path):
    #Sanity checks
    if options.arch is None:
      raise Exception('No architecture specified')
    if path == 0:
      raise Exception('No path specified')
    #Auto-detect number of cores
    if options.threads is None or options.threads < 1:
      try:
        import multiprocessing
        options.threads = multiprocessing.cpu_count()
      except(ImportError, NotImplementedError):
        options.threads = 1
    #Show options
    options.show(path)
    #Generate the kernel
    Compiler.compile_kernel_h(kernel, options, path)
    Compiler.compile_kernel(kernel, options, path)
    #Generate API for each language
    include_files = []
    build_flags = [options.arch['flag']]
    if Binding.all in options.bindings or Binding.java in options.bindings:
      Compiler.compile_java(kernel, options, path)
      include_files.append(Compiler.get_java_file(kernel))
      build_flags.append('-I$JAVA_HOME/include/')
      build_flags.append('-I$JAVA_HOME/include/linux/')
    #Generate the core
    Compiler.compile_core(kernel, options, include_files, path)

