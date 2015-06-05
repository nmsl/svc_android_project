LOCAL_PATH := $(call my-dir)

# We need to build this for both the device (as a shared library)
# and the host (as a static library for tools to use).

common_SRC_FILES := \
	buf.c \
	c14n.c \
	catalog.c \
	chvalid.c \
	debugXML.c \
	dict.c \
	DOCBparser.c \
	encoding.c \
	entities.c \
	error.c \
	globals.c \
	hash.c \
	HTMLparser.c \
	HTMLtree.c \
	legacy.c \
	list.c \
	nanoftp.c \
	nanohttp.c \
	parser.c \
	parserInternals.c \
	pattern.c \
	relaxng.c \
	SAX.c \
	SAX2.c \
	schematron.c \
	threads.c \
	tree.c \
	uri.c \
	valid.c \
	xinclude.c \
	xlink.c \
	xmlIO.c \
	xmlmemory.c \
	xmlmodule.c \
	xmlreader.c \
	xmlregexp.c \
	xmlsave.c \
	xmlschemas.c \
	xmlschemastypes.c \
	xmlstring.c \
	xmlunicode.c \
	xmlwriter.c \
	xpath.c \
	xpointer.c


common_C_INCLUDES += \
	$(LOCAL_PATH)/include


# For the device
# =====================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_C_INCLUDES += \
	$(common_C_INCLUDES)
	# external/icu4c/common
LOCAL_SHARED_LIBRARIES += $(common_SHARED_LIBRARIES)
LOCAL_CFLAGS += -fvisibility=hidden -I$(LOCAL_C_INCLUDES) -O3

LOCAL_MODULE:= xml2

# LOCAL_LDLIBS += -lz

include $(BUILD_STATIC_LIBRARY)
# include $(BUILD_SHARED_LIBRARY)


# For the host
# ========================================================
# include $(CLEAR_VARS)
# LOCAL_SRC_FILES := $(common_SRC_FILES)
# LOCAL_C_INCLUDES += $(common_C_INCLUDES) external/icu4c/common
# LOCAL_SHARED_LIBRARIES += $(common_SHARED_LIBRARIES)
# LOCAL_MODULE:= libxml2
# include $(BUILD_HOST_STATIC_LIBRARY)


# For prebuild static library
# ========================================================
# include $(CLEAR_VARS)

# LOCAL_MODULE := libxml2
# LOCAL_SRC_FILES := $(LOCAL_PATH)/../../obj/local/$(TARGET_ARCH_ABI)/libxml2.a

# include $(PREBUILT_STATIC_LIBRARY)