#include "SegmentIndexBox.h"

using namespace Boxes;
using namespace std;

SegmentIndexBox::SegmentIndexBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		reference_ID (0),
		timescale (0),
		earliest_presentation_time (0),
		first_offset (0)
		{}
SegmentIndexBox::~SegmentIndexBox () {
	uint32_t size = this->refs.size();
	for (uint32_t i = 0; i < size; i++) {
		SIDXBoxRef *ref = this->refs.back();
		this->refs.pop_back();
		if (ref) delete ref;
	}
}

void SegmentIndexBox::SetReferenceID (uint32_t ID) {
	this->reference_ID = ID;
}
void SegmentIndexBox::SetTimeScale (uint32_t timeS) {
	this->timescale = timeS;
}
void SegmentIndexBox::SetPresentationTime (uint64_t t) {
	this->earliest_presentation_time = t;
}
void SegmentIndexBox::SetFirstOffset (uint64_t offset) {
	this->first_offset = offset;
}
void SegmentIndexBox::SetReference (SIDXBoxRef *ref) {
	this->refs.push_back (ref);
}

uint32_t SegmentIndexBox::GetReferenceID () {
	return this->reference_ID;
}
uint32_t SegmentIndexBox::GetTimeScale () {
	return this->timescale;
}
uint64_t SegmentIndexBox::GetPresentationTime () {
	return this->earliest_presentation_time;
}
uint64_t SegmentIndexBox::GetFirstOffset () {
	return this->first_offset;
}
uint32_t SegmentIndexBox::GetReferenceCount () {
	return this->refs.size();
}
SIDXBoxRef* SegmentIndexBox::GetReference (uint32_t index) {
	if (GetReferenceCount() == 0)
		return NULL;
	else if (index < GetReferenceCount())
		return this->refs.at(index);

	return this->refs.at(0);
}

void SegmentIndexBox::Print(uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Segment Index Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Reference ID = " << this->reference_ID << endl;
	cout << ss.str() << "  Time Scale = " << this->timescale << endl;
	cout << ss.str() << "  Earliest Presentation Time = " << this->earliest_presentation_time << endl;
	cout << ss.str() << "  First OffSet = " << this->first_offset << endl;
	for (uint32_t i = 0; i < this->refs.size(); i++) {
		this->refs.at(i)->Print(level+1);
	}
}