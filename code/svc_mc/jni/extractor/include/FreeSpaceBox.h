#ifndef FREE_SPACE_BOX_H
#define FREE_SPACE_BOX_H

#include "BaseBox.h"

namespace Boxes {
	class FreeSpaceBox : public BaseBox {
	public:
		FreeSpaceBox (uint32_t size, uint64_t type);
		~FreeSpaceBox ();

		void SetDataSize (uint32_t size);
		void SetData (uint8_t* data);

		uint32_t GetDataSize ();
		uint8_t* GetData();

		void Print(uint32_t level);

	private:
		uint32_t dataSize;
		uint8_t *data;
	};
}

#endif /* FREE_SPACE_BOX_H */