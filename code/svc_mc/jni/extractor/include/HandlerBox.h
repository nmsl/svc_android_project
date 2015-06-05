#ifndef HANDLER_BOX_H
#define HANDLER_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class HandlerBox : public BaseBox, public FullBox {
	public:
		HandlerBox (uint32_t size, uint64_t type);
		~HandlerBox ();

		void SetReserved1 (uint32_t value);
		void SetHandlerType (uint32_t type);
		void SetNameUTF8Size (uint32_t size);
		void SetNameUTF8 (uint8_t *data);

		uint32_t GetReserved1 ();
		uint32_t GetHandlerType ();
		uint8_t* GetReserved2 ();
		uint8_t* GetNameUTF8 ();

		void Print (uint32_t level);

	private:
		uint32_t reserved1;
		uint32_t handlerType;
		uint8_t reserved2[12];
		uint32_t nameUTF8Size;
		uint8_t *nameUTF8;
	};
}


#endif /* HANDLER_BOX_H */