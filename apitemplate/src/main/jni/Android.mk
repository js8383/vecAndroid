LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := vecandroid
LOCAL_SRC_FILES := vecandroid.c

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI), armeabi-v7a x86))
    LOCAL_CFLAGS := -DHAVE_NEON=1
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -mssse3
endif
    LOCAL_SRC_FILES += vecandroid-intrinsics.c.neon
endif

LOCAL_STATIC_LIBRARIES := cpufeatures
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := saxpy
LOCAL_SRC_FILES := neoncompile_saxpy_core.cpp

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI), armeabi-v7a x86))
    LOCAL_CFLAGS := -DHAVE_NEON=1
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -mssse3
endif
    LOCAL_SRC_FILES += neoncompile_saxpy_kernel.c.neon
endif

LOCAL_STATIC_LIBRARIES := cpufeatures
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := volume
LOCAL_SRC_FILES := neoncompile_volume_core.cpp

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI), armeabi-v7a x86))
    LOCAL_CFLAGS := -DHAVE_NEON=1
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -mssse3
endif
    LOCAL_SRC_FILES += neoncompile_volume_kernel.c.neon
endif

LOCAL_STATIC_LIBRARIES := cpufeatures
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cpufeatures)
