#include "MultimediaStream.h"

#include <android/log.h>
#define  LOG_TAG    "MultimediaStream"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,   LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__)

using namespace dashhandler::managers;
using namespace libdash::framework::input;
using namespace dash::mpd;

MultimediaStream::MultimediaStream  (IMPD *mpd, uint32_t bufferSize) :
				  segmentBufferSize (bufferSize),
				  dashManager       (NULL),
				  mpd               (mpd)
{
	this->Init();
}
MultimediaStream::~MultimediaStream ()
{
	this->Stop();
	delete this->dashManager;
}


bool        MultimediaStream::Start                     ()
{
	// LOGI("[Start]");

	if(!this->StartDownload())
		return false;

	return true;
}
void        MultimediaStream::Stop                      ()
{
	this->StopDownload();
}
void        MultimediaStream::StopDownload              ()
{
	this->dashManager->Stop();
}
bool        MultimediaStream::StartDownload             ()
{
	if(!dashManager->Start())
		return false;

	return true;
}
void        MultimediaStream::Clear                     ()
{
	this->dashManager->Clear();
}
uint32_t    MultimediaStream::GetPosition               ()
{
	return this->dashManager->GetPosition();
}
void        MultimediaStream::SetPosition               (uint32_t segmentNumber)
{
	this->dashManager->SetPosition(segmentNumber);
}
void        MultimediaStream::SetPositionInMsec         (uint32_t milliSecs)
{
	this->dashManager->SetPositionInMsec(milliSecs);
}
void        MultimediaStream::SetRepresentation         (IPeriod *period, IAdaptationSet *adaptationSet, IRepresentation *representation)
{
	// LOGD("[SetRepresentation]");
	this->dashManager->SetRepresentation(period, adaptationSet, representation);
}
void        MultimediaStream::EnqueueRepresentation     (IPeriod *period, IAdaptationSet *adaptationSet, IRepresentation *representation)
{
	this->dashManager->EnqueueRepresentation(period, adaptationSet, representation);
}
// Jargo - Return Extractor
Bitstream::Manager* MultimediaStream::GetExtractor () {
	return this->dashManager->GetExtractor();
}

void        MultimediaStream::Init                      ()
{
	this->dashManager   = new DASHManager(this->segmentBufferSize, this->mpd);
}