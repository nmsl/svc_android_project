#include "MovieExtendsHeaderBox.h"

using namespace Boxes;
using namespace std;

MovieExtendsHeaderBox::MovieExtendsHeaderBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		fragmentDruation (0)
		{}
MovieExtendsHeaderBox::~MovieExtendsHeaderBox () {}

void MovieExtendsHeaderBox::SetFragmentDuration (uint64_t duration) {
	this->fragmentDruation = duration;
}

uint64_t MovieExtendsHeaderBox::GetFragmentDuration () {
	return this->fragmentDruation;
}

void MovieExtendsHeaderBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Movie Extends Header Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Fragment Duration = " << this->fragmentDruation << endl;
}