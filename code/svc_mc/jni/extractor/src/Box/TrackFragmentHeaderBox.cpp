#include "TrackFragmentHeaderBox.h"

using namespace Boxes;
using namespace std;

TrackFragmentHeaderBox::TrackFragmentHeaderBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		trackID (0),
		base_data_offset (0),
		sample_desc_index (0),
		default_sample_duration (0),
		default_sample_size (0),
		default_sample_flags (0)
		{}
TrackFragmentHeaderBox::~TrackFragmentHeaderBox () {}

void TrackFragmentHeaderBox::SetTrackID (uint32_t id) {
	this->trackID = id;
}
void TrackFragmentHeaderBox::SetBaseOffset (uint64_t offset) {
	this->base_data_offset = offset;
}
void TrackFragmentHeaderBox::SetSampleDescIndex (uint32_t index) {
	this->sample_desc_index = index;
}
void TrackFragmentHeaderBox::SetSampleDuration (uint32_t duration) {
	this->default_sample_duration = duration;
}
void TrackFragmentHeaderBox::SetSampleSize (uint32_t size) {
	this->default_sample_size = size;
}
void TrackFragmentHeaderBox::SetSampleFlags (uint32_t flags) {
	this->default_sample_flags = flags;
}

uint32_t TrackFragmentHeaderBox::GetTrackID () {
	return this->trackID;
}
uint64_t TrackFragmentHeaderBox::GetBaseOffset () {
	return this->base_data_offset;
}
uint32_t TrackFragmentHeaderBox::GetSampleDescIndex () {
	return this->sample_desc_index;
}
uint32_t TrackFragmentHeaderBox::GetSampleDuration () {
	return this->default_sample_duration;
}
uint32_t TrackFragmentHeaderBox::GetSampleSize () {
	return this->default_sample_size;
}
uint32_t TrackFragmentHeaderBox::GetSampleFlags () {
	return this->default_sample_flags;
}

void TrackFragmentHeaderBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Fragment Header Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Track ID = " << this->trackID << endl;
	cout << ss.str() << "  Base Data Offset = " << this->base_data_offset << endl;
	cout << ss.str() << "  Sample Description Index = " << this->sample_desc_index << endl;
	cout << ss.str() << "  Default Sample Duration = " << this->default_sample_duration << endl;
	cout << ss.str() << "  Default Sample Size = " << this->default_sample_size << endl;
	cout << ss.str() << "  Default Sample Flags = " << this->default_sample_flags << endl;
}