#include "TrackHeaderBox.h"

using namespace Boxes;
using namespace std;

TrackHeaderBox::TrackHeaderBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		creationTime (0),
		modificationTime (0),
		trackID (0),
		reserved1 (0),
		duration (0),
		layer (0),
		alternate_group (0),
		volume (0),
		reserved3 (0),
		width (0),
		height (0)
		{}
TrackHeaderBox::~TrackHeaderBox () {}

void TrackHeaderBox::SetCreationTime (uint64_t time) {
	this->creationTime = time;
}
void TrackHeaderBox::SetModificationTime (uint64_t time) {
	this->modificationTime = time;
}
void TrackHeaderBox::SetTrackID (uint32_t id) {
	this->trackID = id;
}
void TrackHeaderBox::SetReserved1 (uint32_t value) {
	this->reserved1 = value;
}
void TrackHeaderBox::SetDuration (uint64_t duration) {
	this->duration = duration;
}
void TrackHeaderBox::SetReserved2 (uint32_t value, uint32_t index) {
	if (index < 2) {
		this->reserved2[index] = value;
	}
}
void TrackHeaderBox::SetLayer (uint16_t layer) {
	this->layer = layer;
}
void TrackHeaderBox::SetAlternateGroup (uint16_t group) {
	this->alternate_group = group;
}
void TrackHeaderBox::SetVolume (uint16_t volume) {
	this->volume = volume;
}
void TrackHeaderBox::SetReserved3 (uint16_t value) {
	this->reserved3 = value;
}
void TrackHeaderBox::SetMatrix (uint32_t value, uint32_t index) {
	if (index < 9) {
		this->matrix[index] = value;
	}
}
void TrackHeaderBox::SetWidth (uint32_t width) {
	this->width = width;
}
void TrackHeaderBox::SetHeight (uint32_t height) {
	this->height = height;
}

uint64_t TrackHeaderBox::GetCreationTime () {
	return this->creationTime;
}
uint64_t TrackHeaderBox::GetModificationTime () {
	return this->modificationTime;
}
uint32_t TrackHeaderBox::GetTrackID () {
	return this->trackID;
}
uint32_t TrackHeaderBox::GetReserved1 () {
	return this->reserved1;
}
uint64_t TrackHeaderBox::GetDuration () {
	return this->duration;
}
uint32_t TrackHeaderBox::GetReserved2 (uint32_t index) {
	if (index < 2) {
		return this->reserved2[index];
	}

	return 0;
}
uint16_t TrackHeaderBox::GetLayer () {
	return this->layer;
}
uint16_t TrackHeaderBox::GetAlternateGroup () {
	return this->alternate_group;
}
uint16_t TrackHeaderBox::GetVolume () {
	return this->volume;
}
uint16_t TrackHeaderBox::GetReserved3 () {
	return this->reserved3;
}
uint32_t TrackHeaderBox::GetMatrix (uint32_t index) {
	if (index < 9) {
		return this->matrix[index];
	}

	return 0;
}
uint32_t TrackHeaderBox::GetWidth () {
	return this->width;
}
uint32_t TrackHeaderBox::GetHeight () {
	return this->height;
}

void TrackHeaderBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Track Header Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Creation Time = " << this->creationTime << endl;
	cout << ss.str() << "  Modification Time = " << this->modificationTime << endl;
	cout << ss.str() << "  Track ID = " << this->trackID << endl;
	cout << ss.str() << "  Reserved 1 = " << this->reserved1 << endl;
	cout << ss.str() << "  Duration = " << this->duration << endl;
	cout << ss.str() << "  Reserved 2[0] = " << this->reserved2[0] << endl;
	cout << ss.str() << "  Reserved 2[1] = " << this->reserved2[1] << endl;
	cout << ss.str() << "  Alternate Group = " << this->alternate_group << endl;
	cout << ss.str() << "  Volume = " << this->volume << endl;
	cout << ss.str() << "  Reserved 3 = " << this->reserved3 << endl;
	for (int i = 0; i < 9; i++) {
		cout << ss.str() << "  Matrix[" << i << "] = " << this->matrix[i] << endl;
	}
	cout << ss.str() << "  Width = " << (this->width>>16) << endl;
	cout << ss.str() << "  Height = " << (this->height>>16) << endl;
}