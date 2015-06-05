#ifndef TRACK_EXTENDS_BOX_H
#define TRACK_EXTENDS_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class TrackExtendsBox : public BaseBox, public FullBox {
	public:
		TrackExtendsBox (uint32_t size, uint64_t type);
		~TrackExtendsBox ();

		void SetTrackID (uint32_t id);
		void SetSampleDescIndex (uint32_t index);
		void SetSampleDuration (uint32_t duration);
		void SetSampleSize (uint32_t size);
		void SetSampleFlags (uint32_t flags);

		uint32_t GetTrackID ();
		uint32_t GetSampleDescIndex ();
		uint32_t GetSampleDuration ();
		uint32_t GetSampleSize ();
		uint32_t GetSampleFlags ();

		void Print (uint32_t level);

	private:
		uint32_t trackID;
		uint32_t default_sample_desc_index;
		uint32_t default_sample_duration;
		uint32_t default_sample_size;
		uint32_t default_sample_flags;
	};
}

#endif /* TRACK_EXTENDS_BOX_H */