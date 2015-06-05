#include "MediaDataBox.h"

using namespace Boxes;
using namespace std;

MediaDataBox::MediaDataBox (uint32_t size, uint64_t type) :
		BaseBox(size, type),
		refTRUN (NULL)
		{}
MediaDataBox::~MediaDataBox () {
	// if (this->refTRUN) delete this->refTRUN;

	uint32_t size = this->dataList.size();
	for (uint32_t i = 0; i < size; i++) {
		MediaDataEntry *entry = this->dataList.back();
		this->dataList.pop_back();
		if (entry) delete entry;
	}
}

void MediaDataBox::SetRefTRUN (TrackFragmentRunBox *trun) {
	this->refTRUN = trun;
}
void MediaDataBox::AddMediaData (MediaDataEntry* sample) {
	this->dataList.push_back(sample);
}

TrackFragmentRunBox* MediaDataBox::GetRefTRUN () {
	return this->refTRUN;
}
MediaDataEntry* MediaDataBox::GetMediaData (uint32_t index) {
	if (this->dataList.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->dataList.size())
		return this->dataList.at(index);

	return this->dataList.at(0);
}

void MediaDataBox::Print(uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Media Data Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	for (uint32_t i = 0; i < this->dataList.size(); i++) {
		this->dataList.at(i)->Print(level+1);
	}
}