#ifndef BASEBOX_H
#define BASEBOX_H

#include "config.h"

namespace Boxes {
	class BaseBox {
	public:
		BaseBox(uint32_t size, uint64_t type);
		~BaseBox();

		void SetSize(uint32_t s);
		void SetType(uint64_t t);
		void IncRemaind(uint32_t v);
		void DecRemaind(uint32_t v);

		uint32_t GetSize();
		uint64_t GetType();
		uint32_t HasData();

	private:
		uint32_t size;
		uint64_t type;
		uint32_t remained;
	};
}

#endif /* BASEBOX_H */