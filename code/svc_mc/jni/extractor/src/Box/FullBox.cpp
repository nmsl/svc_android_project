#include "FullBox.h"

using namespace Boxes;

FullBox::FullBox () : version (0), flags(0) {}
FullBox::~FullBox () {}

void FullBox::SetVersion (uint8_t v) {
	this->version = v;
}
void FullBox::SetFlags (uint32_t f) {
	this->flags = f;
}

uint8_t FullBox::GetVersion () {
	return this->version;
}
uint32_t FullBox::GetFlags () {
	return this->flags;
}