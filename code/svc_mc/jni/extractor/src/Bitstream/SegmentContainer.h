#ifndef SEGMENT_CONTAINER_H
#define SEGMENT_CONTAINER_H

#include "config.h"
#include "AllBoxes.h"

using namespace Boxes;

namespace Bitstream {
	enum SegmentType{
		INIT_SEGMENT = 1,
		DATA_SEGMENT = 2
	};

	class SegmentContainer {
	public:
		SegmentContainer (SegmentType t);
		~SegmentContainer ();

		SegmentType GetType ();
		uint32_t GetTrackID ();

		/* Data Segment */
		void SetSegmentIndexBox (SegmentIndexBox *sidxBox);
		void SetMovieFragmentBox (MovieFragmentBox *moofBox);
		void SetMediaDataBox (MediaDataBox *mdatBox);

		SegmentIndexBox* GetSegmentIndexBox ();
		MovieFragmentBox* GetMovieFragmentBox ();
		MediaDataBox* GetMediaDataBox();

		/* Init Segment */
		void SetFileTypeBox (FileTypeBox *ftypBox);
		void SetFreeSpaceBox (FreeSpaceBox *freeBox);
		void SetMovieBox (MovieBox *moovBox);

		FileTypeBox* GetFileTypeBox();
		FreeSpaceBox* GetFreeSpaceBox();
		MovieBox* GetMovieBox();

	private:
		SegmentType type;
		uint32_t TrackID;

		/* For Data Segment */
		SegmentIndexBox *sidx;
		MovieFragmentBox *moof;
		MediaDataBox *mdat;

		/* For Init Segment */
		FileTypeBox *ftyp;
		FreeSpaceBox *free;
		MovieBox *moov;

		bool CheckType(SegmentType t);
	};
}

#endif /* SEGMENT_CONTAINER_H */