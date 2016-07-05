LOCAL_PATH := $(call my-dir)
include $(${LOCAL_PATH}/../../dependencies/Utix/UtixJNI/jni/Android.mk)

include $(CLEAR_VARS)

LOCAL_MODULE := XChip

XCHIP_SRC_DIR :=../../src/
XCHIP_INCLUDE_DIR :=../../include/
UTIX_INCLUDE_DIR :=../../dependencies/Utix/Utix/include/
SDL2_INCLUDE_DIR :=../../dependencies/SDL2/ANDROID/include/


LOCAL_C_INCLUDES += ${XCHIP_INCLUDE_DIR} \
                    ${UTIX_INCLUDE_DIR}  \
                    ${SDL2_INCLUDE_DIR}  \
                    ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.9/include
                    

LOCAL_CPP_INCLUDES += ${LOCAL_C_INCLUDES} 


LOCAL_SRC_FILES := $(wildcard ${XCHIP_SRC_DIR}Core/*.cpp)               \
                   $(wildcard ${XCHIP_SRC_DIR}Plugins/SDLPlugins/*.cpp)

LOCAL_CPPFLAGS += -std=c++11 -Wall -Wextra -O3 -fPIC -static
LOCAL_STATIC_LIBRARIES += Utix log

include $(BUILD_STATIC_LIBRARY)



