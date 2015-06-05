#include "SampleSizeBox.h"

using namespace Boxes;
using namespace std;

SampleSizeBox::SampleSizeBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		sampleSize (0),
		sampleCount (0),
		sizes (NULL)
		{}
SampleSizeBox::~SampleSizeBox () {
	if (this->sizes) delete[] this->sizes;
}

void SampleSizeBox::SetSampleSize (uint32_t size) {
	this->sampleSize = size;
}
void SampleSizeBox::SetSampleCount (uint32_t count) {
	this->sampleCount = count;
}
// void SampleSizeBox::AddSizeValue (uint32_t value) {
// 	this->sizes.push_back(value);
// }
void SampleSizeBox::SetSizeValues (uint32_t *values) {
	this->sizes = values;
}

uint32_t SampleSizeBox::GetSampleSize () {
	return this->sampleSize;
}
uint32_t SampleSizeBox::GetSampleCount () {
	return this->sampleCount;
}
uint32_t SampleSizeBox::GetSizeValue (uint32_t index) {
	// if (this->sizes.size() == 0)
	// 	return 0;
	// else if (index < this->sizes.size())
	// 	return this->sizes.at(index);

	// return this->sizes.at(0);

	if (index >= 0 && index < this->sampleCount) return this->sizes[index];
	return 0;
}

void SampleSizeBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Sample Table Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Sample Size = " << this->sampleSize << endl;
	cout << ss.str() << "  Sample Count = " << this->sampleCount << endl;
	for (uint32_t i = 0; i < this->sampleCount; i++) {
		cout << ss.str() << "    Size[" << i << "] = " << this->sizes[i] << endl;
	}
}