#ifndef AVC_CONFIG_SLOT_H
#define AVC_CONFIG_SLOT_H

#include "config.h"

namespace Boxes {
	class AVCConfigSlot {
	public:
		AVCConfigSlot ();
		~AVCConfigSlot ();

		void SetDataSize (uint32_t s);
		void SetData (uint8_t *data);

		uint8_t* GetData();
		uint32_t GetSize ();

		void Print (uint32_t level);

	private:
		uint8_t *data;
		uint32_t size;
	};
}

#endif /* AVC_CONFIG_SLOT_H */