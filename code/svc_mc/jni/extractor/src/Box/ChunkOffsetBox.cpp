#include "ChunkOffsetBox.h"

using namespace Boxes;
using namespace std;

ChunkOffsetBox::ChunkOffsetBox (uint32_t size, uint64_t type) :
		BaseBox (size, type),
		entry_count (0),
		offsets (NULL)
		{}
ChunkOffsetBox::~ChunkOffsetBox () {
	if (this->offsets) delete[] this->offsets;
}

void ChunkOffsetBox::SetEntryCount (uint32_t count) {
	this->entry_count = count;
}
// void ChunkOffsetBox::AddOffset (uint32_t value) {
// 	this->offsets.push_back(value);
// }
void ChunkOffsetBox::SetOffsets (uint32_t *o) {
	this->offsets = o;
}

uint32_t ChunkOffsetBox::GetEntryCount () {
	return this->entry_count;
}
uint32_t ChunkOffsetBox::GetOffset (uint32_t index) {
	// if (this->offsets.size() == 0)
	// 	return 0;
	// else if (index < this->offsets.size())
	// 	return this->offsets.at(index);

	// return this->offsets.at(0);

	if (index >= 0 && index < this->entry_count) return this->offsets[index];
	return 0;
}

void ChunkOffsetBox::Print(uint32_t level) {
	stringstream ss;
	for (uint32_t i = 0; i < level; i++)
		ss << "  ";

	cout << endl;
	cout << ss.str() << "Chunk Offset Box :" << endl;
	cout << ss.str() << "  Size = " << this->GetSize() << endl;
	cout << ss.str() << "  Type = " << this->GetType() << " (";
	for (int i = 3; i >= 0; i--) cout << (uint8_t)(this->GetType() >> i*8);
	cout << ")" << endl;
	cout << ss.str() << "  Version = " << int(this->GetVersion()) << endl;
	cout << ss.str() << "  Flags = " << this->GetFlags() << endl;
	for (uint32_t i = 0; i < this->entry_count; i++) {
		cout << ss.str() << "    Offset[" << i << "] = " << this->offsets[i] << endl;
	}
}