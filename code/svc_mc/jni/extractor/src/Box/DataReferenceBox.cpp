#include "DataReferenceBox.h"

using namespace Boxes;
using namespace std;

DataReferenceBox::DataReferenceBox (uint32_t size, uint64_t type) :
		BaseBox (size, type)
		{}
DataReferenceBox::~DataReferenceBox () {
	uint32_t size = this->entryList.size();
	for (uint32_t i = 0; i < size; i++) {
		DataEntryURLBox *box = this->entryList.back();
		this->entryList.pop_back();
		if (box) delete box;
	}
}

void DataReferenceBox::AddDataEntryURLBox (DataEntryURLBox *url) {
	this->entryList.push_back(url);
}

DataEntryURLBox* DataReferenceBox::GetDataEntryURLBox (uint32_t index) {
	if (this->entryList.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->entryList.size())
		return this->entryList.at(index);

	return this->entryList.at(0);
}

void DataReferenceBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Data Reference Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;

	for (uint32_t i = 0; i < this->entryList.size(); i++)
		this->entryList.at(i)->Print(level+1);
}