#ifndef MOVIE_EXTENDS_HEADER_BOX_H
#define MOVIE_EXTENDS_HEADER_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class MovieExtendsHeaderBox : public BaseBox, public FullBox {
	public:
		MovieExtendsHeaderBox (uint32_t size, uint64_t type);
		~MovieExtendsHeaderBox ();

		void SetFragmentDuration (uint64_t duration);

		uint64_t GetFragmentDuration ();

		void Print (uint32_t level);

	private:
		uint64_t fragmentDruation;
	};
}

#endif /* MOVIE_EXTENDS_HEADER_BOX_H */