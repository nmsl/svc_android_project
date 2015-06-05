#include "MPEGVisualSampleEntryBox.h"

using namespace Boxes;
using namespace std;

MPEGVisualSampleEntryBox::MPEGVisualSampleEntryBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		dataReferenceIndex (0),
		version (0),
		revision (0),
		vendor (0),
		temporal_quality (0),
		spatial_quality (0),
		width (0),
		height (0),
		horiz_res (0),
		vert_res (0),
		entry_data_size (0),
		frames_per_sample (0),
		bit_depth (0),
		color_table_index (0),
		avc_config (NULL),
		svc_config (NULL),
		bitrate (NULL)
		{}
MPEGVisualSampleEntryBox::~MPEGVisualSampleEntryBox () {
	if (this->avc_config) delete this->avc_config;
	if (this->svc_config) delete this->svc_config;
	if (this->bitrate) delete this->bitrate;
}

void MPEGVisualSampleEntryBox::SetDataReferenceIndex (uint16_t index) {
	this->dataReferenceIndex = index;
}
void MPEGVisualSampleEntryBox::SetVersion (uint16_t v) {
	this->version = v;
}
void MPEGVisualSampleEntryBox::SetRevision (uint16_t r) {
	this->revision = r;
}
void MPEGVisualSampleEntryBox::SetVendor (uint32_t vendor) {
	this->vendor = vendor;
}
void MPEGVisualSampleEntryBox::SetTemporal (uint32_t t) {
	this->temporal_quality = t;
}
void MPEGVisualSampleEntryBox::SetSpatial (uint32_t s) {
	this->spatial_quality = s;
}
void MPEGVisualSampleEntryBox::SetWidth (uint16_t w) {
	this->width = w;
}
void MPEGVisualSampleEntryBox::SetHeight (uint16_t h) {
	this->height = h;
}
void MPEGVisualSampleEntryBox::SetHorizResolution (uint32_t res) {
	this->horiz_res = res;
}
void MPEGVisualSampleEntryBox::SetVertResolution (uint32_t res) {
	this->vert_res = res;
}
void MPEGVisualSampleEntryBox::SetEntryDataSize (uint32_t size) {
	this->entry_data_size = size;
}
void MPEGVisualSampleEntryBox::SetFramePerSample (uint16_t fps) {
	this->frames_per_sample = fps;
}
void MPEGVisualSampleEntryBox::SetBitDepth (uint16_t depth) {
	this->bit_depth = depth;
}
void MPEGVisualSampleEntryBox::SetColorTableIndex (uint16_t index) {
	this->color_table_index = index;
}
void MPEGVisualSampleEntryBox::SetAVCConfig (AVCConfigurationBox *conf) {
	if (!this->avc_config) this->avc_config = conf;
}
void MPEGVisualSampleEntryBox::SetSVCConfig (AVCConfigurationBox *conf) {
	if (!this->svc_config) this->svc_config = conf;
}
void MPEGVisualSampleEntryBox::SetBitrateBox (MPEG4BitrateBox *btrtBox) {
	if (!this->bitrate) this->bitrate = btrtBox;
}

uint8_t* MPEGVisualSampleEntryBox::GetReserved () {
	return this->reserved;
}
uint16_t MPEGVisualSampleEntryBox::GetDataReferenceIndex () {
	return this->dataReferenceIndex;
}
uint16_t MPEGVisualSampleEntryBox::GetVersion () {
	return this->version;
}
uint16_t MPEGVisualSampleEntryBox::GetRevision () {
	return this->revision;
}
uint32_t MPEGVisualSampleEntryBox::GetVendor () {
	return this->vendor;
}
uint32_t MPEGVisualSampleEntryBox::GetTemporal () {
	return this->temporal_quality;
}
uint32_t MPEGVisualSampleEntryBox::GetSpatial () {
	return this->spatial_quality;
}
uint16_t MPEGVisualSampleEntryBox::GetWidth () {
	return this->width;
}
uint16_t MPEGVisualSampleEntryBox::GetHeight () {
	return this->height;
}
uint32_t MPEGVisualSampleEntryBox::GetHorizResolution () {
	return this->horiz_res;
}
uint32_t MPEGVisualSampleEntryBox::GetVertResolution () {
	return this->vert_res;
}
uint32_t MPEGVisualSampleEntryBox::GetEntryDataSize () {
	return this->entry_data_size;
}
uint16_t MPEGVisualSampleEntryBox::GetFramePerSample () {
	return this->frames_per_sample;
}
uint8_t* MPEGVisualSampleEntryBox::GetCompressorName () {
	return this->compressor_name;
}
uint16_t MPEGVisualSampleEntryBox::GetBitDepth () {
	return this->bit_depth;
}
uint16_t MPEGVisualSampleEntryBox::GetColorTableIndex () {
	return this->color_table_index;
}
AVCConfigurationBox* MPEGVisualSampleEntryBox::GetAVCConfig () {
	return this->avc_config;
}
AVCConfigurationBox* MPEGVisualSampleEntryBox::GetSVCConfig () {
	return this->svc_config;
}
MPEG4BitrateBox* MPEGVisualSampleEntryBox::GetBitrateBox () {
	return this->bitrate;
}

void MPEGVisualSampleEntryBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "MPEG Visual Sample Entry Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Reserved = " << this->reserved << endl;
	cout << ss.str() << "  Data Reference Index = " << this->dataReferenceIndex << endl;
	cout << ss.str() << "  Version = " << this->version << endl;
	cout << ss.str() << "  Revision = " << this->revision << endl;
	cout << ss.str() << "  Vendor = " << this->vendor << endl;
	cout << ss.str() << "  Temporal Quality = " << this->temporal_quality << endl;
	cout << ss.str() << "  Spatial Quality = " << this->spatial_quality << endl;
	cout << ss.str() << "  Width = " << this->width << endl;
	cout << ss.str() << "  Height = " << this->height << endl;
	cout << ss.str() << "  Horiz Resolution = " << this->horiz_res << endl;
	cout << ss.str() << "  Vert Resolution = " << this->vert_res << endl;
	cout << ss.str() << "  Entry Data Size = " << this->entry_data_size << endl;
	cout << ss.str() << "  Frames Per Sample = " << this->frames_per_sample << endl;
	cout << ss.str() << "  Compressor Name = " << this->compressor_name << endl;
	cout << ss.str() << "  Bit Depth = " << this->bit_depth << endl;
	cout << ss.str() << "  Color Table Index = " << this->color_table_index << endl;

	if (this->avc_config) this->avc_config->Print(level+1);
	if (this->svc_config) this->svc_config->Print(level+1);
	if (this->bitrate) this->bitrate->Print(level+1);
}