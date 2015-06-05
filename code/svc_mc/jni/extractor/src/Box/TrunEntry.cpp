#include "TrunEntry.h"

using namespace Boxes;
using namespace std;

TrunEntry::TrunEntry () :
		duration (0),
		size (0),
		flags (0),
		CTS_offset (0)
		{}
TrunEntry::~TrunEntry () {}

void TrunEntry::SetDuration (uint32_t d) {
	this->duration = d;
}
void TrunEntry::SetSize (uint32_t s) {
	this->size = s;
}
void TrunEntry::SetFlags (uint32_t f) {
	this->flags = f;
}
void TrunEntry::SetCTSOffset (uint32_t offset) {
	this->CTS_offset = offset;
}

uint32_t TrunEntry::GetDuration () {
	return this->duration;
}
uint32_t TrunEntry::GetSize () {
	return this->size;
}
uint32_t TrunEntry::GetFlags () {
	return this->flags;
}
uint32_t TrunEntry::GetCTSOffset () {
	return this->CTS_offset;
}

void TrunEntry::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Fragment Run Entry :" << endl;
	cout << ss.str() << "  Duration :" << this->duration << endl;
	cout << ss.str() << "  Size :" << this->size << endl;
	cout << ss.str() << "  Flags :" << this->flags << endl;
	cout << ss.str() << "  Composition Time Offset :" << this->CTS_offset << endl;
}