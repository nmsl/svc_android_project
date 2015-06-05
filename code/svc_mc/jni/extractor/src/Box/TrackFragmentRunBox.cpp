#include "TrackFragmentRunBox.h"

using namespace Boxes;
using namespace std;

TrackFragmentRunBox::TrackFragmentRunBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		sample_count (0),
		data_offset (0),
		first_sample_flags (0)
		{}
TrackFragmentRunBox::~TrackFragmentRunBox () {
	uint32_t size = this->samples.size();
	for (uint32_t i = 0; i < size; i++) {
		TrunEntry *entry = this->samples.back();
		this->samples.pop_back();
		if (entry) delete entry;
	}
}

void TrackFragmentRunBox::SetSampleCount (uint32_t count) {
	this->sample_count = count;
}
void TrackFragmentRunBox::SetDataOffset (uint32_t offset) {
	this->data_offset = offset;
}
void TrackFragmentRunBox::SetFirstSampleFlags (uint32_t flags) {
	this->first_sample_flags = flags;
}
void TrackFragmentRunBox::AddEntry (TrunEntry* entry) {
	this->samples.push_back(entry);
}

uint32_t TrackFragmentRunBox::GetSampleCount () {
	return this->sample_count;
}
uint32_t TrackFragmentRunBox::GetDataOffset () {
	return this->data_offset;
}
uint32_t TrackFragmentRunBox::GetFirstSampleFlags () {
	return this->first_sample_flags;
}
TrunEntry* TrackFragmentRunBox::GetEntry(uint32_t index) {
	if (this->samples.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->samples.size())
		return this->samples.at(index);

	return this->samples.at(0);
}

void TrackFragmentRunBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Fragment Run Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;;
	cout << ss.str() << "  Sample Count = " << this->sample_count << endl;
	cout << ss.str() << "  Data Offset = " << this->data_offset << endl;
	cout << ss.str() << "  First Sample Flags = " << this->first_sample_flags << endl;
	for (uint32_t i = 0; i < this->samples.size(); i++) {
		this->samples.at(i)->Print(level+1);
	}
}