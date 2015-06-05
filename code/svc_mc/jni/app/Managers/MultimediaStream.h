#ifndef QTSAMPLEPLAYER_MANAGERS_MULTIMEDIASTREAM_H_
#define QTSAMPLEPLAYER_MANAGERS_MULTIMEDIASTREAM_H_

#include "IMPD.h"
#include "../libdashframework/Input/DASHManager.h"

// Jargo - Extractor class
#include "BitStream.h"

namespace dashhandler
{
	namespace managers
	{
		class MultimediaStream
		{
			public:
				MultimediaStream 			(dash::mpd::IMPD *mpd, uint32_t segmentBufferSize);
				virtual ~MultimediaStream 	();

				bool        Start                   ();
				void        Stop                    ();
				void        StopDownload            ();
				bool        StartDownload           ();
				void        Clear                   ();
				uint32_t    GetPosition             ();
				void        SetPosition             (uint32_t segmentNumber);
				void        SetPositionInMsec       (uint32_t milliSecs);

				void        SetRepresentation       (dash::mpd::IPeriod *period, dash::mpd::IAdaptationSet *adaptationSet, dash::mpd::IRepresentation *representation);
				void        EnqueueRepresentation   (dash::mpd::IPeriod *period, dash::mpd::IAdaptationSet *adaptationSet, dash::mpd::IRepresentation *representation);

				// Jargo - Return Extractor
				Bitstream::Manager* GetExtractor ();

			private:
				dash::mpd::IMPD                                     *mpd;
				libdash::framework::input::DASHManager              *dashManager;
				uint32_t                                            segmentBufferSize;

				void Init ();
		};
	}
}

#endif /* QTSAMPLEPLAYER_MANAGERS_MULTIMEDIASTREAM_H_ */
