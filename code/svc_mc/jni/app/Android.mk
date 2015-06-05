LOCAL_PATH := $(call my-dir)
#######################################
# JNI in this
include $(CLEAR_VARS)

LOCAL_MODULE := dashFramework

# Libdash Framwork
# ############################
FrameWork_MPD_SRC_FILES 		:= $(notdir $(wildcard $(LOCAL_PATH)/libdashframework/MPD/*.cpp))
FrameWork_Portable_SRC_FILES 	:= $(notdir $(wildcard $(LOCAL_PATH)/libdashframework/Portable/*.cpp))
FrameWork_Input_SRC_FILES		:= $(notdir $(wildcard $(LOCAL_PATH)/libdashframework/Input/*.cpp))
FrameWork_Buffer_SRC_FILES		:= $(notdir $(wildcard $(LOCAL_PATH)/libdashframework/Buffer/*.cpp))

UI_SRC_FILES 					:= $(notdir $(wildcard $(LOCAL_PATH)/UI/*.cpp))
MANAGERS_SRC_FILES 				:= $(notdir $(wildcard $(LOCAL_PATH)/Managers/*.cpp))

# Include Framework cpp files
# ############################
LOCAL_SRC_FILES := \
	$(addprefix libdashframework/MPD/, $(FrameWork_MPD_SRC_FILES)) \
	$(addprefix libdashframework/Portable/, $(FrameWork_Portable_SRC_FILES)) \
	$(addprefix libdashframework/Input/, $(FrameWork_Input_SRC_FILES)) \
	$(addprefix libdashframework/Buffer/, $(FrameWork_Buffer_SRC_FILES)) \
	$(addprefix UI/, $(UI_SRC_FILES)) \
	$(addprefix Managers/, $(MANAGERS_SRC_FILES))

# Include header files
# ############################
LOCAL_CFLAGS := \
	-I$(LOCAL_PATH)/../libdash/include \
	-I$(LOCAL_PATH)/../libdash/source/helpers \
	-I$(LOCAL_PATH)/../libdash/source/manager \
	-I$(LOCAL_PATH)/../libdash/source/metrics \
	-I$(LOCAL_PATH)/../libdash/source/mpd \
	-I$(LOCAL_PATH)/../libdash/source/network \
	-I$(LOCAL_PATH)/../libdash/source/protable \
	-I$(LOCAL_PATH)/../libdash/source/xml \
	-I$(LOCAL_PATH)/../curl/include \
	-I$(LOCAL_PATH)/../libxml2/include \
	-I$(LOCAL_PATH)/libdashframework/MPD \
	-I$(LOCAL_PATH)/libdashframework/Portable \
	-I$(LOCAL_PATH)/libdashframework/Input \
	-I$(LOCAL_PATH)/libdashframework/Buffer \
	-I$(LOCAL_PATH)/../extractor/include \
	-I$(LOCAL_PATH)/../extractor/src/Bitstream

# For Test libdash
# ############################
NETWORK_TEST_SRC_FILES 			:= $(notdir $(wildcard $(LOCAL_PATH)/libdash_networkpart_test/*.cpp))
SEGMENT_TEST_SRC_FILES 			:= $(notdir $(wildcard $(LOCAL_PATH)/libdash_segmentpart_test/*.cpp))

LOCAL_CXX_INCLUDES += \
	$(LOCAL_PATH)/libdash_networkpart_test \
	$(LOCAL_PATH)/libdash_segmentpart_test

LOCAL_SRC_FILES += \
	$(addprefix libdash_networkpart_test/, $(NETWORK_TEST_SRC_FILES)) \
	$(addprefix libdash_segmentpart_test/, $(SEGMENT_TEST_SRC_FILES))

LOCAL_CFLAGS += \
	-I$(LOCAL_PATH)/libdash_networkpart_test \
	-I$(LOCAL_PATH)/libdash_segmentpart_test
# ############################

LOCAL_STATIC_LIBRARIES := dash
LOCAL_LDLIBS := -llog -lz
LOCAL_CFLAGS += -O3

include $(BUILD_SHARED_LIBRARY)
# include $(BUILD_STATIC_LIBRARY)
#######################################
