#include "MovieBox.h"

using namespace Boxes;
using namespace std;

MovieBox::MovieBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		mvhd (NULL),
		mvex (NULL)
		{}
MovieBox::~MovieBox () {
	if (this->mvhd) delete this->mvhd;
	if (this->mvex) delete this->mvex;

	uint32_t size = this->trackList.size();
	for (uint32_t i = 0; i < size; i++) {
		TrackBox *track = this->trackList.back();
		this->trackList.pop_back();
		delete track;
	}
}

void MovieBox::SetMovieHeaderBox (MovieHeaderBox *mvhdBox) {
	if (!this->mvhd) this->mvhd = mvhdBox;
}
void MovieBox::SetMovieExtendsBox (MovieExtendsBox *mvexBox) {
	if (!this->mvex) this->mvex = mvexBox;
}
void MovieBox::AddTrackBox (TrackBox *track) {
	this->trackList.push_back(track);
}

MovieHeaderBox* MovieBox::GetMovieHeaderBox () {
	return this->mvhd;
}
MovieExtendsBox* MovieBox::GetMovieExtendsBox () {
	return this->mvex;
}
TrackBox* MovieBox::GetTrackBox (uint32_t index) {
	if (this->trackList.size() == 0)
		return NULL;
	else if (index < this->trackList.size())
		return this->trackList.at(index);

	return this->trackList.at(0);
}

void MovieBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Movie Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;

	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;

	this->mvhd->Print(level+1);
	this->mvex->Print(level+1);
	for (uint32_t i = 0; i < this->trackList.size(); i++) {
		this->trackList.at(i)->Print(0);
	}
}