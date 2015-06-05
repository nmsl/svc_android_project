#ifndef MEDIA_DATA_BOX_H
#define MEDIA_DATA_BOX_H

#include "TrackFragmentRunBox.h"
#include "MediaDataEntry.h"

namespace Boxes {
	class MediaDataBox : public BaseBox {
	public:
		MediaDataBox (uint32_t size, uint64_t type);
		~MediaDataBox ();

		void SetRefTRUN (TrackFragmentRunBox *trun);
		void AddMediaData (MediaDataEntry* sample);

		TrackFragmentRunBox* GetRefTRUN ();
		MediaDataEntry* GetMediaData (uint32_t index);

		void Print(uint32_t level);

	private:
		TrackFragmentRunBox *refTRUN;
		std::vector<MediaDataEntry*> dataList;
	};
}

#endif /* MEDIA_DATA_BOX_H */