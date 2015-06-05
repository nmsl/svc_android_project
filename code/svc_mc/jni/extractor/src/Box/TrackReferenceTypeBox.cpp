#include "TrackReferenceTypeBox.h"

using namespace Boxes;
using namespace std;

TrackReferenceTypeBox::TrackReferenceTypeBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		count (0),
		trackIDs (NULL)
		{}
TrackReferenceTypeBox::~TrackReferenceTypeBox () {
	if (this->trackIDs) delete[] this->trackIDs;
}

void TrackReferenceTypeBox::SetTrackCount (uint32_t count) {
	this->count = count;
}
void TrackReferenceTypeBox::SetTrackID (uint32_t *id) {
	if (!this->trackIDs) this->trackIDs = id;
}

uint32_t TrackReferenceTypeBox::GetTrackIDCount () {
	return this->count;
}
uint32_t TrackReferenceTypeBox::GetTrackID (uint32_t index) {
	if (index >= 0 && index < this->count) return this->trackIDs[index];
	return 0;
}

void TrackReferenceTypeBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Reference Type Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;

	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;

	cout << ss.str() << "  Track IDs = ";
	for (uint32_t i = 0; i < this->GetTrackIDCount(); i++) {
		cout << this->trackIDs[i] << ", ";
	}
	cout << endl;
}