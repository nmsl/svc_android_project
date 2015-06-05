/*
 * AbstractSetHelper.cpp
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/
#include <android/log.h>

#define  LOG_TAG    "AdaptationSetHelper"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,   LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__)

#include "AdaptationSetHelper.h"

using namespace libdash::framework::mpd;
using namespace dash::mpd;

std::vector<IAdaptationSet *>   AdaptationSetHelper::GetAudioAdaptationSets (dash::mpd::IPeriod *period)
{
	std::vector<IAdaptationSet *> audioAdaptationSets;
	std::vector<IAdaptationSet *> adaptationSets = period->GetAdaptationSets();

	for (size_t i = 0; i < adaptationSets.size(); i++)
	    if (AdaptationSetHelper::IsAudioAdaptationSet(adaptationSets.at(i)))
	        audioAdaptationSets.push_back(adaptationSets.at(i));

	return audioAdaptationSets;
}
std::vector<IAdaptationSet *>   AdaptationSetHelper::GetVideoAdaptationSets (dash::mpd::IPeriod *period)
{
	std::vector<IAdaptationSet *> videoAdaptationSets;
	std::vector<IAdaptationSet *> adaptationSets = period->GetAdaptationSets();

	for (size_t i = 0; i < adaptationSets.size(); i++)
		if (AdaptationSetHelper::IsVideoAdaptationSet(adaptationSets.at(i)))
			videoAdaptationSets.push_back(adaptationSets.at(i));

	return videoAdaptationSets;
}
bool                            AdaptationSetHelper::IsAudioAdaptationSet   (dash::mpd::IAdaptationSet *adaptationSet)
{
	return IsContainedInMimeType(adaptationSet, "audio");
}
bool                            AdaptationSetHelper::IsVideoAdaptationSet   (dash::mpd::IAdaptationSet *adaptationSet)
{
	// return IsContainedInMimeType(adaptationSet, "video");

	// The string buffer of representation mimetype incorrect
	return IsContainedInMimeType(adaptationSet, "vide");
}
bool                            AdaptationSetHelper::IsContainedInMimeType  (dash::mpd::IAdaptationSet *adaptationSet, std::string value)
{
	// Jargo : Del
	// 		Call GetMimeType() error
	// --------------------------
	// if (adaptationSet->GetMimeType() != "")
	// 	if (adaptationSet->GetMimeType().find(value) != std::string::npos)
	// 		return true;

	for (size_t i = 0; i < adaptationSet->GetRepresentation().size(); i++) {
		if (adaptationSet->GetRepresentation().at(i)->GetMimeType() != "") {
			size_t r = adaptationSet->GetRepresentation().at(i)->GetMimeType().find(value);
			// LOGD("[IsContainedInMimeType]:%d - MimeType=%s, value=%s, IsMatch=%d", __LINE__, adaptationSet->GetRepresentation().at(i)->GetMimeType().c_str(), value.c_str(), r);
			if (r != std::string::npos) return true;
			// if (adaptationSet->GetRepresentation().at(i)->GetMimeType().find(value) != std::string::npos)
			// 	return true;
		}
	}

	return false;
}
