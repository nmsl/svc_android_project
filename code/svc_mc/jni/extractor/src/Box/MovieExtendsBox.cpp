#include "MovieExtendsBox.h"

using namespace Boxes;
using namespace std;

MovieExtendsBox::MovieExtendsBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		mehd (NULL)
		{}
MovieExtendsBox::~MovieExtendsBox () {
	if (this->mehd) delete this->mehd;

	uint32_t size = this->trexList.size();
	for (uint32_t i = 0; i < size; i++) {
		TrackExtendsBox *box = this->trexList.back();
		this->trexList.pop_back();
		delete box;
	}
}

void MovieExtendsBox::SetMovieExtendsHeaderBox (MovieExtendsHeaderBox *mehdBox) {
	if (!mehd) this->mehd = mehdBox;
}
void MovieExtendsBox::AddTrackExtendsBox (TrackExtendsBox *trexBox) {
	this->trexList.push_back(trexBox);
}

MovieExtendsHeaderBox* MovieExtendsBox::GetMovieExtendsHeaderBox() {
	return this->mehd;
}
TrackExtendsBox* MovieExtendsBox::GetTrackExtendsBox (uint32_t index) {
	if (this->trexList.size() == 0)
		return NULL;
	else if (index < this->trexList.size())
		return this->trexList.at(index);

	return this->trexList.at(0);
}

void MovieExtendsBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Movie Extends Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	this->mehd->Print(level+1);
	for (uint32_t i = 0; i < this->trexList.size(); i++) {
		this->trexList.at(i)->Print(level+1);
	}
}