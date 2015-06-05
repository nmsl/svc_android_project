LOCAL_PATH:= $(call my-dir)

# #######################################
# # Helpers
# include $(CLEAR_VARS)

# LOCAL_MODULE := helpers

# # LOCAL_CXX_INCLUDES := \
# # 	$(MY_INTERFACE_INCLUDES)
# # 	$(LOCAL_PATH)/source/helpers \
# # 	$(LOCAL_PATH)/source/portable

# LOCAL_SRC_FILES	:= \
# 	source/helpers/Time.cpp \
# 	source/helpers/String.cpp \
# 	source/helpers/Path.cpp \
# 	source/helpers/BlockStream.cpp \
# 	source/helpers/SyncedBlockStream.cpp

# # LOCAL_STATIC_LIBRARIES := portable

# LOCAL_CFLAGS := -I$(LOCAL_PATH)/include

# include $(BUILD_STATIC_LIBRARY)
# #######################################


# #######################################
# # Manager
# include $(CLEAR_VARS)

# LOCAL_MODULE := manager

# # LOCAL_CXX_INCLUDES := \
# # 	$(MY_INTERFACE_INCLUDES)
# # 	$(LOCAL_PATH)/source/manager \
# # 	$(LOCAL_PATH)/source/xml \
# # 	$(LOCAL_PATH)/source/helper

# LOCAL_SRC_FILES	:= \
# 	source/manager/DASHManager.cpp \

# # LOCAL_STATIC_LIBRARIES := helpers xml

# LOCAL_CFLAGS := \
# 	-I$(LOCAL_PATH)/include \
# 	-I$(LOCAL_PATH)/../curl/include \
# 	-I$(LOCAL_PATH)/../libxml2/include

# include $(BUILD_STATIC_LIBRARY)
# #######################################


# #######################################
# # Metrics
# include $(CLEAR_VARS)

# LOCAL_MODULE := metrics

# # LOCAL_CXX_INCLUDES := \
# # 	$(LOCAL_PATH)/include

# LOCAL_SRC_FILES := \
# 	source/metrics/HTTPTransaction.cpp \
# 	source/metrics/TCPConnection.cpp \
# 	source/metrics/ThroughputMeasurement.cpp

# LOCAL_CFLAGS := -I$(LOCAL_PATH)/include

# include $(BUILD_STATIC_LIBRARY)
# #######################################


# #######################################
# # MPD
# include $(CLEAR_VARS)

# LOCAL_MODULE := mpd

# # LOCAL_CXX_INCLUDES := \
# # 	$(LOCAL_PATH)/include \
# #	$(LOCAL_PATH)/helpers \
# #	$(LOCAL_PATH)/metrics \
# #	$(LOCAL_PATH)/network

# LOCAL_SRC_FILES	:= \
# 	source/mpd/Descriptor.cpp \
# 	source/mpd/ProgramInformation.cpp \
# 	source/mpd/Timeline.cpp \
# 	source/mpd/Segment.cpp \
# 	source/mpd/SegmentList.cpp \
# 	source/mpd/SegmentBase.cpp \
# 	source/mpd/BaseUrl.cpp \
# 	source/mpd/Metrics.cpp \
# 	source/mpd/MPD.cpp \
# 	source/mpd/SubRepresentation.cpp \
# 	source/mpd/SegmentTimeline.cpp \
# 	source/mpd/Representation.cpp \
# 	source/mpd/ContentComponent.cpp \
# 	source/mpd/AbstractMPDElement.cpp \
# 	source/mpd/MultipleSegmentBase.cpp \
# 	source/mpd/SegmentTemplate.cpp \
# 	source/mpd/Range.cpp \
# 	source/mpd/Period.cpp \
# 	source/mpd/RepresentationBase.cpp \
# 	source/mpd/AdaptationSet.cpp \
# 	source/mpd/URLType.cpp \
# 	source/mpd/Subset.cpp \
# 	source/mpd/SegmentURL.cpp

# LOCAL_CFLAGS := -I$(LOCAL_PATH)/include

# include $(BUILD_STATIC_LIBRARY)
# #######################################


# #######################################
# # network
# include $(CLEAR_VARS)

# LOCAL_MODULE := network

# # LOCAL_CXX_INCLUDES := \
# # 	$(LOCAL_PATH)/include \

# LOCAL_SRC_FILES	:= \
# 	source/network/AbstractChunk.cpp \
# 	source/network/DownloadStateManager.cpp

# LOCAL_CFLAGS := \
# 	-I$(LOCAL_PATH)/include \
# 	-I$(LOCAL_PATH)/../curl/include

# include $(BUILD_STATIC_LIBRARY)
# #######################################


# #######################################
# # portable
# include $(CLEAR_VARS)

# LOCAL_MODULE := portable

# # LOCAL_CXX_INCLUDES := \
# # 	$(LOCAL_PATH)/include \

# LOCAL_SRC_FILES	:= \
# 	source/portable/MultiThreading.cpp

# LOCAL_CFLAGS := -I$(LOCAL_PATH)/include

# include $(BUILD_STATIC_LIBRARY)
# #######################################


# #######################################
# # xml
# include $(CLEAR_VARS)

# LOCAL_MODULE := xml

# # LOCAL_CXX_INCLUDES := \
# # 	$(LOCAL_PATH)/include \

# LOCAL_SRC_FILES	:= \
# 	source/xml/DOMParser.cpp \
# 	source/xml/DOMHelper.cpp \
# 	source/xml/Node.cpp

# LOCAL_CFLAGS := \
# 	-I$(LOCAL_PATH)/include \
# 	-I$(LOCAL_PATH)/../curl/include \
# 	-I$(LOCAL_PATH)/../libxml2/include

# include $(BUILD_STATIC_LIBRARY)
# #######################################

#######################################
# DASH
include $(CLEAR_VARS)

LOCAL_MODULE := dash

Helper_SRC_FILES 	:= $(notdir $(wildcard $(LOCAL_PATH)/source/helpers/*.cpp))
Manager_SRC_FILES 	:= $(notdir $(wildcard $(LOCAL_PATH)/source/manager/*.cpp))
Metrics_SRC_FILES 	:= $(notdir $(wildcard $(LOCAL_PATH)/source/metrics/*.cpp))
Mpd_SRC_FILES 		:= $(notdir $(wildcard $(LOCAL_PATH)/source/mpd/*.cpp))
Network_SRC_FILES 	:= $(notdir $(wildcard $(LOCAL_PATH)/source/network/*.cpp))
Portable_SRC_FILES 	:= $(notdir $(wildcard $(LOCAL_PATH)/source/portable/*.cpp))
Xml_SRC_FILES 		:= $(notdir $(wildcard $(LOCAL_PATH)/source/xml/*.cpp))

LOCAL_SRC_FILES := \
	$(addprefix source/helpers/, $(Helper_SRC_FILES)) \
	$(addprefix source/manager/, $(Manager_SRC_FILES)) \
	$(addprefix source/metrics/, $(Metrics_SRC_FILES)) \
	$(addprefix source/mpd/, $(Mpd_SRC_FILES)) \
	$(addprefix source/network/, $(Network_SRC_FILES)) \
	$(addprefix source/portable/, $(Portable_SRC_FILES)) \
	$(addprefix source/xml/, $(Xml_SRC_FILES)) \
	source/libdash.cpp

# LOCAL_SRC_FILES	:= \
# 	source/helpers/Time.cpp \
# 	source/helpers/String.cpp \
# 	source/helpers/Path.cpp \
# 	source/helpers/BlockStream.cpp \
# 	source/helpers/SyncedBlockStream.cpp \
# 	source/manager/DASHManager.cpp \
# 	source/metrics/HTTPTransaction.cpp \
# 	source/metrics/TCPConnection.cpp \
# 	source/metrics/ThroughputMeasurement.cpp \
# 	source/mpd/Descriptor.cpp \
# 	source/mpd/ProgramInformation.cpp \
# 	source/mpd/Timeline.cpp \
# 	source/mpd/Segment.cpp \
# 	source/mpd/SegmentList.cpp \
# 	source/mpd/SegmentBase.cpp \
# 	source/mpd/BaseUrl.cpp \
# 	source/mpd/Metrics.cpp \
# 	source/mpd/MPD.cpp \
# 	source/mpd/SubRepresentation.cpp \
# 	source/mpd/SegmentTimeline.cpp \
# 	source/mpd/Representation.cpp \
# 	source/mpd/ContentComponent.cpp \
# 	source/mpd/AbstractMPDElement.cpp \
# 	source/mpd/MultipleSegmentBase.cpp \
# 	source/mpd/SegmentTemplate.cpp \
# 	source/mpd/Range.cpp \
# 	source/mpd/Period.cpp \
# 	source/mpd/RepresentationBase.cpp \
# 	source/mpd/AdaptationSet.cpp \
# 	source/mpd/URLType.cpp \
# 	source/mpd/Subset.cpp \
# 	source/mpd/SegmentURL.cpp \
# 	source/network/AbstractChunk.cpp \
# 	source/network/DownloadStateManager.cpp \
# 	source/portable/MultiThreading.cpp \
# 	source/xml/DOMParser.cpp \
# 	source/xml/DOMHelper.cpp \
# 	source/xml/Node.cpp

LOCAL_CFLAGS := \
	-I$(LOCAL_PATH)/include \
	-I$(LOCAL_PATH)/source/helpers \
	-I$(LOCAL_PATH)/source/manager \
	-I$(LOCAL_PATH)/source/metrics \
	-I$(LOCAL_PATH)/source/mpd \
	-I$(LOCAL_PATH)/source/portable \
	-I$(LOCAL_PATH)/source/xml \
	-I$(LOCAL_PATH)/../curl/include \
	-I$(LOCAL_PATH)/../libxml2/include \

# LOCAL_SHARED_LIBRARIES += libxml2 libcurl
LOCAL_STATIC_LIBRARIES := libxml2 libcurl libextractor
LOCAL_CFLAGS += -O3

# LOCAL_LDLIBS := -lz -llog

include $(BUILD_STATIC_LIBRARY)
# include $(BUILD_SHARED_LIBRARY)
#######################################