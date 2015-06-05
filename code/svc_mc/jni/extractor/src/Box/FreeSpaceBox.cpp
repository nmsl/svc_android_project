#include "FreeSpaceBox.h"

using namespace Boxes;
using namespace std;

FreeSpaceBox::FreeSpaceBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		dataSize (0),
		data (NULL)
		{}
FreeSpaceBox::~FreeSpaceBox () {
	if (this->data) delete[] this->data;
}

void FreeSpaceBox::SetDataSize (uint32_t size) {
	this->dataSize = size;
}
void FreeSpaceBox::SetData (uint8_t* data) {
	this->data = data;
}

uint32_t FreeSpaceBox::GetDataSize () {
	return this->dataSize;
}
uint8_t* FreeSpaceBox::GetData() {
	return data;
}

void FreeSpaceBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Free Space Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Detail Information = " << this->data << endl;
}