#include "MovieFragmentHeaderBox.h"

using namespace Boxes;
using namespace std;

MovieFragmentHeaderBox::MovieFragmentHeaderBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		sequence_number (0)
		{}
MovieFragmentHeaderBox::~MovieFragmentHeaderBox () {}

void MovieFragmentHeaderBox::SetSequenceNumber(uint32_t number) {
	this->sequence_number = number;
}

uint32_t MovieFragmentHeaderBox::GetSequenceNumber () {
	return this->sequence_number;
}

void MovieFragmentHeaderBox::Print(uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Movie Fragment Header Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Sequence Number = " << this->sequence_number << endl;
}