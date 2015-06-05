/*
 * MediaObjectDecoder.cpp
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "MediaObjectReaderTest.h"

#include <android/log.h>

#define  LOG_TAG    "MediaObjectReaderTest"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,   LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__)

using namespace libdash::framework::input;
using namespace dash::mpd;

MediaObjectReaderTest::MediaObjectReaderTest  (IMediaObjectReaderObserver *observer) :
					observer 			(observer),
					threadHandle        (NULL)
{
}
MediaObjectReaderTest::~MediaObjectReaderTest()
{
}

bool    MediaObjectReaderTest::Start                   ()
{
	this->run = true;

	return true;
}
void    MediaObjectReaderTest::Stop                    ()
{
	if (!this->run)
		return;

	this->run = false;

	if (this->threadHandle != NULL)
	{
		JoinThread(this->threadHandle);
		DestroyThreadPortable(this->threadHandle);
	}
}
int     MediaObjectReaderTest::Read 					(uint8_t *buf, int buf_size)
{
	int ret = 0;
	// if (!this->decoderInitialized && this->initSegment)
	// {
	// 	ret = this->initSegment->Peek(buf, buf_size, this->initSegmentOffset);
	// 	this->initSegmentOffset += (size_t) ret;
	// }

	// if (ret == 0) {
	// 	ret = this->mediaSegment->Read(buf, buf_size);
	// }

	return ret;
}
void 	MediaObjectReaderTest::StartRead 				()
{
	LOGI("[StartRead]");

	this->threadHandle = CreateThreadPortable (ReadTest, this);

	if(this->threadHandle == NULL)
		return;
}

void*   MediaObjectReaderTest::ReadTest 				(void *data)
{
	MediaObjectReaderTest *readerTest = (MediaObjectReaderTest *) data;

	while (readerTest->run)
	{
		readerTest->ReadNotify();
	}

	free(data);
	return NULL;
}
void 	MediaObjectReaderTest::ReadNotify 				()
{
	this->observer->ReadSegment();
}