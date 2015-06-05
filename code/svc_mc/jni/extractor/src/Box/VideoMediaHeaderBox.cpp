#include "VideoMediaHeaderBox.h"

using namespace Boxes;
using namespace std;

VideoMediaHeaderBox::VideoMediaHeaderBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		reserved (0)
		{}
VideoMediaHeaderBox::~VideoMediaHeaderBox () {}

void VideoMediaHeaderBox::SetReserved (uint64_t value) {
	this->reserved = value;
}

uint64_t VideoMediaHeaderBox::GetReserved () {
	return this->reserved;
}

void VideoMediaHeaderBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Video Media Header Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Reserved = " << this->reserved << endl;
}