LOCAL_PATH := $(call my-dir)

# include XChip lib, which calls and builds with Utix lib
include ${LOCAL_PATH}/../../../XChip/XChipJNI/jni/Android.mk



include $(CLEAR_VARS)

LOCAL_MODULE := main
SDL_PATH := ../SDL
LOCAL_CPPFLAGS := -std=c++11 -pedantic -Wall -Wextra -fPIC -fno-exceptions

ifdef DEBUG
LOCAL_CPPFLAGS += -D_DEBUG -O0 -g -fno-omit-frame-pointer 
else
LOCAL_CPPFLAGS += -DNDEBUG -O3 -fomit-frame-pointer 
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include

LOCAL_C_INCLUDES += ${LOCAL_PATH}/../../../XChip/dependencies/Utix/Utix/include/      \
                    ${LOCAL_PATH}/../../../XChip/include/                             \
                    ${LOCAL_PATH}/../../../XChip/dependencies/SDL2/ANDROID/include

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
                   main.cpp

LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_STATIC_LIBRARIES += Utix XChip
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
