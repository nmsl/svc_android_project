#ifndef SEGMENT_INDEX_BOX_H
#define SEGMENT_INDEX_BOX_H

#include "BaseBox.h"
#include "FullBox.h"
#include "SIDXBoxRef.h"

namespace Boxes {
	class SegmentIndexBox : public BaseBox, public FullBox {
	public:
		SegmentIndexBox (uint32_t size, uint64_t type);
		~SegmentIndexBox ();

		void SetReferenceID (uint32_t ID);
		void SetTimeScale (uint32_t timeS);
		void SetPresentationTime (uint64_t time);
		void SetFirstOffset (uint64_t offset);
		void SetReference (SIDXBoxRef* ref);

		uint32_t GetReferenceID ();
		uint32_t GetTimeScale ();
		uint64_t GetPresentationTime ();
		uint64_t GetFirstOffset ();
		uint32_t GetReferenceCount ();
		SIDXBoxRef* GetReference (uint32_t index);

		void Print(uint32_t level);
	private:
		uint32_t reference_ID;
		uint32_t timescale;
		uint64_t earliest_presentation_time;
		uint64_t first_offset;
		std::vector<SIDXBoxRef*> refs;
	};
}

#endif /* SEGMENT_INDEX_BOX_H */