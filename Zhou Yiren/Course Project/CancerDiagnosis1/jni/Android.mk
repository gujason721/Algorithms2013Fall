LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_INSTALL_MODULES:=off
OPENCV_LIB_TYPE:=STATIC
include C:/eclipse/download/OpenCV-2.4.6-android-sdk-r2/OpenCV-2.4.6-android-sdk/sdk/native/jni/OpenCV.mk
#include ../../sdk/native/jni/OpenCV.mk
LOCAL_MODULE    := CancerDiagnosis
LOCAL_SRC_FILES := CancerDiagnosis.cpp utils.cpp imgfeatures.cpp sift.cpp
LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)

#LOCAL_PATH := $(call my-dir)
#include $(CLEAR_VARS)
#include C:\eclipse\download\OpenCV-2.4.6-android-sdk-r2\OpenCV-2.4.6-android-sdk\sdk\native\jni\OpenCV.mk
#LOCAL_MODULE    := CancerDiagnosis
#LOCAL_SRC_FILES := CancerDiagnosis.cpp
#include $(BUILD_SHARED_LIBRARY)
