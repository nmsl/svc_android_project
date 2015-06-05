#include "AVCConfigSlot.h"

using namespace Boxes;
using namespace std;

AVCConfigSlot::AVCConfigSlot () :
		data (NULL),
		size (0)
		{}
AVCConfigSlot::~AVCConfigSlot () {
	if (this->data) delete[] data;
}

void AVCConfigSlot::SetDataSize (uint32_t s) {
	this->size = s;
}
void AVCConfigSlot::SetData (uint8_t *data) {
	this->data = data;
}

uint8_t* AVCConfigSlot::GetData() {
	return this->data;
}
uint32_t AVCConfigSlot::GetSize () {
	return this->size;
}

void AVCConfigSlot::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "AVC Configuration Slot :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
}