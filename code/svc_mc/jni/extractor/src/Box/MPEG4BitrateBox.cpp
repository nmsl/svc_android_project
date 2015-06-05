#include "MPEG4BitrateBox.h"

using namespace Boxes;
using namespace std;

MPEG4BitrateBox::MPEG4BitrateBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		bufferSizeDB (0),
		maxBitrate (0),
		avgBitrate (0)
		{}
MPEG4BitrateBox::~MPEG4BitrateBox () {}

void MPEG4BitrateBox::SetBufferSizeDB (uint32_t bufferSize) {
	this->bufferSizeDB = bufferSize;
}
void MPEG4BitrateBox::SetMaxBitrate (uint32_t max) {
	this->maxBitrate = max;
}
void MPEG4BitrateBox::SetAvgBitrate (uint32_t avg) {
	this->avgBitrate = avg;
}

uint32_t MPEG4BitrateBox::GetBufferSizeDB () {
	return this->bufferSizeDB;
}
uint32_t MPEG4BitrateBox::GetMaxBitrate () {
	return this->maxBitrate;
}
uint32_t MPEG4BitrateBox::GetAvgBitrate () {
	return this->avgBitrate;
}

void MPEG4BitrateBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "MPEG Bitrate Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Buffer Size = " << this->bufferSizeDB << endl;
	cout << ss.str() << "  Max. Bitrate = " << this->maxBitrate << endl;
	cout << ss.str() << "  Avg. Bitrate = " << this->avgBitrate << endl;
}