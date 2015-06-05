/*
 * libdash_memoryleak_test.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <android/log.h>

#include "libdash_segmentpart_test.h"
#include "IMPD.h"
#include "INode.h"

#define  LOG_TAG    "SegmentTest"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace dash;
using namespace dash::network;
using namespace std;
using namespace dash::mpd;
using namespace libdashtest;
using namespace libdash::framework::mpd;

SegmentTest::SegmentTest()
{
	LOGI("<Init> - Create SegmentTest Class");

	this->manager = new DASHManager();

	mpd = manager->Open("http://www-itec.aau.at/~cmueller/libdashtest/showcases/big_buck_bunny_480.mpd");
	if (mpd)
		LOGI("<Init> - MPD is parsed");
	else {
		LOGE("<Init> - MPD Parse Error");
		return ;
	}

	std::string mediauri = mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().at(0)->GetSegmentList()->GetSegmentURLs().at(0)->GetMediaURI();
	LOGI("<Init> - Media Uri => %s", mediauri.c_str());
	std::string mediarng = mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().at(0)->GetSegmentList()->GetSegmentURLs().at(0)->GetMediaRange();
	LOGI("<Init> - Media Range => %s", mediarng.c_str());

	this->period = mpd->GetPeriods().at(0);
	this->adaptationSet = this->period->GetAdaptationSets().at(0);
	this->representation = adaptationSet->GetRepresentation().at(0);

	// RepresentationStreamType type = DetermineRepresentationStreamType(representation);
	// LOGI("<Init> - Representation type is %d", type);

	AdaptationSetStream *adaptationSetStream = new AdaptationSetStream(mpd, period, adaptationSet);
	IRepresentationStream *representationStream = adaptationSetStream->GetRepresentationStream(this->representation);
	LOGI("<Init> - Get Representation Stream");

	dash::mpd::ISegment *seg = representationStream->GetInitializationSegment();
	if (!seg)
		LOGE("<Init> - Init Segment Get Error!");
	else
		LOGI("<Init> - Get Init Segment");
	seg->StartDownload();
	LOGI("<Init> - init segment is downloaded");

	// int max_seg = representationStream->GetSize();
	// LOGI("<Init> - Total Segment size is %d", max_seg);

	seg = representationStream->GetMediaSegment(0);
	seg->StartDownload();
	LOGI("<Init> - segment is downloaded");

	// std::vector<dash::mpd::IBaseUrl *> baseurls;
	// baseurls.push_back(mpd->GetBaseUrls().at(0));
	// dash::mpd::ISegment *seg = mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().at(0)->GetSegmentList()->GetSegmentURLs().at(0)->ToMediaSegment(baseurls);
	// LOGI("<Init> - Get Segment");
	// seg->StartDownload();
	// LOGI("<Init> - Segment is downloaded");

	// delete(initseg);	LOGI("<Init> - InitSegment is Deleted");
	// delete(mpd);		LOGI("<Init> - MPD is Deleted");
	// delete(manager);	LOGI("<Init> - Manager is Deleted");
}

SegmentTest::~SegmentTest() {}