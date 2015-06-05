#include "TrackFragmentBox.h"

using namespace Boxes;
using namespace std;

TrackFragmentBox::TrackFragmentBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		tfhd (NULL),
		tfdt (NULL),
		trun (NULL)
		{}
TrackFragmentBox::~TrackFragmentBox () {
	if (this->tfhd) delete this->tfhd;
	if (this->tfdt) delete this->tfdt;
	if (this->trun) delete this->trun;
}

void TrackFragmentBox::SetTrackFragmentHeaderBox (TrackFragmentHeaderBox *tfhdBox) {
	if (!this->tfhd) this->tfhd = tfhdBox;
}
void TrackFragmentBox::SetTFBaseMediaDecodeTimeBox (TFBaseMediaDecodeTimeBox *tfdtBox) {
	if (!this->tfdt) this->tfdt = tfdtBox;
}
void TrackFragmentBox::SetTrackFragmentRunBox (TrackFragmentRunBox *trunBox) {
	if (!this->trun) this->trun = trunBox;
}

TrackFragmentHeaderBox* TrackFragmentBox::GetTrackFragmentHeaderBox () {
	return this->tfhd;
}
TFBaseMediaDecodeTimeBox* TrackFragmentBox::GetTFBaseMediaDecodeTimeBox () {
	return this->tfdt;
}
TrackFragmentRunBox* TrackFragmentBox::GetTrackFragmentRunBox () {
	return this->trun;
}

void TrackFragmentBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Fragment Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	if (this->tfhd) this->tfhd->Print(level+1);
	if (this->tfdt) this->tfdt->Print(level+1);
	if (this->trun) this->trun->Print(level+1);
}