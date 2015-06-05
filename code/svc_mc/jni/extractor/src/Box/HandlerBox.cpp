#include "HandlerBox.h"

using namespace Boxes;
using namespace std;

HandlerBox::HandlerBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		reserved1 (0),
		handlerType (0),
		nameUTF8 (NULL)
		{}
HandlerBox::~HandlerBox () {
	if (this->nameUTF8) delete[] this->nameUTF8;
}

void HandlerBox::SetReserved1 (uint32_t value) {
	this->reserved1 = value;
}
void HandlerBox::SetHandlerType (uint32_t type) {
	this->handlerType = type;
}
void HandlerBox::SetNameUTF8Size (uint32_t size) {
	this->nameUTF8Size = size;
}
void HandlerBox::SetNameUTF8 (uint8_t *data) {
	this->nameUTF8 = data;
}

uint32_t HandlerBox::GetReserved1 () {
	return this->reserved1;
}
uint32_t HandlerBox::GetHandlerType () {
	return this->handlerType;
}
uint8_t* HandlerBox::GetReserved2 () {
	return this->reserved2;
}
uint8_t* HandlerBox::GetNameUTF8 () {
	return this->nameUTF8;
}

void HandlerBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Handler Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Reserved 1 = " << this->reserved1 << endl;
	cout << ss.str() << "  Handler Type = ";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->handlerType >> i*8);
	cout << endl;
	cout << ss.str() << "  Reserved 2 = " << this->reserved2 << endl;
	cout << ss.str() << "  Name UTF8 = " << this->nameUTF8 << endl;
}