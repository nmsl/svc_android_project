#ifndef TIME_TO_SAMPLE_BOX_H
#define TIME_TO_SAMPLE_BOX_H

#include "BaseBox.h"
#include "FullBox.h"
#include "STTSEntry.h"

namespace Boxes {
	class TimeToSampleBox : public BaseBox, public FullBox {
	public:
		TimeToSampleBox (uint32_t size, uint64_t type);
		~TimeToSampleBox ();

		void SetEntryCount (uint32_t count);
		void SetCurrentSampelNum (uint32_t num);
		void AddCurrentSampleNum (uint32_t addNum);
		void SetLastDTS (uint32_t dts);
		void AddLastDTS (uint32_t addDTS);
		void AddEntry (STTSEntry *entry);

		uint32_t GetEntryCount ();
		uint32_t GetCurrentSampleNum ();
		uint32_t GetLastDTS ();
		STTSEntry* GetEntry (uint32_t index);

		void Print (uint32_t level);

	private:
		uint32_t entry_count;
		uint32_t currentSampleNum;
		uint32_t lastDTS;
		std::vector<STTSEntry*> entryList;
	};
}

#endif /* TIME_TO_SAMPLE_BOX_H */