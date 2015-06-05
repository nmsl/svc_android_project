#ifndef FULLBOX_H
#define FULLBOX_H

#include "config.h"

namespace Boxes {
	class FullBox {
	public:
		FullBox ();
		~FullBox ();

		void SetVersion (uint8_t v);
		void SetFlags (uint32_t f);

		uint8_t GetVersion ();
		uint32_t GetFlags ();

	private:
		uint8_t version;
		uint32_t flags;
	};
}

#endif /* FULLBOX_H */