#ifndef SAMPLE_TO_CHUNK_BOX_H
#define SAMPLE_TO_CHUNK_BOX_H

#include "BaseBox.h"
#include "FullBox.h"
#include "STSCEntry.h"

namespace Boxes {
	class SampleToChunkBox : public BaseBox, public FullBox {
	public:
		SampleToChunkBox (uint32_t size, uint64_t type);
		~SampleToChunkBox ();

		void SetEntryCount (uint32_t count);
		void AddEntry (STSCEntry *entry);

		uint32_t GetEntryCount ();
		STSCEntry* GetEntry (uint32_t index);

		uint32_t currentIndex;
		uint32_t firstSampleInCurrentChunk;
		uint32_t currentChunk;
		uint32_t gostNumber;

		void Print (uint32_t level);

	private:
		uint32_t entry_count;
		std::vector<STSCEntry*> entryList;
	};
}

#endif /* SAMPLE_TO_CHUNK_BOX_H */