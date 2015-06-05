#ifndef TRACK_REFERENCE_BOX_H
#define TRACK_REFERENCE_BOX_H

#include "BaseBox.h"
#include "TrackReferenceTypeBox.h"

namespace Boxes {
	class TrackReferenceBox : public BaseBox {
	public:
		TrackReferenceBox (uint32_t size, uint64_t type);
		~TrackReferenceBox ();

		void SetReferenceBox (TrackReferenceTypeBox *refBox);

		uint32_t GetReferenceCount ();
		TrackReferenceTypeBox* GetReferenceBoxByIndex (uint32_t index);
		TrackReferenceTypeBox* GetReferenceBoxByType (uint64_t type);

		void Print (uint32_t level);

	private:
		std::vector<TrackReferenceTypeBox*> entryList;
	};
}

#endif /* TRACK_REFERENCE_BOX_H */