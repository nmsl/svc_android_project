#include "AVCConfigurationBox.h"

using namespace Boxes;
using namespace std;

AVCConfigurationBox::AVCConfigurationBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		configurationVersion (0),
		AVCProfileIndication (0),
		profile_compatibility (0),
		AVCLevelIndication (0),
		complete_representation (0),
		nal_unit_size (0)
		{}
AVCConfigurationBox::~AVCConfigurationBox () {
	uint32_t size = this->sequenceParameterSets.size();
	for (uint32_t i = 0; i < size; i++) {
		AVCConfigSlot *slot = this->sequenceParameterSets.back();
		this->sequenceParameterSets.pop_back();
		if (slot) delete slot;
	}

	size = this->pictureParameterSets.size();
	for (uint32_t i = 0; i < size; i++) {
		AVCConfigSlot *slot = this->pictureParameterSets.back();
		this->pictureParameterSets.pop_back();
		if (slot) delete slot;
	}
}

void AVCConfigurationBox::SetConfigurationVersion (uint8_t v) {
	this->configurationVersion = v;
}
void AVCConfigurationBox::SetAVCProfileIndication (uint8_t i) {
	this->AVCProfileIndication = i;
}
void AVCConfigurationBox::SetProfileCompatiblitiy (uint8_t p) {
	this->profile_compatibility = p;
}
void AVCConfigurationBox::SetAVCLevelIndication (uint8_t i) {
	this->AVCLevelIndication = i;
}
void AVCConfigurationBox::SetCompleteRepresentation (uint32_t c) {
	this->complete_representation = c;
}
void AVCConfigurationBox::SetNALUintSize (uint32_t size) {
	this->nal_unit_size = size;
}
void AVCConfigurationBox::AddSequenceParameterSet (AVCConfigSlot *slot) {
	this->sequenceParameterSets.push_back(slot);
}
void AVCConfigurationBox::AddPictureParameterSet (AVCConfigSlot *slot) {
	this->pictureParameterSets.push_back(slot);
}

uint8_t AVCConfigurationBox::GetConfigurationVersion () {
	return this->configurationVersion;
}
uint8_t AVCConfigurationBox::GetAVCProfileIndication () {
	return this->AVCProfileIndication;
}
uint8_t AVCConfigurationBox::GetProfileCompatiblitiy () {
	return this->profile_compatibility;
}
uint8_t AVCConfigurationBox::GetAVCLevelIndication () {
	return this->AVCLevelIndication;
}
uint32_t AVCConfigurationBox::GetCompleteRepresentation () {
	return this->complete_representation;
}
uint32_t AVCConfigurationBox::GetNALUintSize () {
	return this->nal_unit_size;
}
AVCConfigSlot* AVCConfigurationBox::GetSequenceParameterSet (uint32_t index) {
	if (this->sequenceParameterSets.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->sequenceParameterSets.size())
		return this->sequenceParameterSets.at(index);

	return this->sequenceParameterSets.at(0);
}
AVCConfigSlot* AVCConfigurationBox::GetPictureParameterSet (uint32_t index) {
	if (this->pictureParameterSets.size() == 0)
		return NULL;
	else if (index >= 0 && index < this->pictureParameterSets.size())
		return this->pictureParameterSets.at(index);

	return this->pictureParameterSets.at(0);
}

void AVCConfigurationBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "AVC Configuration Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Configuration Version = " << int(this->configurationVersion) << endl;
	cout << ss.str() << "  AVC Profile Indication = " << int(this->AVCProfileIndication) << endl;
	cout << ss.str() << "  Profile Compatibility = " << int(this->profile_compatibility) << endl;
	cout << ss.str() << "  AVC Level Indication = " << int(this->AVCLevelIndication) << endl;
	cout << ss.str() << "  Complete Representation = " << this->complete_representation << endl;
	cout << ss.str() << "  Nal Unit Size = " << this->nal_unit_size << endl;
	for (uint32_t i = 0; i < this->sequenceParameterSets.size(); i++) {
		this->sequenceParameterSets.at(i)->Print(level+1);
	}
	for (uint32_t i = 0; i < this->pictureParameterSets.size(); i++) {
		this->pictureParameterSets.at(i)->Print(level+1);
	}
}