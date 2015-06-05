LOCAL_PATH := $(call my-dir)
include $(LOCAL_PATH)/android-ndk-profiler.mk
#-include android-ndk-profiler.mk
include $(CLEAR_VARS)

SDL_PATH := ../SDL
EXTRACTOR_PATH := ../extractor
LIBDASH_PATH := ../libdash
APP_PATH := ../app

LOCAL_MODULE += svc

APPDIR 		:= $(shell readlink $(LOCAL_PATH)/src)
APP_SUBDIRS := $(patsubst $(LOCAL_PATH)/%, %, $(shell find $(LOCAL_PATH)/$(APPDIR) -path '*/.svn' -prune -o -type d -print))

LOCAL_SRC_FILES += $(foreach F, $(APP_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.cpp))))
LOCAL_SRC_FILES += $(foreach F, $(APP_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.c))))
LOCAL_SRC_FILES += $(SDL_PATH)/src/main/android/SDL_android_main.c
# LOCAL_SRC_FILES += $(foreach F, $(APP_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.s))))
# uses-permission android:name="android.permission.ACCESS_SURFACE_FLINGER"
# ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
# 	LOCAL_CFLAGS += -DHAVE_NEON=1
# 	LOCAL_SRC_FILES += SVC/neonpart.c.neon
# endif

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/$(EXTRACTOR_PATH)/include $(LOCAL_PATH)/$(EXTRACTOR_PATH)/src/Bitstream \
	$(LOCAL_PATH)/$(LIBDASH_PATH)/include \
	$(LOCAL_PATH)/$(LIBDASH_PATH)/source/helpers \
	$(LOCAL_PATH)/$(LIBDASH_PATH)/source/manager \
	$(LOCAL_PATH)/$(LIBDASH_PATH)/source/metrics \
	$(LOCAL_PATH)/$(LIBDASH_PATH)/source/mpd \
	$(LOCAL_PATH)/$(LIBDASH_PATH)/source/network \
	$(LOCAL_PATH)/$(LIBDASH_PATH)/source/protable \
	$(LOCAL_PATH)/$(LIBDASH_PATH)/source/xml \
	$(LOCAL_PATH)/../curl/include \
	$(LOCAL_PATH)/../libxml2/include \
	$(LOCAL_PATH)/$(APP_PATH)/Managers \
	$(LOCAL_PATH)/$(APP_PATH)/UI \
	$(LOCAL_PATH)/$(APP_PATH)/libdashframework/MPD \
	$(LOCAL_PATH)/$(APP_PATH)/libdashframework/Portable \
	$(LOCAL_PATH)/$(APP_PATH)/libdashframework/Input \
	$(LOCAL_PATH)/$(APP_PATH)/libdashframework/Buffer

# LOCAL_CFLAGS += -pg -DPOCKET_PC -DTI_OPTIM
LOCAL_CFLAGS += $(foreach D, $(APP_SUBDIRS), -I$(LOCAL_PATH)/$(D))
LOCAL_CFLAGS += \
	-march=armv7-a \
	-mtune=cortex-a8 \
	-mfpu=neon \
	-fpic \
	-mthumb-interwork \
	-mfloat-abi=softfp \
	-ftree-vectorize \
	-fomit-frame-pointer \
	-ffast-math \
	-funroll-loops \
	-funsafe-math-optimizations \
	-fsingle-precision-constant
LOCAL_CFLAGS += -g 
LOCAL_CFLAGS += -DERROR_DETECTION -DENABLE_PACKET_LOSS_DETECTION
LOCAL_CFLAGS += -O3

LOCAL_SHARED_LIBRARIES := SDL2 dashFramework
LOCAL_STATIC_LIBRARIES := andprof cpufeatures
LOCAL_LDLIBS := -lm -llog -ljnigraphics -lGLESv1_CM -lGLESv2

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cpufeatures)
