#include "SIDXBoxRef.h"

using namespace Boxes;
using namespace std;

SIDXBoxRef::SIDXBoxRef () :
		reference_type (false),
		reference_size (0),
		subsegment_duration (0),
		start_with_SAP (false),
		SAP_type (0),
		SAP_delta_time (0)
		{}
SIDXBoxRef::~SIDXBoxRef () {}

void SIDXBoxRef::SetType (bool t) {
	this->reference_type = t;
}
void SIDXBoxRef::SetSize (uint32_t s) {
	this->reference_size = s;
}
void SIDXBoxRef::SetDuration (uint32_t d) {
	this->subsegment_duration = d;
}
void SIDXBoxRef::SetSAP (bool isSAP) {
	this->start_with_SAP = isSAP;
}
void SIDXBoxRef::SetSAPType (uint32_t SAPtype) {
	this->SAP_type = SAPtype;
}
void SIDXBoxRef::SetSAPDeltaTime (uint32_t SAPtime) {
	this->SAP_delta_time = SAPtime;
}

bool SIDXBoxRef::GetType () {
	return this->reference_type;
}
uint32_t SIDXBoxRef::GetSize () {
	return this->reference_size;
}
uint32_t SIDXBoxRef::GetDuration () {
	return this->subsegment_duration;
}
bool SIDXBoxRef::IsSAP () {
	return this->start_with_SAP;
}
uint32_t SIDXBoxRef::GetSAPType () {
	return this->SAP_type;
}
uint32_t SIDXBoxRef::GetSAPDeltaTime () {
	return this->SAP_delta_time;
}

void SIDXBoxRef::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Reference Info. :" << endl;
	cout << ss.str() << "  Type = " << this->reference_type << endl;
	cout << ss.str() << "  Size = " << this->reference_size << endl;
	cout << ss.str() << "  Subsegment Duration = " << this->subsegment_duration << endl;
	cout << ss.str() << "  Start With SAP = " << this->start_with_SAP << endl;
	cout << ss.str() << "  SAP Type = " << this->SAP_type << endl;
	cout << ss.str() << "  SAP Delta Time = " << this->SAP_delta_time << endl;
}