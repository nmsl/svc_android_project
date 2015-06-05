#include "MovieFragmentBox.h"

using namespace Boxes;
using namespace std;

MovieFragmentBox::MovieFragmentBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		mfhd (NULL)
		{}
MovieFragmentBox::~MovieFragmentBox () {
	if (this->mfhd) delete this->mfhd;

	uint32_t size = this->trackList.size();
	for (uint32_t i = 0; i < size; i++) {
		TrackFragmentBox *box = this->trackList.back();
		this->trackList.pop_back();
		if (box) delete box;
	}
}

void MovieFragmentBox::SetMovieFragmentHeaderBox (MovieFragmentHeaderBox *mfhdBox) {
	if (!this->mfhd) this->mfhd = mfhdBox;
}
void MovieFragmentBox::AddTrackFragmentBox (TrackFragmentBox *trafBox) {
	this->trackList.push_back(trafBox);
}

MovieFragmentHeaderBox* MovieFragmentBox::GetMovieFragmentHeaderBox () {
	return this->mfhd;
}
TrackFragmentBox* MovieFragmentBox::GetTrackFragmentBox  (uint32_t index) {
	if (this->trackList.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->trackList.size())
		return this->trackList.at(index);

	return this->trackList.at(0);
}

void MovieFragmentBox::Print(uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Movie Fragment Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	this->mfhd->Print(level+1);
	for (uint32_t i = 0; i < this->trackList.size(); i++) {
		this->trackList.at(i)->Print(level+1);
	}
}