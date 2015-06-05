#ifndef TRACK_FRAGMENT_HEADER_BOX_H
#define TRACK_FRAGMENT_HEADER_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	enum {
		TRAF_BASE_OFFSET	= 0x01,
		TRAF_SAMPLE_DESC 	= 0x02,
		TRAF_SAMPLE_DUR 	= 0x08,
		TRAF_SAMPLE_SIZE	= 0x10,
		TRAF_SAMPLE_FLAGS 	= 0x20
	};

	class TrackFragmentHeaderBox : public BaseBox, public FullBox {
	public:
		TrackFragmentHeaderBox (uint32_t size, uint64_t type);
		~TrackFragmentHeaderBox();

		void SetTrackID (uint32_t id);
		void SetBaseOffset (uint64_t offset);
		void SetSampleDescIndex (uint32_t index);
		void SetSampleDuration (uint32_t duration);
		void SetSampleSize (uint32_t size);
		void SetSampleFlags (uint32_t flags);

		uint32_t GetTrackID ();
		uint64_t GetBaseOffset ();
		uint32_t GetSampleDescIndex ();
		uint32_t GetSampleDuration ();
		uint32_t GetSampleSize ();
		uint32_t GetSampleFlags ();

		void Print(uint32_t level);

	private:
		uint32_t trackID;
		uint64_t base_data_offset;
		uint32_t sample_desc_index;
		uint32_t default_sample_duration;
		uint32_t default_sample_size;
		uint32_t default_sample_flags;
	};
}

#endif /* TRACK_FRAGMENT_HEADER_BOX_H */