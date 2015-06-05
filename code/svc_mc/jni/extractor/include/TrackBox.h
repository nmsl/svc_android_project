#ifndef TRACK_BOX_H
#define TRACK_BOX_H

#include "BaseBox.h"
#include "TrackHeaderBox.h"
#include "MediaBox.h"
#include "TrackReferenceBox.h"

namespace Boxes {
	class TrackBox : public BaseBox {
	public:
		TrackBox (uint32_t size, uint64_t type);
		~TrackBox ();

		void SetTrackHeaderBox (TrackHeaderBox *tkhdBox);
		void SetMediaBox (MediaBox *mdiaBox);
		void SetReferenceBox (TrackReferenceBox *trefBox);

		TrackHeaderBox* GetTrackHeaderBox ();
		MediaBox* GetMediaBox ();
		TrackReferenceBox* GetReferenceBox ();

		void Print (uint32_t level);

	private:
		TrackHeaderBox *header;
		MediaBox *media;
		TrackReferenceBox *references;
	};
}

#endif /* TRACK_BOX_H */