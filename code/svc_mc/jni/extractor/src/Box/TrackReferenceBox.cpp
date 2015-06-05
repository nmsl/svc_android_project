#include "TrackReferenceBox.h"

using namespace Boxes;
using namespace std;

TrackReferenceBox::TrackReferenceBox (uint32_t size, uint64_t type) : BaseBox (size, type) {}
TrackReferenceBox::~TrackReferenceBox () {
	uint32_t size = this->entryList.size();
	for (uint32_t i = 0; i < size; i++) {
		TrackReferenceTypeBox *box = this->entryList.back();
		this->entryList.pop_back();
		if (box) delete box;
	}
}

void TrackReferenceBox::SetReferenceBox (TrackReferenceTypeBox *refBox) {
	this->entryList.push_back(refBox);
}

uint32_t TrackReferenceBox::GetReferenceCount () {
	return this->entryList.size();
}
TrackReferenceTypeBox* TrackReferenceBox::GetReferenceBoxByIndex (uint32_t index) {
	if (GetReferenceCount() == 0)
		return NULL;
	else if (index < GetReferenceCount())
		return this->entryList.at(index);

	return this->entryList.at(0);
}
TrackReferenceTypeBox* TrackReferenceBox::GetReferenceBoxByType (uint64_t type) {
	TrackReferenceTypeBox *r = NULL;

	for (uint32_t i = 0; i < GetReferenceCount(); i++) {
		r = this->entryList.at(i);
		if (r->GetType() == type) return r;
	}

	return NULL;
}

void TrackReferenceBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Reference Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;

	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;

	for (uint32_t i = 0; i < this->entryList.size(); i++)
		this->entryList.at(i)->Print(level+1);
}