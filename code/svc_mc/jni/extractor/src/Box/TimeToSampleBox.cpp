#include "TimeToSampleBox.h"

using namespace Boxes;
using namespace std;

TimeToSampleBox::TimeToSampleBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		entry_count (0),
		currentSampleNum (0),
		lastDTS (0)
		{}
TimeToSampleBox::~TimeToSampleBox () {
	uint32_t size = this->entryList.size();
	for (uint32_t i = 0; i < size; i++) {
		STTSEntry *entry = this->entryList.back();
		this->entryList.pop_back();
		if (entry) delete entry;
	}
}

void TimeToSampleBox::SetEntryCount (uint32_t count) {
	this->entry_count = count;
}
void TimeToSampleBox::SetCurrentSampelNum (uint32_t num) {
	this->currentSampleNum = num;
}
void TimeToSampleBox::AddCurrentSampleNum (uint32_t addNum) {
	this->currentSampleNum += addNum;
}
void TimeToSampleBox::SetLastDTS (uint32_t dts) {
	this->lastDTS = dts;
}
void TimeToSampleBox::AddLastDTS (uint32_t addDTS) {
	this->lastDTS += addDTS;
}
void TimeToSampleBox::AddEntry (STTSEntry *entry) {
	this->entryList.push_back(entry);
}

uint32_t TimeToSampleBox::GetEntryCount () {
	return this->entry_count;
}
uint32_t TimeToSampleBox::GetCurrentSampleNum () {
	return this->currentSampleNum;
}
uint32_t TimeToSampleBox::GetLastDTS () {
	return this->lastDTS;
}
STTSEntry* TimeToSampleBox::GetEntry (uint32_t index) {
	if (this->entryList.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->entryList.size())
		return this->entryList.at(index);

	return this->entryList.at(0);
}

void TimeToSampleBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Time To Sample Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Entry Count = " << this->entry_count << endl;
	for (uint32_t i = 0; i < this->GetEntryCount(); i++) {
		this->entryList.at(i)->Print(level+1);
	}
}