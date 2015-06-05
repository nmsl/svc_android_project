#include "DASHSVCReceiver.h"

#include <android/log.h>

#define  LOG_TAG    "DASHSVCReceiver"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,   LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__)

using namespace libdash::framework::input;
using namespace libdash::framework::buffer;
using namespace libdash::framework::mpd;
using namespace dash::mpd;
using namespace std;

DASHSVCReceiver::DASHSVCReceiver (IMPD *mpd, IDASHReceiverObserver *obs, MediaObjectBuffer *buffer, uint32_t bufferSize) :
		initSegment (NULL),
		buffer (buffer),
		observer (obs),
		mpd (mpd),
		period (NULL),
		adaptationSet (NULL),
		maxRepresentation (NULL),
		adaptationSetStream (NULL),
		// representationStream (NULL),
		segmentNumber (0),
		bufferSize (bufferSize),
		isBuffering (false),
		maxLayer (0),
		currentLayer (1)
{
	this->period                = this->mpd->GetPeriods().at(0);
	this->adaptationSet         = this->period->GetAdaptationSets().at(0);

	// Jargo - record Max Representation
	// ---------------------------------
	// this->representation        = this->adaptationSet->GetRepresentation().at(0);
	this->maxRepresentation        = this->adaptationSet->GetRepresentation().back();
	// ---------------------------------

	this->adaptationSetStream   = new AdaptationSetStream(mpd, period, adaptationSet);

	// Jargo - Store RepresentationsStreams until get Max RepresentationStream
	// -----------------------------------------------------------------------
	// this->representationStream  = adaptationSetStream->GetRepresentationStream(this->representation);
	for (uint32_t i = 0; i < this->adaptationSet->GetRepresentation().size(); i++) {
		IRepresentation *rep = this->adaptationSet->GetRepresentation().at(i);
		this->representationStreams.push_back(adaptationSetStream->GetRepresentationStream(rep));
		if (this->maxRepresentation == rep)
		{
			this->maxLayer = (i+1);
			break;
		}
	}
	// -----------------------------------------------------------------------

	this->segmentOffset         = CalculateSegmentOffset();

	InitializeCriticalSection(&this->monitorMutex);
}
DASHSVCReceiver::~DASHSVCReceiver () {
	delete this->adaptationSetStream;
	DeleteCriticalSection(&this->monitorMutex);
}

bool DASHSVCReceiver::Start () {
	// LOGI("[Start]");

	if(this->isBuffering)
		return false;

	this->isBuffering       = true;
	this->bufferingThread   = CreateThreadPortable (DoBuffering, this);

	if(this->bufferingThread == NULL)
	{
		this->isBuffering = false;
		return false;
	}

	return true;
}
void DASHSVCReceiver::Stop () {
	if(!this->isBuffering)
		return;

	this->isBuffering = false;
	this->buffer->SetEOS(true);

	if(this->bufferingThread != NULL)
	{
		JoinThread(this->bufferingThread);
		DestroyThreadPortable(this->bufferingThread);
	}
}
MediaObject* DASHSVCReceiver::GetNextSegment () {
	ISegment *seg = NULL;
	uint32_t layerID = this->currentLayer;
	uint32_t curmaxLayer = this->GetMaxLayer(this->GetPosition());

	if (layerID > this->representationStreams.size()) {
		LOGE("[GetNextSegment] - Segment Layer ID is incorrect");
		return NULL;
	}

	// if(this->segmentNumber >= this->representationStream->GetSize())
	if(this->segmentNumber >= this->representationStreams.at(layerID-1)->GetSize()) {
		// Keep Downloading untile stop streaming
		this->segmentNumber = 0;
		// return NULL;
	}

	EnterCriticalSection(&this->monitorMutex);
	// seg = this->representationStream->GetMediaSegment(this->segmentNumber + this->segmentOffset);
	seg = this->representationStreams.at(layerID-1)->GetMediaSegment(this->segmentNumber + this->segmentOffset);

	if (seg != NULL)
	{
		// MediaObject *media = new MediaObject(seg, this->representation);
		MediaObject *media = new MediaObject(seg, this->adaptationSet->GetRepresentation().at(layerID-1), this->GetPosition(), layerID);
		// LOGI("[GetNextSegment] - Segment Number = %d with %dth layer", this->GetPosition(), layerID);
		if (layerID == curmaxLayer) {
			this->SetPosition(this->GetPosition()+1);
			this->currentLayer = 1;
		}
		else {
			this->currentLayer++;
		}
		LeaveCriticalSection(&this->monitorMutex);
		return media;
	}

	LeaveCriticalSection(&this->monitorMutex);
	return NULL;
}
MediaObject* DASHSVCReceiver::GetSegment (uint32_t segNum) {
	ISegment *seg = NULL;
	uint32_t layerID = this->currentLayer;

	// if(segNum >= this->representationStream->GetSize())
	if(segNum >= this->representationStreams.at(layerID)->GetSize())
		return NULL;

	// seg = this->representationStream->GetMediaSegment(segNum + segmentOffset);
	seg = this->representationStreams.at(layerID)->GetMediaSegment(segNum + segmentOffset);

	if (seg != NULL)
	{
		// MediaObject *media = new MediaObject(seg, this->representation);
		MediaObject *media = new MediaObject(seg, this->adaptationSet->GetRepresentation().at(layerID), segNum, layerID);
		return media;
	}

	return NULL;
}
uint32_t DASHSVCReceiver::GetMaxLayer (uint32_t segmentNumber) {

	EnterCriticalSection(&this->monitorMutex);
	uint32_t returnlayer = this->maxLayer;
	LeaveCriticalSection(&this->monitorMutex);

	return returnlayer;
}
MediaObject* DASHSVCReceiver::GetInitSegment () {
	ISegment *seg = NULL;

	// TODO: Select first representation's Init Segment
	seg = this->representationStreams.at(0)->GetInitializationSegment();

	if (seg != NULL) {
		// MediaObject *media = new MediaObject(seg, this->representation);
		MediaObject *media = new MediaObject(seg, NULL, 0, 0);
		return media;
	}

	return NULL;
}

// Jargo - Remove Representation
// MediaObject*  DASHSVCReceiver::FindInitSegment (dash::mpd::IRepresentation *representation) {
MediaObject*  DASHSVCReceiver::FindInitSegment () {
	if (!this->InitSegmentExists())
		return NULL;

	// Jargo - Only one init segment
	// -----------------------------
	// return this->initSegments[representation];
	return this->initSegment;
	// -----------------------------
}
uint32_t DASHSVCReceiver::GetPosition () {
	return this->segmentNumber;
}
void DASHSVCReceiver::SetPosition (uint32_t segmentNumber) {
	// some logic here

	this->segmentNumber = segmentNumber;
}
void DASHSVCReceiver::SetPositionInMsecs (uint32_t milliSecs) {
	// some logic here

	this->positionInMsecs = milliSecs;
}
// Jargo
//	- Change single representationStream to multiple representationStreams
//	- Change representation to maxRepresentation
void DASHSVCReceiver::SetRepresentation (IPeriod *period, IAdaptationSet *adaptationSet, IRepresentation *representation) {
	EnterCriticalSection(&this->monitorMutex);

	bool periodChanged = false;

	// if (this->representation == representation) {
	if (this->maxRepresentation == representation) {
		LeaveCriticalSection(&this->monitorMutex);
		return;
	}

	// this->representation = representation;
	this->maxRepresentation = representation;
	// LOGD("[SetRepresentation] - Set Max Representation to latest one");

	// Jargo - we will not change adaptation set
	// -----------------------------------------
	// if (this->adaptationSet != adaptationSet) {
	// 	this->adaptationSet = adaptationSet;

	// 	if (this->period != period) {
	// 		this->period = period;
	// 		periodChanged = true;
	// 	}

	// 	delete this->adaptationSetStream;
	// 	this->adaptationSetStream = NULL;

	// 	this->adaptationSetStream = new AdaptationSetStream(this->mpd, this->period, this->adaptationSet);
	// }
	// -----------------------------------------

	// this->representationStream  = this->adaptationSetStream->GetRepresentationStream(this->representation);
	this->representationStreams.clear();
	for (uint32_t i = 0; i < this->adaptationSet->GetRepresentation().size(); i++) {
		IRepresentation *rep = this->adaptationSet->GetRepresentation().at(i);
		this->representationStreams.push_back(adaptationSetStream->GetRepresentationStream(rep));
		if (this->maxRepresentation == rep) {
			this->maxLayer = (i+1);
			break;
		}
	}

	// this->DownloadInitSegment(this->representation);
	this->DownloadInitSegment();
	// LOGD("[SetRepresentation] - Download Init Segment after changing");

	if (periodChanged) {
		this->segmentNumber = 0;
		this->CalculateSegmentOffset();
	}

	LeaveCriticalSection(&this->monitorMutex);
}
dash::mpd::IRepresentation* DASHSVCReceiver::GetRepresentation () {

	EnterCriticalSection(&this->monitorMutex);
	dash::mpd::IRepresentation* maxRep = this->maxRepresentation;
	LeaveCriticalSection(&this->monitorMutex);

	// return this->representation;
	return maxRep;
}
uint32_t DASHSVCReceiver::CalculateSegmentOffset () {
	if (mpd->GetType() == "static")
		return 0;

	// Jargo - we will not change period
	// ---------------------------------
	// uint32_t firstSegNum = this->representationStream->GetFirstSegmentNumber();
	// uint32_t currSegNum  = this->representationStream->GetCurrentSegmentNumber();
	// uint32_t startSegNum = currSegNum - 2*bufferSize;

	// return (startSegNum > firstSegNum) ? startSegNum : firstSegNum;
	return 0;
	// ---------------------------------
}
void DASHSVCReceiver::NotifySegmentDownloaded () {
	this->observer->OnSegmentDownloaded();
}
// Jargo - Remove Rerpesentation
// void DASHSVCReceiver::DownloadInitSegment (IRepresentation* rep) {
void DASHSVCReceiver::DownloadInitSegment () {
	if (this->InitSegmentExists())
		return;

	MediaObject *initSeg = NULL;
	initSeg = this->GetInitSegment();

	if (initSeg) {
		initSeg->StartDownload();
		initSeg->WaitFinished();
		// Jargo - Only one init segment
		// -----------------------------
		// this->initSegments[rep] = initSeg;
		this->initSegment = initSeg;
		// -----------------------------
	}
}
// Jargo - Remove Rerpesentation
// bool DASHSVCReceiver::InitSegmentExists (IRepresentation* rep) {
bool DASHSVCReceiver::InitSegmentExists () {
	// Jargo - Only one init segment
	// -----------------------------
	// if (this->initSegments.find(rep) != this->initSegments.end())
	// 	return true;
	if (this->initSegment)
		return true;
	// -----------------------------

	return false;
}

// Jargo - download multiple layers for each segment
/* Thread that does the buffering of segments */
void* DASHSVCReceiver::DoBuffering (void *receiver) {
	DASHSVCReceiver *dashReceiver = (DASHSVCReceiver *) receiver;

	dashReceiver->DownloadInitSegment();

	// Jargo - Change to multiple layers
	// ---------------------------------
	// MediaObject *media = dashReceiver->GetNextSegment();
	// while(media != NULL && dashReceiver->isBuffering) {
	// 	media->StartDownload();

	// 	if (!dashReceiver->buffer->PushBack(media)) {
	// 		return NULL;
	// 	}

	// 	media->WaitFinished();

	// 	dashReceiver->NotifySegmentDownloaded();

	// 	media = dashReceiver->GetNextSegment();
	// }

	//---- Older version of SVC receiver ----//
	// while(medias.at(dashReceiver->maxLayer-1) != NULL && dashReceiver->isBuffering) {
	// 	for (uint32_t i = 0; i < medias.size(); i++) {
	// 		medias.at(i)->StartDownload();
	// 		if (!dashReceiver->buffer->PushBack(medias.at(i))) {
	// 			return NULL;
	// 		}

	// 		medias.at(i)->WaitFinished();
	// 	}
	// 	// dashReceiver->NotifySegmentDownloaded();

	// 	medias.clear();
	// 	for (uint32_t i = 1; i <= dashReceiver->maxLayer; i++) {
	// 		medias.push_back(dashReceiver->GetNextSegment(i));
	// 	}
	// }
	//---- End of Older version of SVC receiver ----//

	do {
		if (!dashReceiver->isBuffering) break;

		uint32_t curMaxLayer = dashReceiver->GetMaxLayer(dashReceiver->GetPosition());
		vector<MediaObject*> mediaList;

		for (uint32_t i = 0; i < curMaxLayer; i++) {
			MediaObject *media = dashReceiver->GetNextSegment();
			if (media) {
				media->StartDownload();
				if (!dashReceiver->buffer->PushBack(media)) return NULL;
				mediaList.push_back(media);
			}
		}

		for (uint32_t i = 0; i < curMaxLayer; i++) {
			mediaList[i]->WaitFinished();
		}
	} while (true);
	// ---------------------------------

	dashReceiver->buffer->SetEOS(true);
	return NULL;
}
