#ifndef STSC_ENTRY_H
#define STSC_ENTRY_H

#include "config.h"

namespace Boxes {
	class STSCEntry {
	public:
		STSCEntry ();
		~STSCEntry ();

		void SetFirstChunk (uint32_t chunk);
		void SetSamplePerChunk (uint32_t spc);
		void SetSampleDescIndex (uint32_t index);
		void SetEdited (bool edited);
		void SetNextChunk (uint32_t chunk);

		uint32_t GetFirstChunk();
		uint32_t GetSamplePerChunk ();
		uint32_t GetSampleDescIndex ();
		bool IsEdited ();
		uint32_t GetNextChunk ();

		void Print (uint32_t level);

	private:
		uint32_t firstChunk;
		uint32_t samplePerChunk;
		uint32_t sampleDescriptionIndex;
		bool isEdited;
		uint32_t nextChunk;
	};
}

#endif /* STSC_ENTRY_H */