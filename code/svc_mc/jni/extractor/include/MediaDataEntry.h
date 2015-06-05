#ifndef MEDIA_DATA_ENTRY_H
#define MEDIA_DATA_ENTRY_H

#include "config.h"

namespace Boxes {
	class MediaDataEntry {
	public:
		MediaDataEntry ();
		~MediaDataEntry ();

		void SetNumber (uint32_t no);
		void SetSize (uint32_t size);
		void SetDataOffset (uint32_t offset);
		void SetData (uint8_t *data);

		uint32_t GetNumber ();
		uint32_t GetSize ();
		uint32_t GetDataOffset ();
		uint8_t* GetData();

		void Print(uint32_t level);

	private:
		uint32_t sampleNumber;
		uint32_t samplesize;
		uint32_t data_offset;
		uint8_t *data;
	};
}

#endif /* MEDIA_DATA_ENTRY_H */