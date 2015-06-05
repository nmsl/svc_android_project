#ifndef MY_MULTIMEDIAMANAGER_H
#define MY_MULTIMEDIAMANAGER_H

#include "../libdashframework/Portable/MultiThreading.h"
#include "MultimediaStream.h"
#include "libdash.h"

// Jargo - Extractor class
#include "Manager.h"

namespace dashhandler
{
	namespace managers
	{
		class MultimediaManager
		{
			public:
				MultimediaManager 	();
				~MultimediaManager 	();

				bool Init 					(const std::string &url);
				void Start 					();
				void Stop 					();
				dash::mpd::IMPD* GetMPD 	();

				bool SetVideoQuality 		(dash::mpd::IPeriod* period, dash::mpd::IAdaptationSet *adaptationSet, dash::mpd::IRepresentation *representation);

				void SetFrameRate 			(double frameRate);

				// Jargo - Return Extractor
				Bitstream::Manager* GetExtractor();

			private:
				dash::IDASHManager 			*manager;
				dash::mpd::IMPD				*mpd;
				dash::mpd::IPeriod 			*period;
                dash::mpd::IAdaptationSet 	*videoAdaptationSet;
                dash::mpd::IRepresentation 	*videoRepresentation;
                bool 						isStarted;
                double 						frameRate;
                MultimediaStream 			*videoStream;
                // Jargo
                uint32_t 						currentOffset;

                CRITICAL_SECTION 			monitorMutex;

                void InitVideoStream (uint32_t offset);
                void StopVideoStream ();
		};
	}
}

#endif /* MY_MULTIMEDIAMANAGER_H */