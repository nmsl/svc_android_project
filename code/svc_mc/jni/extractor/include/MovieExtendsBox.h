#ifndef MOVIE_EXTENDS_BOX_H
#define MOVIE_EXTENDS_BOX_H

#include "BaseBox.h"
#include "MovieExtendsHeaderBox.h"
#include "TrackExtendsBox.h"

namespace Boxes {
	class MovieExtendsBox : public BaseBox {
	public:
		MovieExtendsBox (uint32_t size, uint64_t type);
		~MovieExtendsBox ();

		void SetMovieExtendsHeaderBox (MovieExtendsHeaderBox *mehdBox);
		void AddTrackExtendsBox (TrackExtendsBox *trexBox);

		MovieExtendsHeaderBox* GetMovieExtendsHeaderBox();
		TrackExtendsBox* GetTrackExtendsBox (uint32_t index);

		void Print (uint32_t level);

	private:
		MovieExtendsHeaderBox *mehd;
		std::vector<TrackExtendsBox*> trexList;
	};
}

#endif /* MOVIE_EXTENDS_BOX_H */