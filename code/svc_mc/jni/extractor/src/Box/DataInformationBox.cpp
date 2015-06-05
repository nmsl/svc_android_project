#include "DataInformationBox.h"

using namespace Boxes;
using namespace std;

DataInformationBox::DataInformationBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		dref (NULL)
		{}
DataInformationBox::~DataInformationBox () {
	if (this->dref) delete this->dref;
}

void DataInformationBox::SetDataReferenceBox (DataReferenceBox *drefBox) {
	if (!this->dref) this->dref = drefBox;
}

DataReferenceBox* DataInformationBox::GetDataReferenceBox () {
	return this->dref;
}

void DataInformationBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Data Information Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;

	if (this->dref) this->dref->Print(level+1);
}