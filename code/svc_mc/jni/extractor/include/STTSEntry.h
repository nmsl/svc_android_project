#ifndef STTS_ENTRY_H
#define STTS_ENTRY_H

#include "config.h"

namespace Boxes {
	class STTSEntry {
	public:
		STTSEntry ();
		~STTSEntry ();

		void SetSampleCount (uint32_t count);
		void SetSampleDelta (uint32_t delta);

		uint32_t GetSampleCount ();
		uint32_t GetSampleDelta ();

		void Print (uint32_t level);

	private:
		uint32_t sampleCount;
		uint32_t sampleDelta;
	};
}

#endif /* STTS_ENTRY_H */