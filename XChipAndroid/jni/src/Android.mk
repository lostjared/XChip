LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := Utix
LOCAL_SRC_FILES := ../../obj/local/${TARGET_ARCH_ABI}/libUtix.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := XChip
LOCAL_SRC_FILES := ../../obj/local/${TARGET_ARCH_ABI}/libXChip.a
LOCAL_STATIC_LIBRARIES := Utix
include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_C_INCLUDES += ../../../XChip/dependencies/Utix/Utix/include/ ../../../XChip/include/ ../../../XChip/dependencies/SDL2/ANDROID/include
LOCAL_CPPFLAGS += -std=c++11 -O3 -Wall -Wextra -fPIC
# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	main.cpp

LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_STATIC_LIBRARIES += Utix XChip
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
