#include "MediaBox.h"

using namespace Boxes;
using namespace std;

MediaBox::MediaBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		header (NULL),
		handler (NULL),
		information (NULL)
		{}
MediaBox::~MediaBox () {
	if (this->header) delete this->header;
	if (this->handler) delete this->handler;
	if (this->information) delete this->information;
}

void MediaBox::SetMediaHeaderBox (MediaHeaderBox *mdhdBox) {
	if (!this->header) this->header = mdhdBox;
}
void MediaBox::SetHandlerBox (HandlerBox *hdlrBox) {
	if (!this->handler) this->handler = hdlrBox;
}
void MediaBox::SetMediaInformationBox (MediaInformationBox *minfBox) {
	if (!this->information) this->information = minfBox;
}

MediaHeaderBox* MediaBox::GetMediaHeaderBox () {
	return this->header;
}
HandlerBox* MediaBox::GetHandlerBox () {
	return this->handler;
}
MediaInformationBox* MediaBox::GetMediaInformationBox () {
	return this->information;
}

void MediaBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Media Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;

	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;

	this->header->Print(level+1);
	this->handler->Print(level+1);
	this->information->Print(level+1);
}