#ifndef LIBDASH_FRAMEWORK_INPUT_MEDIAOBJECT_H_
#define LIBDASH_FRAMEWORK_INPUT_MEDIAOBJECT_H_

#include "IMPD.h"
#include "IDownloadObserver.h"
#include "IDASHMetrics.h"
#include "../Portable/MultiThreading.h"

namespace libdash
{
	namespace framework
	{
		namespace input
		{
			class MediaObject : public dash::network::IDownloadObserver, public dash::metrics::IDASHMetrics
			{
				public:
					MediaObject             (dash::mpd::ISegment *segment, dash::mpd::IRepresentation *rep);
					// Jargo - Add SegmentNo and LayerID
					MediaObject             (dash::mpd::ISegment *segment, dash::mpd::IRepresentation *rep, uint32_t no, uint32_t id);
					virtual ~MediaObject    ();

					bool                        StartDownload       ();
					void                        AbortDownload       ();
					void                        WaitFinished        ();
					int                         Read                (uint8_t *data, size_t len);
					int                         Peek                (uint8_t *data, size_t len);
					int                         Peek                (uint8_t *data, size_t len, size_t offset);
					dash::mpd::IRepresentation* GetRepresentation   ();

					virtual void    OnDownloadStateChanged  (dash::network::DownloadState state);
					virtual void    OnDownloadRateChanged   (uint64_t bytesDownloaded);
					/*
					 * IDASHMetrics
					 */
					const std::vector<dash::metrics::ITCPConnection *>&     GetTCPConnectionList    () const;
					const std::vector<dash::metrics::IHTTPTransaction *>&   GetHTTPTransactionList  () const;

					// Jargo - Add GetLength Method
					uint64_t 		GetLength 						();
					// Jargo - Add public function for accessing SegmentNo and LayerId
					uint32_t		GetSegmentNo					();
					uint32_t		GetLayerID						();

				private:
					dash::mpd::ISegment             *segment;
					dash::mpd::IRepresentation      *rep;
					dash::network::DownloadState    state;

					// Jargo - Add SegmentNo and LayerID
					uint32_t segmentNo;
					uint32_t layerID;

					mutable CRITICAL_SECTION    stateLock;
					mutable CONDITION_VARIABLE  stateChanged;
			};
		}
	}
}

#endif /* LIBDASH_FRAMEWORK_INPUT_MEDIAOBJECT_H_ */