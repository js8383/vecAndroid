import sys
import os
import inspect
import subprocess
import importlib.util
import shutil
from neoncompile.runtime import *
from neoncompile.compiler_constants import *

androidmk_singlemodule_template = '''
include $(CLEAR_VARS)

LOCAL_MODULE := %s
LOCAL_SRC_FILES := %s

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI), armeabi-v7a x86))
    LOCAL_CFLAGS := -DHAVE_NEON=1
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -mssse3
endif
    LOCAL_SRC_FILES += %s.neon
endif

LOCAL_STATIC_LIBRARIES := cpufeatures
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

'''

pre_exist_files = ["Android.mk", "Application.mk", "neon_more.h", "vecandroid-intrinsics.c", "vecandroid-intrinsics.h", "vecandroid.c", "samples"]

def makeArr(kernelFile, outputDir):
	# Import the file containing all the kernel functions as a module
	# try

	print("Reading kernel functions ...", end="")
	try:
		spec = importlib.util.spec_from_file_location("module.name", kernelFile)
		fileAsModule = importlib.util.module_from_spec(spec)
		spec.loader.exec_module(fileAsModule)
		all_kernels = inspect.getmembers(fileAsModule, inspect.isfunction)
	except:
		print("Please make sure your python file is valid and contains no indentation error!")
		return
	print("Done!\n")

	# Some dirs 
	script_parent_dir = os.path.abspath(os.path.dirname(__file__))
	project_dir = script_parent_dir[:script_parent_dir.rfind("/")];
	main_dir = project_dir + "/apitemplate/src/main"
	jni_dir = project_dir + "/apitemplate/src/main/jni"

	print("Vectorizing using NEON intrinsics...\n", end="")
	# Clean direcotry before generating the VecAndroidAPI interface
	filename1 = main_dir+"/java/com/vecandroid/KernelOps.java"
	filename2 = main_dir+"/java/com/vecandroid/VecAndroidApi.java"
	if os.path.exists(filename1):
		os.remove(filename1)
	if os.path.exists(filename2):
		os.remove(filename2)

	file_or_dir_under_jni = [f for f in os.listdir(jni_dir)]
	for filename3 in file_or_dir_under_jni:
		if filename3 not in pre_exist_files and filename3[0] != ".":
			os.remove(jni_dir + "/" + filename3)

	kernel_names = []
	for k in all_kernels:
		kernel_names += [k[0]]
		# invoke revised version of vecpy
		vectorize(k[1], Options(Architecture.neon, DataType.float), main_dir)
		continue
	print("Done!\n")

	
	# Modify Android.mk
	print("Generate new Android.mk file ...", end="")
	new_androidmk_file = open(jni_dir+"/Android.mk", 'w')
	new_androidmk_file.write("LOCAL_PATH := $(call my-dir)\n")
	new_androidmk_file.write(androidmk_singlemodule_template % ("vecandroid", "vecandroid.c", "vecandroid-intrinsics.c"))
	for kernel_name in kernel_names:
		new_androidmk_file.write(androidmk_singlemodule_template % (kernel_name, "neoncompile_" + kernel_name + "_core.cpp", "neoncompile_" + kernel_name + "_kernel.c"))
	new_androidmk_file.write("$(call import-module,cpufeatures)\n")
	print("Done!\n")

	# Build ARR
	print("Compiling into AAR (takes a while because of Gradle build) ...", end="")
	template_dir = project_dir + "/apitemplate"
	os.chdir(template_dir)
	with open(os.devnull, "w") as fnull:
		subprocess.call(["gradle","assemble"], stdout = fnull, stderr = fnull) 
	print("Done!\n")

	# Copy the ARR to the outputDir
	shutil.copy(project_dir + "/apitemplate/build/outputs/aar/apitemplate-release.aar", outputDir)
	dst_file = os.path.join(outputDir, "apitemplate-release.aar")
	new_dst_file_name = os.path.join(outputDir, "vecandroidlib.aar")
	os.rename(dst_file, new_dst_file_name)

	print("Check out the output AAR at:\n" + new_dst_file_name)

	return

def environment_check():
	return


# Get the total number of args passed to the demo.py
total = len(sys.argv)
 
# Get the arguments list 
cmdargs = str(sys.argv)

# Print it
# print ("The total numbers of args passed to the script: %d " % total)
# print ("Args list: %s " % cmdargs)

if total < 2:
	print("Not enough arguments. Use '-help' to check usage.")

flag = sys.argv[1]

if flag == "-compile":
	if total != 4:
		print("There should be 2 arguments after '-compile'.")
		exit(0)
	if not (os.path.isfile(sys.argv[2]) and os.path.isdir(sys.argv[3])):
		print("Your kernels .py file or output directory is not valid!")
		exit(0)
	else:
		makeArr(sys.argv[2], sys.argv[3])
		exit(0)
elif flag == "-help":
	print("See usage in our project report.")
else:
	print("Invalid flag.")

# Example usage
# python3 vecAndroidRun.py -compile /Users/Jason/Documents/S16-project/15418/vecAndroid/utils/kernels_example.py /Users/Jason/Documents/S16-project/15418/vecAndroid/utils/
