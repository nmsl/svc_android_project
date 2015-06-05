#ifndef TRACK_REFERENCE_TYPE_BOX_H
#define TRACK_REFERENCE_TYPE_BOX_H

#include "BaseBox.h"

namespace Boxes {
	class TrackReferenceTypeBox : public BaseBox {
	public:
		TrackReferenceTypeBox (uint32_t size, uint64_t type);
		~TrackReferenceTypeBox ();

		void SetTrackCount (uint32_t count);
		void SetTrackID (uint32_t *id);

		uint32_t GetTrackIDCount ();
		uint32_t GetTrackID (uint32_t index);

		void Print (uint32_t level);

	private:
		uint32_t count;
		uint32_t* trackIDs;
	};
}

#endif /* TRACK_REFERENCE_TYPE_BOX_H */