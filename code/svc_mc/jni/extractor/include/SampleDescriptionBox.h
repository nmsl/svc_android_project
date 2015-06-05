#ifndef SAMPLE_DESCRIPTION_BOX_H
#define SAMPLE_DESCRIPTION_BOX_H

#include "BaseBox.h"
#include "FullBox.h"
#include "MPEGVisualSampleEntryBox.h"

namespace Boxes {
	class SampleDescriptionBox : public BaseBox, public FullBox {
	public:
		SampleDescriptionBox (uint32_t size, uint64_t type);
		~SampleDescriptionBox ();

		void SetEntryCount (uint32_t count);
		void AddEntry (MPEGVisualSampleEntryBox* entry);

		uint32_t GetEntryCount ();
		MPEGVisualSampleEntryBox* GetEntry (uint32_t index);

		void Print (uint32_t level);

	private:
		uint32_t entry_count;
		std::vector<MPEGVisualSampleEntryBox*> entryList;
	};
}

#endif /* SAMPLE_DESCRIPTION_BOX_H */