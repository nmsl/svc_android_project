#ifndef MOVIE_FRAGMENT_BOX_H
#define MOVIE_FRAGMENT_BOX_H

#include "BaseBox.h"
#include "MovieFragmentHeaderBox.h"
#include "TrackFragmentBox.h"

namespace Boxes {
	class MovieFragmentBox : public BaseBox {
	public:
		MovieFragmentBox (uint32_t size, uint64_t type);
		~MovieFragmentBox ();

		void SetMovieFragmentHeaderBox (MovieFragmentHeaderBox *mfhdBox);
		void AddTrackFragmentBox (TrackFragmentBox *trafBox);

		MovieFragmentHeaderBox* GetMovieFragmentHeaderBox ();
		TrackFragmentBox* GetTrackFragmentBox (uint32_t index);

		void Print(uint32_t level);
	private:
		MovieFragmentHeaderBox *mfhd;
		std::vector<TrackFragmentBox*> trackList;
	};
}

#endif /* MOVIE_FRAGMENT_BOX_H */