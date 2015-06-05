/*
 * DASHManager.h
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#ifndef LIBDASH_FRAMEWORK_INPUT_DASHMANAGER_H_
#define LIBDASH_FRAMEWORK_INPUT_DASHMANAGER_H_

#include "../Portable/MultiThreading.h"
#include "../Buffer/MediaObjectBuffer.h"
#include "DASHReceiver.h"
#include "IDASHReceiverObserver.h"
#include "IMediaObjectReaderObserver.h"
#include "MediaObjectReaderTest.h"
#include "libdash.h"
#include "IMPD.h"

// Jargo - This is extractor
// -------------------------
#include "Manager.h"
#include "BitStream.h"
#include "Reader.h"
#include "SegmentContainer.h"
// -------------------------

#include "DASHSVCReceiver.h"

namespace libdash
{
	namespace framework
	{
		namespace input
		{
			class DASHManager : public IDASHReceiverObserver, public IMediaObjectReaderObserver
			{
				public:
					DASHManager             (uint32_t maxCapacity, dash::mpd::IMPD *mpd);
					virtual ~DASHManager    ();

					bool        Start                   ();
					void        Stop                    ();
					uint32_t    GetPosition             ();
					void        SetPosition             (uint32_t segmentNumber); // to implement
					void        SetPositionInMsec       (uint32_t millisec);
					void        Clear                   ();
					void        ClearTail               ();
					void        SetRepresentation       (dash::mpd::IPeriod *period, dash::mpd::IAdaptationSet *adaptationSet, dash::mpd::IRepresentation *representation);
					void        EnqueueRepresentation   (dash::mpd::IPeriod *period, dash::mpd::IAdaptationSet *adaptationSet, dash::mpd::IRepresentation *representation);

					/* IDASHReceiverObserver */
					void        OnSegmentDownloaded     ();

					// Jargo - Test Segment Reading
					// ----------------------------
					/* IMediaObjectReaderObserver */
					int ReadSegment ();
					Bitstream::Manager*	GetExtractor ();
					// ----------------------------

				private:
					buffer::MediaObjectBuffer   *buffer;

					// Jargo - SVC downloader
					// ----------------------
					// DASHReceiver                *receiver;
					DASHSVCReceiver                *receiver;
					Bitstream::Manager			   *extractor;
					// ----------------------

					uint32_t                    readSegmentCount;
					bool                        isRunning;

					// Jargo - Test Segment Reading
					// ----------------------------
					MediaObjectReaderTest 		*readerTest;
					// ----------------------------
			};
		}
	}
}

#endif /* LIBDASH_FRAMEWORK_INPUT_DASHMANAGER_H_ */
