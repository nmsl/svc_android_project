#include "DataEntryURLBox.h"

using namespace Boxes;
using namespace std;

DataEntryURLBox::DataEntryURLBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		location (NULL),
		charCount (0)
		{}
DataEntryURLBox::~DataEntryURLBox () {
	if (this->location) delete[] this->location;
}

void DataEntryURLBox::SetCharCount (uint32_t count) {
	this->charCount = count;
}
void DataEntryURLBox::SetLocation (uint8_t *data) {
	this->location = data;
}

uint8_t* DataEntryURLBox::GetLocation () {
	return this->location;
}
uint32_t DataEntryURLBox::GetCharCount () {
	return this->charCount;
}

void DataEntryURLBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Data Entry URL Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Location = " << this->location << endl;
}