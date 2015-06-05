#include "SampleToChunkBox.h"

using namespace Boxes;
using namespace std;

SampleToChunkBox::SampleToChunkBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		entry_count (0)
{
	this->currentIndex = 0;
	this->firstSampleInCurrentChunk = 0;
	this->currentChunk = 0;
	this->gostNumber = 0;
}
SampleToChunkBox::~SampleToChunkBox () {
	uint32_t size = this->entryList.size();
	for (uint32_t i = 0; i < size; i++) {
		STSCEntry *entry = this->entryList.back();
		this->entryList.pop_back();
		if (entry) delete entry;
	}
}

void SampleToChunkBox::SetEntryCount (uint32_t count) {
	this->entry_count = count;
}
void SampleToChunkBox::AddEntry (STSCEntry *entry) {
	this->entryList.push_back(entry);
}

uint32_t SampleToChunkBox::GetEntryCount () {
	// return this->entry_count;
	return this->entryList.size();
}
STSCEntry* SampleToChunkBox::GetEntry (uint32_t index) {
	if (this->entryList.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->entryList.size())
		return this->entryList.at(index);

	return this->entryList.at(0);
}

void SampleToChunkBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Smaple To Chunk Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Entry Count = " << this->GetEntryCount() << endl;
	for (uint32_t i = 0; i < this->entry_count; i++) {
		this->entryList.at(i)->Print(level+1);
	}
}