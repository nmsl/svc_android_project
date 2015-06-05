#include "Manager.h"

#include <android/log.h>
#define  LOG_TAG    "Bitstream::Manager"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, 	LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, 		LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, 		LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, 		LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,	 	LOG_TAG, __VA_ARGS__)

using namespace Bitstream;
using namespace std;

Manager::Manager (uint32_t SegmentSize) : 
		type (INIT_SEGMENT),
		reader (NULL),
		bs (NULL),
		rawVideo (NULL),
		initSeg (NULL),
		totalSize (0),
		curSamples (0),
		curLayers (0),
		maxSegmentSize (SegmentSize),
		eos (false)
{
	InitializeConditionVariable (&this->full);
	InitializeConditionVariable (&this->empty);
	InitializeCriticalSection   (&this->monitorMutex);
}
Manager::~Manager () {
	if (this->initSeg) delete this->initSeg;
	if (this->reader) delete this->reader;
	if (this->bs) delete this->bs;
	if (this->rawVideo) delete this->rawVideo;

	this->DeleteAllDataSegs();
	for (uint32_t i = 0; i < segmentInfoList.size(); i++) {
		delete segmentInfoList[i];
		// BitStream *bs = segmentList[i];
		// if (bs) delete bs;
	}
	// if (rawVideo) delete rawVideo;
}

// Public Functions
void Manager::SetBitStream (uint8_t* data, uint32_t size) {
	if (this->bs) delete this->bs;
	this->bs = new BitStream(data, size, READ_MODE);
}
void Manager::SetSegmentType (SegmentType type) {
	this->type = type;
}
void Manager::StartParse () {
	if (!this->bs) {
		this->ShowWarningMessage("BitStream does not exist");
		return;
	}

	if (this->reader) delete reader;
	this->reader = new Reader(this->bs, this->type);

	if (type == INIT_SEGMENT) {
		this->initSeg = reader->SegmentRead();
	}
	else {
		this->dataSegs.push_back(reader->SegmentRead());
	}
}
void Manager::DeleteAllDataSegs () {
	uint32_t size = this->dataSegs.size();
	for (uint32_t i = 0; i < size; i++) {
		SegmentContainer *seg = this->dataSegs.back();
		this->dataSegs.pop_back();
		if (seg) delete seg;
	}
}
// BitStream* Manager::StartExtract () {
void Manager::StartExtract () {
	// Get the number of available segment
	uint32_t maxLayer = this->IsDataSegmentValid();

	if (!maxLayer) {
		this->ShowErrorMessage("No Data Segments are available");
		return;
	}
	LOGD("[StartExtract] - Start Extract for %d Layers", maxLayer);

	// if (this->rawVideo) {
	// 	delete this->rawVideo;
	// 	this->rawVideo = NULL;
	// }
	this->rawVideo = new BitStream();

	if (!this->WriteParameterSets(maxLayer)) {
		this->ShowErrorMessage("ParameterSets Writing failed");
		return;
	}

	if (!this->WriteData(maxLayer)) {
		this->ShowErrorMessage("NAL units Writing failed");
		return;
	}

	this->rawVideo->Done();

	EnterCriticalSection(&this->monitorMutex);

	while(this->segmentInfoList.size() >= this->maxSegmentSize && !this->eos)
		SleepConditionVariableCS(&this->empty, &this->monitorMutex, INFINITE);

	if(this->segmentInfoList.size() >= this->maxSegmentSize)
	{
		LeaveCriticalSection(&this->monitorMutex);
		return;
	}

	SegmentInfo *info = new SegmentInfo(this->rawVideo, this->curSamples ,maxLayer);
	segmentInfoList.push_back(info);

	// this->segmentList.push_back(this->rawVideo);
	// this->segmentSampleList.push_back(this->curSamples);

	this->ExtractedNotify();

	WakeAllConditionVariable(&this->full);
	LeaveCriticalSection(&this->monitorMutex);

	// return rawVideo;
}
SegmentContainer* Manager::GetInitSegment () {
	return this->initSeg;
}
SegmentContainer* Manager::GetDataSegment (uint32_t index) {
	if (this->dataSegs.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->dataSegs.size())
		return this->dataSegs.at(index);

	return this->dataSegs.at(0);
}
// BitStream* Manager::GetDecodableSegment () {
BitStream* Manager::GetDecodableSegment (uint32_t &samples, uint32_t &id) {
	samples = 0;
	BitStream *outdata = NULL;

	EnterCriticalSection(&this->monitorMutex);

	while(this->segmentInfoList.size() == 0 && !this->eos)
		SleepConditionVariableCS(&this->full, &this->monitorMutex, INFINITE);

	if(this->segmentInfoList.size() == 0)
	{
		LeaveCriticalSection(&this->monitorMutex);
		return 0;
	}

	SegmentInfo *info = this->segmentInfoList.front();
	this->segmentInfoList.pop_front();
	outdata = info->segment;
	samples = info->samples;
	id = info->layerID;

	// outdata = this->segmentList.front();
	// this->segmentList.pop_front();

	// samples = this->segmentSampleList.front();
	// this->segmentSampleList.pop_front();

	this->ExtractedNotify();

	WakeAllConditionVariable(&this->empty);
	LeaveCriticalSection(&this->monitorMutex);

	return outdata;
}
void Manager::SetEOS (bool value) {
	EnterCriticalSection(&this->monitorMutex);

	this->eos = value;

	WakeAllConditionVariable(&this->empty);
	WakeAllConditionVariable(&this->full);
	LeaveCriticalSection(&this->monitorMutex);
}

// Private Functions
uint32_t Manager::IsDataSegmentValid () {
	uint32_t r = 0;

	// LOGI("[IsDataSegmentValid] - DataSegs size = %d", this->dataSegs.size());
	for (uint32_t i = 0; i < this->dataSegs.size(); i++) {
		SegmentContainer *container = this->dataSegs.at(i);
		// LOGI("[IsDataSegmentValid] - Segment-%d trackID=%d", i, container->GetTrackID());
		// If the container order is wrong
		if ((i+1) != container->GetTrackID()) return r;
		r++;
	}

	return r;
}

bool Manager::WriteParameterSets (uint32_t maxLayer) {
	AVCConfigurationBox *config = NULL;

	if (!this->initSeg) {
		this->ShowErrorMessage("Init Segment dose not exist");
		return false;
	}
	if (!this->rawVideo) {
		this->ShowErrorMessage("Output BitStream is NULL");
		return false;
	}

	for (uint32_t trackID = 1; trackID <= maxLayer; trackID++) {
		config = this->GetConfiguation(trackID);
		if (!config) return false;

		// So far, only one slot
		AVCConfigSlot *sps = config->GetSequenceParameterSet(0);
		rawVideo->WriteIntToBits(1, 32);
		rawVideo->WriteData(sps->GetData(), sps->GetSize());

		AVCConfigSlot *pps = config->GetPictureParameterSet(0);
		rawVideo->WriteIntToBits(1, 32);
		rawVideo->WriteData(pps->GetData(), pps->GetSize());
	}

	return true;
}
bool Manager::WriteData (uint32_t maxLayer) {
	vector<MediaDataBox*> mdats;

	if (!this->rawVideo) {
		this->ShowErrorMessage("Output BitStream is NULL");
		return false;
	}

	for (uint32_t trackID = 1; trackID <= maxLayer; trackID++) {
		MediaDataBox *mdat = GetMediaData(trackID);
		if (!mdat) return false;
		if (trackID == 1) {
			this->curSamples = this->GetSampleCount(mdat);
		}
		else {
			if (this->curSamples != this->GetSampleCount(mdat)) {
				this->curSamples = 0;
				this->ShowErrorMessage("Data Segments Samples are Different");
				return false;
			}
		}

		mdats.push_back(mdat);
	}

	// sampleCount = 1;

	for (uint32_t sampleNo = 0; sampleNo < this->curSamples; sampleNo++) {
		for (uint32_t trackID = 1; trackID <= maxLayer; trackID++) {
			MediaDataEntry *sample = mdats.at(trackID-1)->GetMediaData(sampleNo);
			BitStream *sampleStream = new BitStream(sample->GetData(), sample->GetSize(), READ_MODE);
			this->WriteSample(sampleStream, trackID);
			delete sampleStream;
		}
	}

	return true;
}
void Manager::WriteSample (BitStream *bs, uint32_t trackID) {
	for (uint32_t i = 1; i < trackID; i++) {
		uint32_t refDataSize = bs->ReadU32();
		bs->SkipBytes(refDataSize);
	}

	while (bs->IsAvailableRead()) {
		uint32_t nalSize = bs->ReadU32();
		// cout << "NAL Size = " << nalSize << endl;
		uint8_t *tmpData = new uint8_t[nalSize];
		bs->ReadData(tmpData, nalSize);
		rawVideo->WriteIntToBits(1, 32);
		rawVideo->WriteData(tmpData, nalSize);
		delete[] tmpData;
	}
}

void Manager::ShowErrorMessage (std::string msg) {
	LOGE("[Error] - %s", msg.c_str());
	// cout << "[Error] - " << msg << endl;
}
void Manager::ShowWarningMessage (std::string msg) {
	LOGE("[Warning] - %s", msg.c_str());
	// cout << "[Warning] - " << msg << endl;
}

AVCConfigurationBox* Manager::GetConfiguation (uint32_t trackID) {
	MovieBox *moov = NULL;
	TrackBox *traf = NULL;
	MediaBox *mdia = NULL;
	MediaInformationBox *minf = NULL;
	SampleTableBox *stbl = NULL;
	SampleDescriptionBox *stsd = NULL;
	MPEGVisualSampleEntryBox *avc1_svc1 = NULL;
	AVCConfigurationBox *avcc_svcc = NULL;

	moov = this->initSeg->GetMovieBox();
	if (!moov) {
		this->ShowErrorMessage("Movie Fragment Box is NULL");
		return NULL;
	}

	traf = moov->GetTrackBox(trackID-1);
	if (!traf) {
		this->ShowErrorMessage("Track Fragment Box is NULL");
		return NULL;
	}

	mdia = traf->GetMediaBox();
	if (!mdia) {
		this->ShowErrorMessage("Media Box is NULL");
		return NULL;
	}

	minf = mdia->GetMediaInformationBox();
	if (!minf) {
		this->ShowErrorMessage("Media Information Box is NULL");
		return NULL;
	}

	stbl = minf->GetSampleTableBox();
	if (!stbl) {
		this->ShowErrorMessage("Sample Table Box is NULL");
		return NULL;
	}

	stsd = stbl->GetSampleDescriptionBox();
	if (!stsd) {
		this->ShowErrorMessage("Sample Description Box is NULL");
		return NULL;
	}

	// Assume only one MPEG Visual Sample
	avc1_svc1 = stsd->GetEntry(0);
	if (!avc1_svc1) {
		this->ShowErrorMessage("MPEG Visual Sample Entry is NULL");
		return NULL;
	}

	avcc_svcc = avc1_svc1->GetAVCConfig();
	if (avcc_svcc) return avcc_svcc;
	avcc_svcc = avc1_svc1->GetSVCConfig();
	if (avcc_svcc) return avcc_svcc;
	this->ShowErrorMessage("Configuration Box is NULL");
	return NULL;
}

MediaDataBox* Manager::GetMediaData (uint32_t trackID) {
	SegmentContainer *seg = this->dataSegs.at(trackID-1);
	if (!seg) {
		this->ShowErrorMessage("Data Segment Container is NULL");
		return NULL;
	}

	MediaDataBox *mdat = seg->GetMediaDataBox();
	if (!mdat) {
		this->ShowErrorMessage("Media Data Box is NULL");
		return NULL;
	}

	return mdat;
}

uint32_t Manager::GetSampleCount (MediaDataBox *mdat) {
	uint32_t sampleNumber = 0;

	TrackFragmentRunBox *trun = mdat->GetRefTRUN();
	if (!trun) {
		this->ShowErrorMessage("Track Fragment Run Box is NULL");
	}
	else {
		sampleNumber = trun->GetSampleCount();
	}

	return sampleNumber;
}

void Manager::ExtractedNotify () {
	LOGI("[ExtractedNotify] - %d/%d Decodable Segments", segmentInfoList.size(), this->maxSegmentSize);
}