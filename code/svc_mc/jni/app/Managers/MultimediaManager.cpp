#include "MultimediaManager.h"

#include <android/log.h>
#define  LOG_TAG    "MultimediaManager"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,   LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__)

using namespace dashhandler::managers;
using namespace dash;
using namespace dash::mpd;

// Jargo - Define MAX number of layers for SVC video
// -------------------------------------------------
#define SVC_LAYERS 4
// -------------------------------------------------
#define SEGMENTBUFFER_SIZE 2

MultimediaManager::MultimediaManager () :
				mpd 				(NULL),
				period 				(NULL),
				videoAdaptationSet 	(NULL),
				videoRepresentation (NULL),
				videoStream 		(NULL),
				isStarted 			(false),
				currentOffset		(0)
{
	InitializeCriticalSection (&this->monitorMutex);

	this->manager = CreateDashManager();
}
MultimediaManager::~MultimediaManager ()
{
	this->manager->Delete();
	DeleteCriticalSection (&this->monitorMutex);
}

bool MultimediaManager::Init (const std::string &url)
{
	EnterCriticalSection(&this->monitorMutex);

	this->mpd = this->manager->Open((char *)url.c_str());

	if(this->mpd == NULL)
	{
		LeaveCriticalSection(&this->monitorMutex);
		return false;
	}

	LeaveCriticalSection(&this->monitorMutex);
	return true;
}
void MultimediaManager::Start ()
{
	// LOGI("[Start]");

	if (this->isStarted)
		this->Stop();

	EnterCriticalSection(&this->monitorMutex);

	if (this->videoAdaptationSet && this->videoRepresentation)
	{
		// Jargo - start from previous segment number
		// ------------------------------------------
		// this->InitVideoStream(0);
		this->InitVideoStream(this->currentOffset);
		// ------------------------------------------

		this->videoStream->Start();

		LOGD("[Start] - video Stream started");
	}

	this->isStarted = true;

	LeaveCriticalSection(&this->monitorMutex);

	LOGD("[Start] - End of Start");
}
void MultimediaManager::Stop ()
{
	if (!this->isStarted)
		return;

	EnterCriticalSection(&this->monitorMutex);

	this->StopVideoStream();

	this->isStarted = false;

	LeaveCriticalSection(&this->monitorMutex);
}
IMPD* MultimediaManager::GetMPD ()
{
	return this->mpd;
}
bool MultimediaManager::SetVideoQuality (IPeriod* period, IAdaptationSet *adaptationSet, IRepresentation *representation)
{
	EnterCriticalSection(&this->monitorMutex);

	this->period                = period;
	this->videoAdaptationSet    = adaptationSet;
	this->videoRepresentation   = representation;

	if (this->videoStream) {
		this->videoStream->SetRepresentation(this->period, this->videoAdaptationSet, this->videoRepresentation);
	}

	LeaveCriticalSection(&this->monitorMutex);
	return true;
}
void MultimediaManager::SetFrameRate (double framerate)
{
	this->frameRate = framerate;
}
// Jargo - Return Extractor
Bitstream::Manager* MultimediaManager::GetExtractor() {
	return this->videoStream->GetExtractor();
}

void MultimediaManager::InitVideoStream (uint32_t offset)
{
	// Jargo - Set Buffer size to (Max #layers * #Segments)
	// ------------------------------------------------
	// this->videoStream = new MultimediaStream(this->mpd, SEGMENTBUFFER_SIZE);
	this->videoStream = new MultimediaStream(this->mpd, SEGMENTBUFFER_SIZE*SVC_LAYERS);
	// ------------------------------------------------

	this->videoStream->SetRepresentation(this->period, this->videoAdaptationSet, this->videoRepresentation);
}
void MultimediaManager::StopVideoStream ()
{
	if (this->isStarted && this->videoStream)
	{
		this->videoStream->Stop();

		// Jargo - record current segment number
		// -------------------------------------
		this->currentOffset = this->videoStream->GetPosition();
		// -------------------------------------

		delete this->videoStream;

		this->videoStream = NULL;
	}
}