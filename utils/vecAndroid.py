import sys
import os
import inspect
import subprocess
import importlib.util
import shutil

print("\n--- This is vecAndroid runner ---\n")

# Get the total number of args passed to the demo.py
total = len(sys.argv)
 
# Get the arguments list 
cmdargs = str(sys.argv)


# Print it
# print ("The total numbers of args passed to the script: %d " % total)
# print ("Args list: %s " % cmdargs)

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

def makeArr(kernelFile, outputDir):
	# Import the file containing all the kernel functions as a module
	# try

	print("Reading kernel functions ...", end="")
	spec = importlib.util.spec_from_file_location("module.name", kernelFile)
	fileAsModule = importlib.util.module_from_spec(spec)
	spec.loader.exec_module(fileAsModule)
	all_kernels = inspect.getmembers(fileAsModule, inspect.isfunction)
	print("Done!\n")

	# Some dirs 
	script_parent_dir = os.path.abspath(os.path.dirname(__file__))
	project_dir = script_parent_dir[:script_parent_dir.rfind("/")];
	jni_dir = project_dir + "/apitemplate/src/main/jni"

	print("Vectorizing using NEON intrinsics...", end="")
	kernel_names = []
	for k in all_kernels:
		kernel_names += [k[0]]
		# invoke revised version of vecpy
		continue
	print("Done!\n")

	
	# Modify Android.mk
	print("Generate new Android.mk file ...", end="")
	new_androidmk_file = open(jni_dir+"/Andoirdbk.mk", 'w')
	new_androidmk_file.write("LOCAL_PATH := $(call my-dir)\n")
	new_androidmk_file.write(androidmk_singlemodule_template % ("vecandroid", "vecandroid.c", "vecandroid-intrinsics.c"))
	for kernel_name in kernel_names:
		new_androidmk_file.write(androidmk_singlemodule_template % (kernel_name, "vecandroid_" + kernel_name + "_core.cpp", "vecandroid_" + kernel_name + "_kernel.c"))
	new_androidmk_file.write("$(call import-module,cpufeatures)\n")
	print("Done!\n")

	# Build ARR
	print("Compiling into AAR (takes a while because of Gradle build) ...", end="")
	# parent_dir = os.path.abspath(os.path.dirname(__file__))
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

makeArr("/Users/Jason/Documents/S16-project/15418/vecAndroid/utils/kernels_example.py", "/Users/Jason/Documents/S16-project/15418/vecAndroid/utils/")

# script_parent_dir = os.path.abspath(os.path.dirname(__file__))
# project_dir = script_parent_dir[:script_parent_dir.rfind("/")];
# jni_dir = project_dir + "/apitemplate/src/main/jni"

# print(androidmk_singlemodule_template % (1,2,3))
# new_androidmk_file = open(jni_dir+"/Andoirdbk.mk", 'w')
# new_androidmk_file.write("LOCAL_PATH := $(call my-dir)\n")
# new_androidmk_file.write(androidmk_singlemodule_template % ("vecandroid", "vecandroid.c", "vecandroid-intrinsics.c"))
# for kernel_name in kernel_names:
# 	new_androidmk_file.write(androidmk_singlemodule_template % (kernel_name, "vecpy_" + kernel_name + "_core.cpp", "vecpy_" + kernel_name + "_kernel.c"))
# new_androidmk_file.write("$(call import-module,cpufeatures)\n")



# parent_dir = os.path.abspath(os.path.dirname(__file__))
# template_dir = parent_dir[:parent_dir.rfind("/")] + "/apitemplate"
# os.chdir(template_dir)
# subprocess.call(["gradle","assemble"]) 

# spec = importlib.util.spec_from_file_location("module.name", "/Users/Jason/Documents/S16-project/15418/vecpyt/volume.py")
# fileAsModule = importlib.util.module_from_spec(spec)
# spec.loader.exec_module(fileAsModule)
# all_kernels = inspect.getmembers(fileAsModule, inspect.isfunction)
# print(all_kernels[0][1])
