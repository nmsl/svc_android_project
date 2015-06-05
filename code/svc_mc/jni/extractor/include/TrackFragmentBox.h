#ifndef TRACK_FRAGMENT_BOX_H
#define TRACK_FRAGMENT_BOX_H

#include "BaseBox.h"
#include "TrackFragmentHeaderBox.h"
#include "TFBaseMediaDecodeTimeBox.h"
#include "TrackFragmentRunBox.h"

namespace Boxes {
	class TrackFragmentBox : public BaseBox {
	public:
		TrackFragmentBox (uint32_t size, uint64_t type);
		~TrackFragmentBox ();

		void SetTrackFragmentHeaderBox (TrackFragmentHeaderBox *tfhdBox);
		void SetTFBaseMediaDecodeTimeBox (TFBaseMediaDecodeTimeBox *tfdtBox);
		void SetTrackFragmentRunBox (TrackFragmentRunBox *trunBox);

		TrackFragmentHeaderBox* GetTrackFragmentHeaderBox ();
		TFBaseMediaDecodeTimeBox* GetTFBaseMediaDecodeTimeBox ();
		TrackFragmentRunBox* GetTrackFragmentRunBox ();

		void Print(uint32_t level);

	private:
		TrackFragmentHeaderBox *tfhd;
		TFBaseMediaDecodeTimeBox *tfdt;
		TrackFragmentRunBox *trun;
	};
}

#endif /* TRACK_FRAGMENT_BOX_H */