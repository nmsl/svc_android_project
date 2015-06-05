#ifndef TRACK_FRAGMENT_RUN_BOX_H
#define TRACK_FRAGMENT_RUN_BOX_H

#include "BaseBox.h"
#include "FullBox.h"
#include "TrunEntry.h"

namespace Boxes {
	class TrackFragmentRunBox : public BaseBox, public FullBox {
	public:
		TrackFragmentRunBox (uint32_t size, uint64_t type);
		~TrackFragmentRunBox ();

		void SetSampleCount (uint32_t count);
		void SetDataOffset (uint32_t offset);
		void SetFirstSampleFlags (uint32_t flags);
		void AddEntry (TrunEntry* entry);

		uint32_t GetSampleCount ();
		uint32_t GetDataOffset ();
		uint32_t GetFirstSampleFlags ();
		TrunEntry* GetEntry(uint32_t index);

		void Print (uint32_t level);

	private:
		uint32_t sample_count;
		uint32_t data_offset;
		uint32_t first_sample_flags;
		std::vector<TrunEntry*> samples;
	};
}

#endif /* TRACK_FRAGMENT_RUN_BOX_H */