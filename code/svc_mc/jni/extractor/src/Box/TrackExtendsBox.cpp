#include "TrackExtendsBox.h"

using namespace Boxes;
using namespace std;

TrackExtendsBox::TrackExtendsBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		trackID (0),
		default_sample_desc_index (1),
		default_sample_duration (0),
		default_sample_size (0),
		default_sample_flags (0)
		{}
TrackExtendsBox::~TrackExtendsBox () {}

void TrackExtendsBox::SetTrackID (uint32_t id) {
	this->trackID = id;
}
void TrackExtendsBox::SetSampleDescIndex (uint32_t index) {
	if (!index)
		this->default_sample_desc_index = index;
}
void TrackExtendsBox::SetSampleDuration (uint32_t duration) {
	this->default_sample_duration = duration;
}
void TrackExtendsBox::SetSampleSize (uint32_t size) {
	this->default_sample_size = size;
}
void TrackExtendsBox::SetSampleFlags (uint32_t flags) {
	this->default_sample_flags = flags;
}

uint32_t TrackExtendsBox::GetTrackID () {
	return this->trackID;
}
uint32_t TrackExtendsBox::GetSampleDescIndex () {
	return this->default_sample_desc_index;
}
uint32_t TrackExtendsBox::GetSampleDuration () {
	return this->default_sample_duration;
}
uint32_t TrackExtendsBox::GetSampleSize () {
	return this->default_sample_size;
}
uint32_t TrackExtendsBox::GetSampleFlags () {
	return this->default_sample_flags;
}

void TrackExtendsBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Extends Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Track ID = " << this->trackID << endl;
	cout << ss.str() << "  Sample Description Index = " << this->default_sample_desc_index << endl;
	cout << ss.str() << "  Sample Duration = " << this->default_sample_duration << endl;
	cout << ss.str() << "  Sample Size = " << this->default_sample_size << endl;
	cout << ss.str() << "  Sample Flags = " << this->default_sample_flags << endl;
}