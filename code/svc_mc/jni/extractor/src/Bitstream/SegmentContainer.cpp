#include "SegmentContainer.h"

using namespace Boxes;
using namespace Bitstream;
using namespace std;

SegmentContainer::SegmentContainer (SegmentType t) :
		type (t),
		TrackID (0),
		sidx (NULL),
		moof (NULL),
		mdat (NULL),
		ftyp (NULL),
		free (NULL),
		moov (NULL)
		{}
SegmentContainer::~SegmentContainer () {
	if (this->sidx) delete this->sidx;
	if (this->moof) delete this->moof;
	if (this->mdat) delete this->mdat;
	if (this->ftyp) delete this->ftyp;
	if (this->free) delete this->free;
	if (this->moov) delete this->moov;
}

SegmentType SegmentContainer::GetType() {
	return this->type;
}
uint32_t SegmentContainer::GetTrackID () {
	return this->TrackID;
}

void SegmentContainer::SetSegmentIndexBox (SegmentIndexBox *sidxBox) {
	if (this->type == DATA_SEGMENT) {
		if (!this->sidx) this->sidx = sidxBox;
	}
}
void SegmentContainer::SetMovieFragmentBox (MovieFragmentBox *moofBox) {
	if (this->type == DATA_SEGMENT) {
		if (!this->moof) this->moof = moofBox;
		TrackFragmentBox *traf = this->moof->GetTrackFragmentBox(0);
		if (!traf) return;

		TrackFragmentHeaderBox *tfhd = traf->GetTrackFragmentHeaderBox();
		if (!tfhd) return;

		this->TrackID = tfhd->GetTrackID();
	}
}
void SegmentContainer::SetMediaDataBox (MediaDataBox *mdatBox) {
	if (this->type == DATA_SEGMENT) {
		if (!this->mdat) this->mdat = mdatBox;
	}
}

SegmentIndexBox* SegmentContainer::GetSegmentIndexBox () {
	if (CheckType(DATA_SEGMENT)) return this->sidx;
	return NULL;
}
MovieFragmentBox* SegmentContainer::GetMovieFragmentBox () {
	if (CheckType(DATA_SEGMENT)) return this->moof;
	return NULL;
}
MediaDataBox* SegmentContainer::GetMediaDataBox() {
	if (CheckType(DATA_SEGMENT)) return this->mdat;
	return NULL;
}

void SegmentContainer::SetFileTypeBox (FileTypeBox *ftypBox) {
	if (this->type == INIT_SEGMENT) {
		if (!this->ftyp) this->ftyp = ftypBox;
	}
}
void SegmentContainer::SetFreeSpaceBox (FreeSpaceBox *freeBox) {
	if (this->type == INIT_SEGMENT) {
		if (!this->free) this->free = freeBox;
	}
}
void SegmentContainer::SetMovieBox (MovieBox *moovBox) {
	if (this->type == INIT_SEGMENT) {
		if (!this->moov) this->moov = moovBox;
	}
}

FileTypeBox* SegmentContainer::GetFileTypeBox() {
	if (CheckType(INIT_SEGMENT)) return this->ftyp;
	return NULL;
}
FreeSpaceBox* SegmentContainer::GetFreeSpaceBox() {
	if (CheckType(INIT_SEGMENT)) return this->free;
	return NULL;
}
MovieBox* SegmentContainer::GetMovieBox() {
	if (CheckType(INIT_SEGMENT)) return this->moov;
	return NULL;
}

bool SegmentContainer::CheckType(SegmentType t) {
	if (this->type != t) {
		cout << "[Error] : Type not Match, will return NULL" << endl;
		return false;
	}

	return true;
}