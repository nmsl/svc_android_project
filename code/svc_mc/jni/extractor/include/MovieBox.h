#ifndef MOVIE_BOX_H
#define MOVIE_BOX_H

#include "BaseBox.h"
#include "MovieHeaderBox.h"
#include "MovieExtendsBox.h"
#include "TrackBox.h"

namespace Boxes {
	class MovieBox : public BaseBox {
	public:
		MovieBox (uint32_t size, uint64_t type);
		~MovieBox ();

		void SetMovieHeaderBox (MovieHeaderBox *mvhdBox);
		void SetMovieExtendsBox (MovieExtendsBox *mvexBox);
		void AddTrackBox (TrackBox *track);

		MovieHeaderBox* GetMovieHeaderBox ();
		MovieExtendsBox* GetMovieExtendsBox ();
		TrackBox* GetTrackBox (uint32_t index);

		void Print (uint32_t level);
	private:
		MovieHeaderBox *mvhd;
		MovieExtendsBox *mvex;
		std::vector<TrackBox*> trackList;
	};
}

#endif /* MOVIE_BOX_H */