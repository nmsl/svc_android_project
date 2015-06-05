#include "STTSEntry.h"

using namespace Boxes;
using namespace std;

STTSEntry::STTSEntry () :
		sampleCount (0),
		sampleDelta (0)
		{}
STTSEntry::~STTSEntry () {}

void STTSEntry::SetSampleCount (uint32_t count) {
	this->sampleCount = count;
}
void STTSEntry::SetSampleDelta (uint32_t delta) {
	this->sampleDelta = delta;
}

uint32_t STTSEntry::GetSampleCount () {
	return this->sampleCount;
}
uint32_t STTSEntry::GetSampleDelta () {
	return this->sampleDelta;
}

void STTSEntry::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "STTS Entry :" << endl;
	cout << ss.str() << "  Sample Count = " << this->sampleCount << endl;
	cout << ss.str() << "  Sample Delta = " << this->sampleDelta << endl;
}