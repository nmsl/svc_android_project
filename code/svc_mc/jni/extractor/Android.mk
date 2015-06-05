LOCAL_PATH := $(call my-dir)

#######################################
include $(CLEAR_VARS)

LOCAL_MODULE := extractor

BS_SRC_FILES 	:= $(notdir $(wildcard $(LOCAL_PATH)/src/Bitstream/*.cpp))
BOX_SRC_FILES 	:= $(notdir $(wildcard $(LOCAL_PATH)/src/Box/*.cpp))

LOCAL_SRC_FILES := \
	$(addprefix src/Bitstream/, $(BS_SRC_FILES)) \
	$(addprefix src/Box/, $(BOX_SRC_FILES))

LOCAL_C_INCLUES := $(LOCAL_PATH)/include $(LOCAL_PATH)/../app/libdashframework/Portable

LOCAL_CFLAGS := -I$(LOCAL_PATH)/include -I$(LOCAL_PATH)/../app/libdashframework/Portable -O3

include $(BUILD_STATIC_LIBRARY)
#######################################