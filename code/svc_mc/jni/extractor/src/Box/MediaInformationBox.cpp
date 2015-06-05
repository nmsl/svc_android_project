#include "MediaInformationBox.h"

using namespace Boxes;
using namespace std;

MediaInformationBox::MediaInformationBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		infoHeader (NULL),
		dataInfo (NULL),
		sampleTable (NULL)
		{}
MediaInformationBox::~MediaInformationBox () {
	if (this->infoHeader) 	delete this->infoHeader;
	if (this->dataInfo) 	delete this->dataInfo;
	if (this->sampleTable) 	delete this->sampleTable;
}

void MediaInformationBox::SetVideoMediaHeaderBox (VideoMediaHeaderBox *vmhdBox) {
	if (!this->infoHeader) this->infoHeader = vmhdBox;
}
void MediaInformationBox::SetDataInformationBox (DataInformationBox *drefBox) {
	if (!this->dataInfo) this->dataInfo = drefBox;
}
void MediaInformationBox::SetSampleTableBox (SampleTableBox *stblBox) {
	if (!this->sampleTable) this->sampleTable = stblBox;
}

VideoMediaHeaderBox* MediaInformationBox::GetVideoMediaHeaderBox () {
	return this->infoHeader;
}
DataInformationBox* MediaInformationBox::GetDataInformationBox () {
	return this->dataInfo;
}
SampleTableBox* MediaInformationBox::GetSampleTableBox () {
	return this->sampleTable;
}

void MediaInformationBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Media Information Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;

	if (this->infoHeader) 	this->infoHeader->Print(level+1);
	if (this->dataInfo) 	this->dataInfo->Print(level+1);
	if (this->sampleTable) 	this->sampleTable->Print(level+1);
}