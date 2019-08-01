LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_ABI := armeabi-v7a x86 arm64-v8a

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include-all

include $(BUILD_EXECUTABLE)
