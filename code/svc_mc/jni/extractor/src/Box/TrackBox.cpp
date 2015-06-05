#include "TrackBox.h"

using namespace Boxes;
using namespace std;

TrackBox::TrackBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		header (NULL),
		media (NULL),
		references (NULL)
		{}
TrackBox::~TrackBox () {
	if (this->header) delete this->header;
	if (this->media) delete this->media;
	if (this->references) delete this->references;
}

void TrackBox::SetTrackHeaderBox (TrackHeaderBox *tkhdBox) {
	if (!this->header) this->header = tkhdBox;
}
void TrackBox::SetMediaBox (MediaBox *mdiaBox) {
	if (!this->media) this->media = mdiaBox;
}
void TrackBox::SetReferenceBox (TrackReferenceBox *trefBox) {
	if (!this->references) this->references = trefBox;
}

TrackHeaderBox* TrackBox::GetTrackHeaderBox () {
	return this->header;
}
MediaBox* TrackBox::GetMediaBox () {
	return this->media;
}
TrackReferenceBox* TrackBox::GetReferenceBox () {
	return this->references;
}

void TrackBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;

	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;

	this->header->Print(level+1);
	if (this->references) this->references->Print(level+1);
	this->media->Print(level+1);
}