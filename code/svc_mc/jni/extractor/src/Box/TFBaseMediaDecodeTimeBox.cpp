#include "TFBaseMediaDecodeTimeBox.h"

using namespace Boxes;
using namespace std;

TFBaseMediaDecodeTimeBox::TFBaseMediaDecodeTimeBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		baseMediaDecodeTime (0)
		{}
TFBaseMediaDecodeTimeBox::~TFBaseMediaDecodeTimeBox () {}

void TFBaseMediaDecodeTimeBox::SetMediaDecodeTime (uint64_t time) {
	this->baseMediaDecodeTime = time;
}

uint64_t TFBaseMediaDecodeTimeBox::GetMediaDecodeTime () {
	return this->baseMediaDecodeTime;
}

void TFBaseMediaDecodeTimeBox::Print(uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Fragment Media Decode Time Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Base Media Decode Time = " << this->baseMediaDecodeTime << endl;
}