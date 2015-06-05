#include "MediaDataEntry.h"

#include "stdio.h"

using namespace Boxes;
using namespace std;

MediaDataEntry::MediaDataEntry () :
		sampleNumber (0),
		samplesize (0),
		data_offset (0),
		data (NULL)
		{}
MediaDataEntry::~MediaDataEntry () {
	if (this->data) delete[] this->data;
}

void MediaDataEntry::SetNumber (uint32_t no) {
	this->sampleNumber = no;
}
void MediaDataEntry::SetSize (uint32_t size) {
	this->samplesize = size;
}
void MediaDataEntry::SetDataOffset (uint32_t offset) {
	this->data_offset = offset;
}
void MediaDataEntry::SetData (uint8_t *data) {
	this->data = data;
}

uint32_t MediaDataEntry::GetNumber () {
	return this->sampleNumber;
}
uint32_t MediaDataEntry::GetSize () {
	return this->samplesize;
}
uint32_t MediaDataEntry::GetDataOffset () {
	return this->data_offset;
}
uint8_t* MediaDataEntry::GetData() {
	return data;
}

void MediaDataEntry::Print(uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Media Data Entry :" << endl;
	cout << ss.str() << "  Number = " << this->sampleNumber << endl;
	cout << ss.str() << "  Size = " << this->samplesize << endl;
	cout << ss.str() << "  Data Offset = " << this->data_offset << endl;
	// cout << ss.str() << "  Data = ";
	// for (uint32_t i = 0; i < this->samplesize; i++) printf("%.2X ", this->data[i]);
	cout << endl;
}