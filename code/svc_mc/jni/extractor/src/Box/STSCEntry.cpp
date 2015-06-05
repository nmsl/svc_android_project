#include "STSCEntry.h"

using namespace Boxes;
using namespace std;

STSCEntry::STSCEntry () :
		firstChunk (0),
		samplePerChunk (0),
		sampleDescriptionIndex (0),
		isEdited (false),
		nextChunk (0)
		{}
STSCEntry::~STSCEntry () {}

void STSCEntry::SetFirstChunk (uint32_t chunk) {
	this->firstChunk = chunk;
}
void STSCEntry::SetSamplePerChunk (uint32_t spc) {
	this->samplePerChunk = spc;
}
void STSCEntry::SetSampleDescIndex (uint32_t index) {
	this->sampleDescriptionIndex = index;
}
void STSCEntry::SetEdited (bool edited) {
	this->isEdited = edited;
}
void STSCEntry::SetNextChunk (uint32_t chunk) {
	this->nextChunk = chunk;
}

uint32_t STSCEntry::GetFirstChunk() {
	return this->firstChunk;
}
uint32_t STSCEntry::STSCEntry::GetSamplePerChunk () {
	return this->samplePerChunk;
}
uint32_t STSCEntry::GetSampleDescIndex () {
	return this->sampleDescriptionIndex;
}
bool STSCEntry::IsEdited () {
	return this->isEdited;
}
uint32_t STSCEntry::GetNextChunk () {
	return this->nextChunk;
}

void STSCEntry::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "STSC Entry :" << endl;
	cout << ss.str() << "  First Chunk = " << this->firstChunk << endl;
	cout << ss.str() << "  Sample Per Chunk = " << this->samplePerChunk << endl;
	cout << ss.str() << "  Sample Description Index = " << this->sampleDescriptionIndex << endl;
	cout << ss.str() << "  Next Chunk = " << this->nextChunk << endl;
}