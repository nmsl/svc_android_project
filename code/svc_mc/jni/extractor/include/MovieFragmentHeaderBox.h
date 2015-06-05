#ifndef MOVIE_FRAGMENT_HEADER_BOX_H
#define MOVIE_FRAGMENT_HEADER_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class MovieFragmentHeaderBox : public BaseBox, public FullBox {
	public:
		MovieFragmentHeaderBox (uint32_t size, uint64_t type);
		~MovieFragmentHeaderBox ();

		void SetSequenceNumber (uint32_t number);

		uint32_t GetSequenceNumber ();

		void Print(uint32_t level);
	private:
		uint32_t sequence_number;
	};
}

#endif /* MOVIE_FRAGMENT_HEADER_BOX_H */