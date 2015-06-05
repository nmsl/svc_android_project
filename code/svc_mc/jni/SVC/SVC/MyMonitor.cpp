#include <android/log.h>
#include <sys/time.h>

#include "MyMonitor.h"

#define  LOG_TAG    "MyMonitor"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, 	 LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, 	 LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, 	 LOG_TAG, __VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, 	 LOG_TAG, __VA_ARGS__)

MyMonitor::MyMonitor (uint32_t threadNo) : 
		// SDL
		GOPFPS 			(0.0),
		TotalFPS 		(0.0),
		DownloadedGOPs 	(0),
		GOPframes 		(0),
		GOPTime 		(0),
		TotalFrames 	(0),
		totalRT 		(0),
		totalTime 		(0),
		// DASH
		DataBytes 		(0),
		tpTime 			(0),
		// Delay
		// decodeTime (0),
		tmpFPS 			(0.0),
		maxFPS 			(0.0),
		record_count	(0),
		sample_count 	(0),
		sample_rate 	(10),
		threads 		(threadNo),
		totoal_sample 	(23),
		transferDelay 	(0),
		transferTime 	(0),
		decodeDelay 	(NULL),
		// Switch
		curSID 		(0),
		curTID 		(0),
		sidIndex 	(0),
		switchGOP 	(-1),
		// Recording
		output_file (NULL)
{
	InitializeCriticalSection(&this->switch_mutex);
	// SIDs = new uint32_t[4];
	SIDs[0] = 8;
	SIDs[1] = 16;
	SIDs[2] = 32;
	SIDs[3] = 48;

	decodeDelay = new uint32_t[threads]();

	// totoal_sample = (9000+(sample_rate*16)-1) / (sample_rate*16);

	// LOGI("[MyMonitor]:%d, sample_count = %d", __LINE__, totoal_sample);
}
MyMonitor::~MyMonitor () {
	// delete[] SIDs;
	delete[] decodeDelay;
}

// Public Functions
// Fir SDL
void MyMonitor::SetFrames (uint32_t frames) {
	uint32_t time_in_ms = 0;

	this->GOPframes = frames;
	this->TotalFrames += this->GOPframes;

	LOGI("[SetFrames] - frames=%d, GOPFrames=%d, TotalFrames=%d", frames, GOPframes, TotalFrames);

	time_in_ms = SDL_GetTicks() - GOPTime;
	this->GOPFPS = this->ComputeFPS(this->GOPframes, time_in_ms/1000.0);

	time_in_ms = SDL_GetTicks() - totalTime;
	this->TotalFPS = this->ComputeFPS(this->TotalFrames, time_in_ms/1000.0);

	this->GOPTime = SDL_GetTicks();
	// this->PrintFPS();
}
void MyMonitor::SetGOPStartTime () {
	this->GOPTime = SDL_GetTicks();
}
void MyMonitor::SetTotalStartTime () {
	this->totalTime = SDL_GetTicks();
}
void MyMonitor::SetRenderingTime (uint32_t time) {
	this->totalRT += time;
}
float MyMonitor::GetAVGRenderingTime () {
	return this->totalRT/(float)(this->TotalFrames);
}
float MyMonitor::GetGOPFPS () {
	return this->GOPFPS;
}
float MyMonitor::GetTotalFPS () {
	return this->TotalFPS;
}
uint32_t MyMonitor::GetFrames () {
	return this->TotalFrames;
}



// For DASH
// void MyMonitor::AddDataBytes (uint32_t bytes) {
// 	this->DataBytes += bytes;
// }
// void MyMonitor::AddDownloadGOP (uint32_t frames, uint32_t layerID) {
// 	if (!frames) LOGE("[AddDownloadFrames] - Get NULL Segment Data with 0 frame");

// 	uint32_t new_sid = SIDs[layerID-1];
// 	if (new_sid > curSID) {
// 		switchGOP = this->DownloadedGOPs + 1;
// 	}
// 	this->DownloadedGOPs += 8;
// }
void MyMonitor::AddDownloadData (uint32_t frames, uint32_t layerID, uint32_t bytes) {
	if (!frames) LOGE("[AddDownloadFrames] - Get NULL Segment Data with 0 frame");
	static uint32_t old_sid;

	uint32_t new_sid = SIDs[layerID-1];
	// uint32_t new_sid = SIDs[layerID-1];
	// LOGI("[AddDownloadFrames] - new_sid = %d, curSID = %d", new_sid, curSID);
	// if (new_sid > curSID) {
	if (new_sid > old_sid) {
		switchGOP = this->DownloadedGOPs + 1;
	}
	this->DownloadedGOPs += 8;

	this->DataBytes += bytes;

	old_sid = new_sid;

	LOGI("[AddDownloadFrames] - new data is layer %d, switch GOP = %d", layerID, switchGOP);
}
void MyMonitor::SetNetworkStartTime () {
	this->tpTime = this->transferTime = SDL_GetTicks();
}
float MyMonitor::GetThroughputNoResetInMbit () {
	float mbits = this->DataBytes*8/1048576.0;
	float diff_in_sec = (SDL_GetTicks() - tpTime) / 1000.0;
	float result = mbits/diff_in_sec;

	return result;
}
float MyMonitor::GetThroughputInMbit () {
	float result = this->GetThroughputNoResetInMbit();
	
	this->tpTime       = SDL_GetTicks();
	this->DataBytes = 0;

	return result;	
}
uint32_t MyMonitor::GetDownloadedGOPs () {
	return this->DownloadedGOPs;
}



// For delay
// void MyMonitor::SetDecodeStartTime () {
// 	this->decodeTime = SDL_GetTicks();
// }
// void MyMonitor::SetDecodeEndTime () {
// 	this->decodeDelay[0] = SDL_GetTicks() - decodeTime;
// }
uint32_t MyMonitor::GetDecodingDelay () {
	uint32_t counter = 0;
	uint32_t total   = 0.0;

	for (uint32_t i = 0; i < this->threads; i++) {
		// LOGI("[GetDecodingDelay]:%d - decode delay[%d]=%d", __LINE__, i, this->decodeDelay[i]);
		if (this->decodeDelay[i]) {
			counter++;
			total += this->decodeDelay[i];
		}
	}

	if (!counter) return 0;
	else return total/counter;
}

void MyMonitor::SetDecodingDelay (uint32_t delay, uint32_t ID) {
	LOGI("[SetDecodingDelay]:%d - delay[%d]=%d", __LINE__, ID, delay);
	this->decodeDelay[ID] += delay;
}
void MyMonitor::SetTransferEndTime () {
	if (this->transferDelay) return;
	this->transferDelay = SDL_GetTicks() - transferTime;
}
uint32_t MyMonitor::GetTransferDelay () {
	return this->transferDelay;
}



// For switching
void MyMonitor::SetSID (uint32_t sid) {
	switch (sid) {
		case 8:
			this->SetSIDbyIndex(0);
			break;
		case 16:
			this->SetSIDbyIndex(1);
			break;
		case 32:
			this->SetSIDbyIndex(2);
			break;
		case 48:
			this->SetSIDbyIndex(3);
			break;
		default:
			this->SetSIDbyIndex(1);
			LOGE("[SetSID] - layer ID setting error, set it as default");
			break;
	}
}
void MyMonitor::SetSIDbyIndex (uint32_t index) {
	this->sidIndex = index;
	this->curSID = SIDs[index];
}
void MyMonitor::HigherSID () {
	EnterCriticalSection(&switch_mutex);
	if (this->sidIndex < 3) {
		this->curSID = SIDs[++sidIndex];
		LOGI("[HigherSID] - Spacial layer Switch Up");
	}
	LeaveCriticalSection(&switch_mutex);
}
void MyMonitor::LowerSID () {
	EnterCriticalSection(&switch_mutex);
	if (this->sidIndex > 1) {
		this->curSID = SIDs[--sidIndex];
		LOGI("[LowerSID] - Spacial layer Switch Down");
	}
	LeaveCriticalSection(&switch_mutex);
}
uint32_t MyMonitor::GetSID () {
	EnterCriticalSection(&switch_mutex);
	uint32_t r = this->curSID;
	LeaveCriticalSection(&switch_mutex);	

	return r;
}
uint32_t MyMonitor::GetSIDIndex () {
	EnterCriticalSection(&switch_mutex);
	uint32_t r = this->sidIndex; 
	LeaveCriticalSection(&switch_mutex);	

	return r;
}

void MyMonitor::SetTID (uint32_t tid) {
	if (tid >= 1 && tid <= 3)
		this->curTID = tid;
}
void MyMonitor::HigherTID () {
	EnterCriticalSection(&switch_mutex);
	if (this->curTID < 3) {
		this->curTID++;
		LOGI("[HigherTID] - Temporal layer Switch Up to %d", this->curTID);
	}
	LeaveCriticalSection(&switch_mutex);
}
void MyMonitor::LowerTID () {
	EnterCriticalSection(&switch_mutex);
	if (this->curTID > 1) {
		this->curTID--;
		LOGI("[LowerTID] - Temporal layer Switch Down to %d", this->curTID);
	}
	LeaveCriticalSection(&switch_mutex);
}
uint32_t MyMonitor::GetTID () {
	EnterCriticalSection(&switch_mutex);
	uint32_t r = this->curTID; 
	LeaveCriticalSection(&switch_mutex);

	return r;
}

uint32_t MyMonitor::GetSwitchableGOPNo () {
	return this->switchGOP;
}

void MyMonitor::SetOutputFilePath (char *path) {
	return;

	if (output_file) {
		fclose (output_file);
	}

	output_file = fopen(path, "a");

	if (!output_file) {
		LOGE("[WriteToOutputFile] - Output File open failed.");
		return;
	}
}
void MyMonitor::WriteToOutputFile (int curGOP, int frames) {
	return;

	static int first = 1;
	static int myCounter = 0;
	if (frames < 5) return;

	record_count++;

	// if (!maxFPS || maxFPS < this->GetGOPFPS()) maxFPS = this->GetGOPFPS();
	tmpFPS += this->GetGOPFPS();
	myCounter++;
	

	if (record_count % this->sample_rate == 0 && this->sample_count < this->totoal_sample) {
		// tmpFPS += (maxFPS - this->tmpFPS)/++myCounter;

		if (first) {
			fprintf(output_file, "(%d) %u=>TrD:%d,DeD:%d\n", this->sample_count, SDL_GetTicks(), this->GetTransferDelay(), this->GetDecodingDelay()*16/this->TotalFrames);	
			first = 0;
		}
		fprintf(output_file, "(%d) %u=>TP:%.2f,FPS:%.2f,AVGRT:%.2f,AVGDT:%0.2f\n", this->sample_count++, SDL_GetTicks(), this->GetThroughputInMbit(), tmpFPS/myCounter, GetAVGRenderingTime()*16, (float)this->GetDecodingDelay()*16/this->TotalFrames);
		// fprintf(output_file, "(%d) %u=>TP:%.2f,FPS:%.2f\n", this->sample_count++, SDL_GetTicks(), this->GetThroughputInMbit(), tmaxFPS);
		fflush(output_file);

		// maxFPS    = 0.0;
		tmpFPS    = 0.0;
		myCounter = 0;
	}
	else if (this->sample_count >= this->totoal_sample) {
		fclose(output_file);
		LOGD("[WriteToOutputFile] - Log message done.");
		exit(0);
	}
}


// Private Functions
float MyMonitor::ComputeFPS(uint32_t frames, float duration) {
	return frames/duration;
}
long long MyMonitor::GetTimeStamp () {
	timeval now;
	gettimeofday (&now, NULL);
	long long mslong = (long long) now.tv_sec * 1000L + now.tv_usec / 1000;
	return mslong;
}
void MyMonitor::PrintFPS () {
	LOGI("[FPS] - Current_FPS=%.2f, Cumulative_FPS=%.2f", this->GOPFPS, this->TotalFPS);
}
