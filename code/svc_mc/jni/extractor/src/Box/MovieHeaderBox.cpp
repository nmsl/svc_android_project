#include "MovieHeaderBox.h"

using namespace Boxes;
using namespace std;

MovieHeaderBox::MovieHeaderBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		creationTime (0),
		modificationTime (0),
		timeScale (600),
		duration (0),
		preferredRate (0),
		preferredVolume (0),
		matrixA (0),
		matrixB (0),
		matrixU (0),
		matrixC (0),
		matrixD (0),
		matrixV (0),
		matrixX (0),
		matrixY (0),
		matrixW (0),
		previewTime (0),
		previewDuration (0),
		posterTime (0),
		selectionTime (0),
		selectionDuration (0),
		currentTime (0),
		nextTrackID (0)
		{}
MovieHeaderBox::~MovieHeaderBox () {}

void MovieHeaderBox::SetCreationTime (uint64_t cTime) {
	this->creationTime = cTime;
}
void MovieHeaderBox::SetModificationTime (uint64_t mTime) {
	this->modificationTime = mTime;
}
void MovieHeaderBox::SetTimeScale (uint32_t sTime) {
	if (!sTime)
		this->timeScale = sTime;
}
void MovieHeaderBox::SetDuration (uint64_t duration) {
	this->duration = duration;
}
void MovieHeaderBox::SetPreferredRate (uint32_t rate) {
	this->preferredRate = rate;
}
void MovieHeaderBox::SetPreferredVolume (uint16_t volume) {
	this->preferredVolume = volume;
}
void MovieHeaderBox::SetMatrixA (uint32_t a) {
	this->matrixA = a;
}
void MovieHeaderBox::SetMatrixB (uint32_t b) {
	this->matrixB = b;
}
void MovieHeaderBox::SetMatrixU (uint32_t u) {
	this->matrixU = u;
}
void MovieHeaderBox::SetMatrixC (uint32_t c) {
	this->matrixC = c;
}
void MovieHeaderBox::SetMatrixD (uint32_t d) {
	this->matrixD = d;
}
void MovieHeaderBox::SetMatrixV (uint32_t v) {
	this->matrixV = v;
}
void MovieHeaderBox::SetMatrixX (uint32_t x) {
	this->matrixX = x;
}
void MovieHeaderBox::SetMatrixY (uint32_t y) {
	this->matrixY = y;
}
void MovieHeaderBox::SetMatrixW (uint32_t w) {
	this->matrixW = w;
}
void MovieHeaderBox::SetPreviewTime (uint32_t pTime) {
	this->previewTime = pTime;
}
void MovieHeaderBox::SetPreviewDuration (uint32_t pDuration) {
	this->previewDuration = pDuration;
}
void MovieHeaderBox::SetPosterTime (uint32_t posterT) {
	this->posterTime = posterT;
}
void MovieHeaderBox::SetSelectionTime (uint32_t selectionT) {
	this->selectionTime = selectionT;
}
void MovieHeaderBox::SetSelectionDuration (uint32_t selectionD) {
	this->selectionDuration = selectionD;
}
void MovieHeaderBox::SetCurrentTime (uint32_t currentT) {
	this->currentTime = currentT;
}
void MovieHeaderBox::SetNextTrackID (uint32_t id) {
	this->nextTrackID = id;
}

uint64_t MovieHeaderBox::GetCreationTime () {
	return this->creationTime;
}
uint64_t MovieHeaderBox::GetModificationTime () {
	return this->modificationTime;
}
uint32_t MovieHeaderBox::GetTimeScale () {
	return this->timeScale;
}
uint64_t MovieHeaderBox::GetDuration () {
	return this->duration;
}
uint32_t MovieHeaderBox::GetPreferredRate () {
	return this->preferredRate;
}
uint16_t MovieHeaderBox::GetPreferredVolume () {
	return this->preferredVolume;
}
uint8_t* MovieHeaderBox::GetReserved () {
	return this->reserved;
}
uint32_t MovieHeaderBox::GetMatrixA () {
	return this->matrixA;
}
uint32_t MovieHeaderBox::GetMatrixB () {
	return this->matrixB;
}
uint32_t MovieHeaderBox::GetMatrixU () {
	return this->matrixU;
}
uint32_t MovieHeaderBox::GetMatrixC () {
	return this->matrixC;
}
uint32_t MovieHeaderBox::GetMatrixD () {
	return this->matrixD;
}
uint32_t MovieHeaderBox::GetMatrixV () {
	return this->matrixV;
}
uint32_t MovieHeaderBox::GetMatrixX () {
	return this->matrixX;
}
uint32_t MovieHeaderBox::GetMatrixY () {
	return this->matrixY;
}
uint32_t MovieHeaderBox::GetMatrixW () {
	return this->matrixW;
}
uint32_t MovieHeaderBox::GetPreviewTime () {
	return this->previewTime;
}
uint32_t MovieHeaderBox::GetPreviewDuration () {
	return this->previewDuration;
}
uint32_t MovieHeaderBox::GetPosterTime () {
	return this->posterTime;
}
uint32_t MovieHeaderBox::GetSelectionTime () {
	return this->selectionTime;
}
uint32_t MovieHeaderBox::GetSelectionDuration () {
	return this->selectionDuration;
}
uint32_t MovieHeaderBox::GetCurrentTime () {
	return this->currentTime;
}
uint32_t MovieHeaderBox::GetNextTrackID () {
	return this->nextTrackID;
}

void MovieHeaderBox::Print (uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Movie Header Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	cout << ss.str() << "  Creation Time = " << this->creationTime << endl;
	cout << ss.str() << "  Modification Time = " << this->modificationTime << endl;
	cout << ss.str() << "  Time Scale = " << this->timeScale << endl;
	cout << ss.str() << "  Duration = " << this->duration << endl;
	cout << ss.str() << "  Rate = " << (this->preferredRate >> 16) << endl;
	cout << ss.str() << "  Volume = " << (this->preferredVolume >> 8) << endl;
	cout << ss.str() << "  MatrixA = " << this->matrixA << endl;
	cout << ss.str() << "  MatrixB = " << this->matrixB << endl;
	cout << ss.str() << "  MatrixU = " << this->matrixU << endl;
	cout << ss.str() << "  MatrixC = " << this->matrixC << endl;
	cout << ss.str() << "  MatrixD = " << this->matrixD << endl;
	cout << ss.str() << "  MatrixV = " << this->matrixV << endl;
	cout << ss.str() << "  MatrixX = " << this->matrixX << endl;
	cout << ss.str() << "  MatrixY = " << this->matrixY << endl;
	cout << ss.str() << "  MatrixW = " << this->matrixW << endl;
	cout << ss.str() << "  Preview Time = " << this->previewTime << endl;
	cout << ss.str() << "  Preview Duration = " << this->previewDuration << endl;
	cout << ss.str() << "  Poster Time = " << this->posterTime << endl;
	cout << ss.str() << "  Selection Time = " << this->selectionTime << endl;
	cout << ss.str() << "  Selection Duration = " << this->selectionDuration << endl;
	cout << ss.str() << "  Current Time = " << this->currentTime << endl;
	cout << ss.str() << "  Next Track ID = " << this->nextTrackID << endl;
}