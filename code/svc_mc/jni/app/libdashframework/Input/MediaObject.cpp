#include "MediaObject.h"

using namespace libdash::framework::input;

using namespace dash::mpd;
using namespace dash::network;
using namespace dash::metrics;

MediaObject::MediaObject    (ISegment *segment, IRepresentation *rep) :
			 segment        (segment),
			 rep            (rep),
			 segmentNo 		(0),
			 layerID 		(0)
{
	InitializeConditionVariable (&this->stateChanged);
	InitializeCriticalSection   (&this->stateLock);
}
// Jargo - Add SegmentNo and LayerID
MediaObject::MediaObject    (ISegment *segment, IRepresentation *rep, uint32_t no, uint32_t id) :
			 segment        (segment),
			 rep            (rep),
			 segmentNo 		(no),
			 layerID 		(id)
{
	InitializeConditionVariable (&this->stateChanged);
	InitializeCriticalSection   (&this->stateLock);
}
MediaObject::~MediaObject   ()
{
	if(this->state == IN_PROGRESS)
	{
		this->segment->AbortDownload();
		this->OnDownloadStateChanged(ABORTED);
	}
	this->segment->DetachDownloadObserver(this);
	this->WaitFinished();

	DeleteConditionVariable (&this->stateChanged);
	DeleteCriticalSection   (&this->stateLock);
}

bool                MediaObject::StartDownload          ()
{
	this->segment->AttachDownloadObserver(this);
	return this->segment->StartDownload();
}
void                MediaObject::AbortDownload          ()
{
	this->segment->AbortDownload();
	this->OnDownloadStateChanged(ABORTED);
}
void                MediaObject::WaitFinished           ()
{
	EnterCriticalSection(&this->stateLock);

	while(this->state != COMPLETED && this->state != ABORTED)
		SleepConditionVariableCS(&this->stateChanged, &this->stateLock, INFINITE);

	LeaveCriticalSection(&this->stateLock);
}
int                 MediaObject::Read                   (uint8_t *data, size_t len)
{
	return this->segment->Read(data, len);
}
int                 MediaObject::Peek                   (uint8_t *data, size_t len)
{
	return this->segment->Peek(data, len);
}
int                 MediaObject::Peek                   (uint8_t *data, size_t len, size_t offset)
{
	return this->segment->Peek(data, len, offset);
}
IRepresentation*    MediaObject::GetRepresentation      ()
{
	return this->rep;
}
void                MediaObject::OnDownloadStateChanged (DownloadState state)
{
	EnterCriticalSection(&this->stateLock);

	this->state = state;

	WakeAllConditionVariable(&this->stateChanged);
	LeaveCriticalSection(&this->stateLock);
}
void                MediaObject::OnDownloadRateChanged  (uint64_t bytesDownloaded)
{
}
const std::vector<ITCPConnection *>&    MediaObject::GetTCPConnectionList   () const
{
	return this->segment->GetTCPConnectionList();
}
const std::vector<IHTTPTransaction *>&  MediaObject::GetHTTPTransactionList () const
{
	return this->segment->GetHTTPTransactionList();
}

// Jargo - Add GetLength Method
uint64_t 			MediaObject::GetLength 				()
{
	return this->segment->GetBlockStreamSize();
}

// Jargo - Add public function for accessing SegmentNo and LayerId
uint32_t			MediaObject::GetSegmentNo				()
{
	return this->segmentNo;
}
uint32_t			MediaObject::GetLayerID					()
{
	return this->layerID;
}