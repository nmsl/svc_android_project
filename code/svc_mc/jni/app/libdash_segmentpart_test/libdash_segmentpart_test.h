#ifndef SEGMENT_TEST_H
#define SEGMENT_TEST_H

#include "IMPD.h"
#include "INode.h"
#include "DASHManager.h"
#include "IRepresentationStream.h"
#include "IRepresentation.h"
#include "IMPD.h"
#include "IPeriod.h"
#include "IAdaptationSet.h"
#include "RepresentationStreamFactory.h"
#include "BaseUrlResolver.h"
#include "AdaptationSetStream.h"

using namespace libdash::framework::mpd;

namespace libdashtest
{
	class SegmentTest
	{
		public:
			SegmentTest();
			~SegmentTest();

		private:
			dash::DASHManager *manager;
    		dash::mpd::IMPD *mpd;

    		dash::mpd::IPeriod *period;
    		dash::mpd::IAdaptationSet *adaptationSet;
    		dash::mpd::IRepresentation *representation;
	};
}



#endif /* SEGMENT_TEST_H */