/*
 * DASHManager.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "DASHManager.h"

#include <android/log.h>
#define  LOG_TAG    "DASHManager"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,   LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__)

#include "stdio.h"
#include <fstream>

using namespace libdash::framework::input;
using namespace libdash::framework::buffer;

using namespace dash;
using namespace dash::network;
using namespace dash::mpd;
using namespace Bitstream;

DASHManager::DASHManager        (uint32_t maxCapacity, IMPD* mpd) :
			 readSegmentCount   (0),
			 receiver           (NULL),
			 isRunning          (false),
			 readerTest 		(NULL),
			 extractor 			(NULL)
{
	this->buffer  		= new MediaObjectBuffer(maxCapacity);

	// Jargo - SVC Downloader
	// ----------------------
	// this->receiver  	= new DASHReceiver(mpd, this, this->buffer, maxCapacity);
	this->receiver  	= new DASHSVCReceiver(mpd, this, this->buffer, maxCapacity);
	this->extractor 	= new Manager(1);
	// ----------------------

	// Jargo - Test Segment Reading
	// ----------------------------
	this->readerTest 	= new MediaObjectReaderTest(this);
	// ----------------------------
}
DASHManager::~DASHManager       ()
{
	this->Stop();
	delete this->receiver;
	delete this->buffer;
	delete this->extractor;

	this->receiver = NULL;
	this->buffer   = NULL;
}

bool        DASHManager::Start                  ()
{
	// LOGI("[Start]");

	if (!this->receiver->Start())
		return false;

	// Jargo - test segment downloader
	// -------------------------------
	if (!this->readerTest->Start())
		return false;
	this->readerTest->StartRead();
	// -------------------------------

	this->isRunning = true;

	return true;
}
void        DASHManager::Stop                   ()
{
	if (!this->isRunning)
		return;

	this->isRunning = false;

	this->receiver->Stop();

	// Jargo - test segment downloader
	// -------------------------------
	this->readerTest->Stop();
	// -------------------------------

	this->buffer->Clear();
}
uint32_t    DASHManager::GetPosition            ()
{
	return this->receiver->GetPosition();
}
void        DASHManager::SetPosition            (uint32_t segmentNumber)
{
	this->receiver->SetPosition(segmentNumber);
}
void        DASHManager::SetPositionInMsec      (uint32_t milliSecs)
{
	this->receiver->SetPositionInMsecs(milliSecs);
}
void        DASHManager::Clear                  ()
{
	this->buffer->Clear();
}
void        DASHManager::ClearTail              ()
{
	this->buffer->ClearTail();
}
void        DASHManager::SetRepresentation      (IPeriod *period, IAdaptationSet *adaptationSet, IRepresentation *representation)
{
	this->receiver->SetRepresentation(period, adaptationSet, representation);
	//this->buffer->ClearTail();
}
void        DASHManager::EnqueueRepresentation  (IPeriod *period, IAdaptationSet *adaptationSet, IRepresentation *representation)
{
	this->receiver->SetRepresentation(period, adaptationSet, representation);
}
void        DASHManager::OnSegmentDownloaded    ()
{
	this->readSegmentCount++;
	// LOGI("[OnSegmentDownloaded] - This stream downloads %d Segment(s)", this->readSegmentCount);
	// notify observers
}

// Call By MediaObjectReaderTest class
int 		DASHManager::ReadSegment 			()
{
	// LOGD("[ReadSegment]");

	int readSize = 0;
	uint64_t buf_size = 0;
	uint8_t *buf = NULL;
	MediaObject *mediaObject = NULL;

	mediaObject = this->buffer->GetFront();
	if (!mediaObject) {
		LOGW("[ReadSegment] - There is no MediaObject in Buffer");
		return -1;
	}

	if (!this->extractor->GetInitSegment()) {
		MediaObject *initObject = this->receiver->FindInitSegment();
		initObject->WaitFinished();

		// Copy Data Into new buffer
		buf_size = initObject->GetLength();
		buf = new uint8_t[buf_size];
		readSize = initObject->Read(buf, buf_size);

		// Give the init segment data for extracting
		LOGI("[ReadSegment] - This stream reads init chunk, size is %d", readSize);
		this->extractor->SetBitStream(buf, readSize);
		this->extractor->SetSegmentType(INIT_SEGMENT); // Set segment type as INIT Segment
		this->extractor->StartParse();

		delete[] buf;
	}

	mediaObject->WaitFinished();
	buf_size = mediaObject->GetLength();
	buf = new uint8_t[buf_size];
	readSize = mediaObject->Read(buf, buf_size);
	// LOGI("[ReadSegment] - This stream reads %dth media chunk with %dth layer, size is %d", mediaObject->GetSegmentNo(), mediaObject->GetLayerID(), readSize);

	// If Extractor has other Segment Data, extract first
	if (mediaObject->GetLayerID() == 1 && this->extractor->GetDataSegment(0)) {
		LOGD("[ReadSegment] - Start Extract %dth Media Segment", this->readSegmentCount);
		this->extractor->StartExtract();

		// Clean All Segment Data
		this->extractor->DeleteAllDataSegs();

		// BitStream *bs = this->extractor->GetDecodableSegment();
		// // Extracted Data has error here, return
		// if (!bs) {
		// 	LOGE("[ReadSegment] - BitStream is null");
		// 	return -1;
		// }
		// LOGD("[ReadSegment] - Decodable raw SVC 264 segment-%d, size is %d", this->readSegmentCount, bs->GetSize());

		// // If no errer, output to file
		// std::stringstream ss;
		// ss << "/sdcard/video/sport" << this->readSegmentCount << "_0.264";
		// std::ofstream outFile (ss.str().c_str(), std::ios::out | std::ios::binary);
		// if (!outFile) {
		// 	LOGE("Output File open Failed");
		// }
		// else {
		// 	outFile.write((char*)bs->GetData(0), bs->GetSize());
		// 	outFile.close();
		// }

		this->OnSegmentDownloaded();
	}

	// Sotre the data into extractor
	// LOGI("[ReadSegment] - Push %dth media chunk with %dth layer into Extractor", mediaObject->GetSegmentNo(), mediaObject->GetLayerID());
	this->extractor->SetBitStream(buf, readSize);
	this->extractor->SetSegmentType(DATA_SEGMENT); // Set type as DATA Segment
	this->extractor->StartParse();

	delete[] buf;

	return readSize;
}

Manager* DASHManager::GetExtractor () {
	return this->extractor;
}