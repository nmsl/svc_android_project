#ifndef H_MY_MONITOR_H
#define H_MY_MONITOR_H

#include <sys/time.h>
#include <cstdio>
#include "MultiThreadDefined.h"
#include "SDL.h"

class MyMonitor {
	public:
		// MyMonitor ();
		MyMonitor (uint32_t threadNo);
		~MyMonitor ();

		// For SDL display
		void SetFrames (uint32_t frames);
		void SetGOPStartTime ();
		void SetTotalStartTime ();
		void SetRenderingTime (uint32_t time);
		float GetAVGRenderingTime ();

		float GetGOPFPS ();
		float GetTotalFPS ();
		uint32_t GetFrames ();


		// For DASH streaming
		// void AddDataBytes (uint32_t bytes);
		// void AddDownloadGOP (uint32_t frames, uint32_t layerID);
        void AddDownloadData (uint32_t frames, uint32_t layerID, uint32_t bytes);
		void SetNetworkStartTime ();
		float GetThroughputNoResetInMbit ();
		float GetThroughputInMbit ();
		uint32_t GetDownloadedGOPs ();


		// For measurement of decoding delay
		void SetDecodingDelay (uint32_t delay, uint32_t ID);
		// void SetDecodeStartTime ();
		// void SetDecodeEndTime ();
		uint32_t GetDecodingDelay ();


		// For measurement of transfer delay
		void SetTransferEndTime ();
		uint32_t GetTransferDelay ();


		// For spatial switching
		void SetSID (uint32_t sid);
		void SetSIDbyIndex (uint32_t index);
		void HigherSID ();
		void LowerSID ();
		uint32_t GetSID ();
		uint32_t GetSIDIndex ();


		// For temporal switching
		void SetTID (uint32_t tid);
		void HigherTID ();
		void LowerTID ();
		uint32_t GetTID ();


		uint32_t GetSwitchableGOPNo ();

		void SetOutputFilePath (char *path);
		void WriteToOutputFile (int curGOP, int frames);

	private:
		// For SDL
        float GOPFPS;
        float TotalFPS;
        uint32_t DownloadedGOPs;
        uint32_t GOPframes;
        uint32_t GOPTime;
        uint32_t TotalFrames;
        uint32_t totalRT;
        uint32_t totalTime;

		// For DASH
        uint32_t DataBytes;
        uint32_t tpTime;

		// For measurement delay
		// uint32_t decodeTime;
        float tmpFPS;
        float maxFPS;
        uint32_t record_count;
        uint32_t sample_count;
        uint32_t sample_rate;
        uint32_t threads;
        uint32_t totoal_sample;
        uint32_t transferDelay;
        uint32_t transferTime;
        uint32_t* decodeDelay;

		// For switching
        uint32_t curSID;
        uint32_t curTID;
        uint32_t sidIndex;
        uint32_t SIDs[4];
        uint32_t switchGOP;
		CRITICAL_SECTION switch_mutex;

		// For recording
		FILE *output_file;

		float ComputeFPS(uint32_t frames, float duration);
		long long GetTimeStamp ();
		void PrintFPS ();
};

#endif /* H_MY_MONITOR_H */
