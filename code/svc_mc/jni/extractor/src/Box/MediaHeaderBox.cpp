#include "MediaHeaderBox.h"

using namespace Boxes;
using namespace std;

MediaHeaderBox::MediaHeaderBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		creationTime (0),
		modificationTime (0),
		timeScale (0),
		duration (0),
		reserved (0)
		{}
MediaHeaderBox::~MediaHeaderBox () {}

void MediaHeaderBox::SetCreationTime (uint64_t time) {
	this->creationTime = time;
}
void MediaHeaderBox::SetModificationTime (uint64_t time) {
	this->modificationTime = time;
}
void MediaHeaderBox::SetTimeScale (uint32_t scale) {
	this->timeScale = scale;
}
void MediaHeaderBox::SetDuration (uint64_t duration) {
	this->duration = duration;
}
void MediaHeaderBox::SetLanguage (uint8_t c, uint32_t index) {
	if (index < 3)
		this->packedLanguage[index] = c;
}
void MediaHeaderBox::SetReserved (uint16_t value) {
	this->reserved = value;
}
void MediaHeaderBox::CheckpackedLanguage () {
	if (this->packedLanguage[0] || this->packedLanguage[1] || this->packedLanguage[2]) {
		this->packedLanguage[0] += 0x60;
		this->packedLanguage[1] += 0x60;
		this->packedLanguage[2] += 0x60;
	}
	else {
		this->packedLanguage[0] = 'u';
		this->packedLanguage[1] = 'n';
		this->packedLanguage[2] = 'd';
	}
}

uint64_t MediaHeaderBox::GetCreationTime () {
	return this->creationTime;
}
uint64_t MediaHeaderBox::GetModificationTime () {
	return this->modificationTime;
}
uint32_t MediaHeaderBox::GetTimeScale () {
	return this->timeScale;
}
uint64_t MediaHeaderBox::GetDuration () {
	return this->duration;
}
uint8_t MediaHeaderBox::GetLanguage (uint32_t index) {
	if (index < 3)
		return this->packedLanguage[index];

	return 0;
}
uint16_t MediaHeaderBox::GetReserved () {
	return this->reserved;
}

void MediaHeaderBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Media Header Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Creation Time = " << this->creationTime << endl;
	cout << ss.str() << "  Modification Time = " << this->modificationTime << endl;
	cout << ss.str() << "  Time Scale = " << this->timeScale << endl;
	cout << ss.str() << "  Duration = " << this->duration << endl;
	cout << ss.str() << "  Packed Language = " << this->packedLanguage[0] << this->packedLanguage[1] << this->packedLanguage[2] << endl;
	cout << ss.str() << "  Reserved = " << this->reserved << endl;
}