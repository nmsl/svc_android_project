#ifndef DATA_ENTRY_URL_BOX_H
#define DATA_ENTRY_URL_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class DataEntryURLBox : public BaseBox, public FullBox {
	public:
		DataEntryURLBox (uint32_t size, uint64_t type);
		~DataEntryURLBox ();

		void SetCharCount (uint32_t count);
		void SetLocation (uint8_t *data);

		uint8_t* GetLocation ();
		uint32_t GetCharCount ();

		void Print (uint32_t level);

	private:
		uint8_t *location;
		uint32_t charCount;
	};
}

#endif /* DATA_ENTRY_URL_BOX_H */