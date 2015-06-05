#include "FileTypeBox.h"

using namespace Boxes;
using namespace std;

FileTypeBox::FileTypeBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		majorBrand (0),
		minorVersion (0)
		{}
FileTypeBox::~FileTypeBox () {}

void FileTypeBox::SetMajorBrand (uint32_t brand) {
	this->majorBrand = brand;
}
void FileTypeBox::SetMinorVersion (uint32_t version) {
	this->minorVersion = version;
}
void FileTypeBox::AddAltBrand (uint32_t brand) {
	this->altBrands.push_back(brand);
}

uint32_t FileTypeBox::GetMajorBrand () {
	return this->majorBrand;
}
uint32_t FileTypeBox::GetMinorVersion () {
	return this->minorVersion;
}
uint32_t FileTypeBox::GetAltCount () {
	return this->altBrands.size();
}
uint32_t FileTypeBox::GetAltBrand (uint32_t index) {
	if (GetAltCount() == 0)
		return 0;
	else if (index >= 0 && index < GetAltCount())
		return this->altBrands.at(index);

	return this->altBrands.at(0);
}

void FileTypeBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "File Type Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Major Brand = " << this->majorBrand << endl;
	cout << ss.str() << "  Minor Version = " << this->minorVersion << endl;
	cout << ss.str() << "  Compatible Brands = ";
	for (uint32_t i = 0; i < this->altBrands.size(); i++) {
		uint32_t temp = this->altBrands.at(i);
		for (int i = 3; i >= 0; i--) cout << (uint8_t)(temp >> i*8);
		cout << "; ";
	}
	cout << endl;
}