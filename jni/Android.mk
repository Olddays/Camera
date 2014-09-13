LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
include /Users/biglandsister/Documents/OpenCV-2.4.6-android-sdk/sdk/native/jni/OpenCV.mk
LOCAL_MODULE    := Camera
LOCAL_SRC_FILES := Factory.cpp
LOCAL_LDLIBS +=  -llog -ldl
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)
