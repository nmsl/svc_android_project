#ifndef LIBDASH_FRAMEWORK_INPUT_DASH_SVC_RECEIVER_H_
#define LIBDASH_FRAMEWORK_INPUT_DASH_SVC_RECEIVER_H_

#include <map>

#include "libdash.h"
#include "IMPD.h"

#include "IDASHReceiverObserver.h"
#include "../Buffer/MediaObjectBuffer.h"
#include "../MPD/AdaptationSetStream.h"
#include "../MPD/IRepresentationStream.h"
#include "../Portable/MultiThreading.h"

namespace libdash
{
	namespace framework
	{
		namespace input
		{
			class DASHSVCReceiver
			{
				public:
					DASHSVCReceiver (dash::mpd::IMPD *mpd, IDASHReceiverObserver *obs, buffer::MediaObjectBuffer *buffer, uint32_t bufferSize);
					virtual ~DASHSVCReceiver ();

					bool Start ();
					void Stop ();
					CRITICAL_SECTION monitorMutex;

					input::MediaObject* GetNextSegment ();
					input::MediaObject* GetSegment (uint32_t segmentNumber);
					uint32_t GetMaxLayer (uint32_t segmentNumber);

					input::MediaObject* GetInitSegment ();

					// Jargo - Remove Rerpesentation
					// -----------------------------
					// input::MediaObject* FindInitSegment (dash::mpd::IRepresentation *representation);
					input::MediaObject* FindInitSegment ();
					// -----------------------------

					uint32_t GetPosition ();
					void SetPosition (uint32_t segmentNumber);
					void SetPositionInMsecs (uint32_t milliSecs);
					dash::mpd::IRepresentation* GetRepresentation ();
					void SetRepresentation (dash::mpd::IPeriod *period, dash::mpd::IAdaptationSet *adaptationSet, dash::mpd::IRepresentation *representation);

				private:
					uint32_t CalculateSegmentOffset ();
					void NotifySegmentDownloaded ();

					// Jargo - Remove Rerpesentation
					// -----------------------------
					// void DownloadInitSegment (dash::mpd::IRepresentation* rep);
					// bool InitSegmentExists (dash::mpd::IRepresentation* rep);
					void DownloadInitSegment ();
					bool InitSegmentExists ();
					// -----------------------------

					static void* DoBuffering (void *receiver);

					// Jargo - only one init segment
					// -----------------------------
					// std::map<dash::mpd::IRepresentation*, MediaObject*> initSegments;
					MediaObject* initSegment;
					// -----------------------------

					buffer::MediaObjectBuffer *buffer;
					IDASHReceiverObserver *observer;
					dash::mpd::IMPD *mpd;
					dash::mpd::IPeriod *period;
					dash::mpd::IAdaptationSet *adaptationSet;
					dash::mpd::IRepresentation *maxRepresentation;
					mpd::AdaptationSetStream *adaptationSetStream;

					// Jargo - store multiple representation stream
					// --------------------------------------------
					// mpd::IRepresentationStream *representationStream;
					std::vector<mpd::IRepresentationStream*> representationStreams;
					// --------------------------------------------

					uint32_t segmentNumber;
					uint32_t positionInMsecs;
					uint32_t segmentOffset;
					uint32_t bufferSize;
					uint32_t maxLayer;
					uint32_t currentLayer;
					std::map<uint32_t, uint32_t> switchQ;

					THREAD_HANDLE bufferingThread;
					bool isBuffering;
			};
		}
	}
}

#endif /* LIBDASH_FRAMEWORK_INPUT_DASH_SVC_RECEIVER_H_ */
