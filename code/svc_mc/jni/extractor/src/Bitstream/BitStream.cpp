#include "BitStream.h"

#include <android/log.h>
#define  LOG_TAG    "Bitstream::BitStream"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace Bitstream;
using namespace std;

BitStream::BitStream () :
		data (NULL),
		size (4096),
		position (0),
		bits (0),
		currentValue (0),
		mode (WRITE_MODE)
{
	AllocSpace();
}
BitStream::BitStream (uint8_t *inData, uint32_t s, BufferMode mode) :
		data (NULL),
		size (s),
		position (0),
		bits (8),
		currentValue (0),
		mode (mode)
{
	if (inData == NULL) {
		AllocSpace();
	}
	else {
		CopyData(inData);
	}
}
BitStream::~BitStream () {
	FreeData();
}

void BitStream::SkipBytes(uint32_t v) {
	this->position += v;
}

bool BitStream::IsAvailableRead () {
	if (this->mode == WRITE_MODE) return false;

	return (this->position < this->size);
}
uint32_t BitStream::GetSize () {
	return this->size;
}
uint8_t* BitStream::GetData (uint32_t pos = 0) {
	if (pos > this->size) return NULL;

	return (this->data + pos);
}
uint32_t BitStream::GetPosition () {
	return this->position;
}
void BitStream::Done () {
	this->size = this->position;
	this->position = 0;
}

uint32_t BitStream::ReadBitsToInt (uint32_t n) {
	uint32_t ret = 0;
	if (!CheckMode(READ_MODE)) return 0;

	while (n-- > 0) {
		ret <<= 1;
		ret |= ReadBit();
	}

	return ret;
}
uint32_t BitStream::ReadU8 () {
	if (!CheckMode(READ_MODE)) return 0;
	BitAlign();

	return (uint32_t) ReadByte();
}
uint32_t BitStream::ReadU16 () {
	uint32_t ret = 0;

	if (!CheckMode(READ_MODE)) return 0;
	BitAlign();

	ret = ReadByte(); ret <<= 8;
	ret |= ReadByte();
	return ret;
}
uint32_t BitStream::ReadU24 () {
	uint32_t ret = 0;

	if (!CheckMode(READ_MODE)) return 0;
	BitAlign();

	ret |= ReadByte(); ret <<= 8;
	ret |= ReadByte(); ret <<= 8;
	ret |= ReadByte();
	return ret;
}
uint32_t BitStream::ReadU32 () {
	uint32_t ret = 0;

	if (!CheckMode(READ_MODE)) return 0;
	BitAlign();

	ret = ReadByte(); ret <<= 8;
	ret |= ReadByte(); ret <<= 8;
	ret |= ReadByte(); ret <<= 8;
	ret |= ReadByte();
	return ret;
}
uint64_t BitStream::ReadU64 () {
	uint64_t ret = 0;

	if (!CheckMode(READ_MODE)) return 0;
	BitAlign();

	ret = ReadU32(); ret <<= 32;
	ret |= ReadU32();
	return ret;
}
uint32_t BitStream::ReadData (uint8_t *outData, uint32_t bytes) {
	uint32_t orig = this->position;

	if (outData == NULL) return 0;
	if (!CheckMode(READ_MODE)) return 0;
	if (this->position + bytes > this->size) return 0;
	BitAlign();

	while (bytes-- > 0) {
		*outData++ = ReadBitsToInt(8);
	}

	return (this->position - orig);
}

void BitStream::WriteIntToBits (int32_t v, int32_t bits) {
	if (!CheckMode(WRITE_MODE)) return;

	v <<= sizeof(int32_t)*8 - bits;
	while (--bits >= 0) {
		WriteBit(v < 0);
		v <<= 1;
	}
}
void BitStream::WriteU8 (uint32_t v) {
	if (!CheckMode(WRITE_MODE)) return;
	BitAlign();

	WriteByte((uint8_t) v);
}
void BitStream::WriteU16 (uint32_t v) {
	if (!CheckMode(WRITE_MODE)) return;
	BitAlign();

	WriteByte((uint8_t) ((v>>8)&0xff));
	WriteByte((uint8_t) ((v)&0xff));
}
void BitStream::WriteU32 (uint32_t v) {
	if (!CheckMode(WRITE_MODE)) return;
	BitAlign();

	WriteByte((uint8_t) ((v>>24)&0xff));
	WriteByte((uint8_t) ((v>>16)&0xff));
	WriteByte((uint8_t) ((v>>8)&0xff));
	WriteByte((uint8_t) ((v)&0xff));
}
void BitStream::WriteU64 (uint64_t v) {
	if (!CheckMode(WRITE_MODE)) return;
	BitAlign();

	WriteU32((uint32_t) ((v>>32)&0xffffffff));
	WriteU32((uint32_t) ((v)&0xffffffff));
}
uint32_t BitStream::WriteData (uint8_t *inData, uint32_t bytes) {
	uint32_t newSize = 0;

	if (inData == NULL) return 0;
	if (!CheckMode(WRITE_MODE)) return 0;

	BitAlign();
	if (this->position + bytes > this->size) {
		newSize = (uint32_t) (this->size * 2);
		while (newSize < (uint32_t) (this->size + bytes)) {
			newSize *= 2;
		}
		this->data = (uint8_t*) realloc(this->data, newSize);
		this->size = newSize;
	}

	memcpy(this->data + this->position, inData, bytes);
	this->position += bytes;

	return bytes;
}

void BitStream::FreeData () {
	if (this->data) free(this->data);
	this->currentValue = this->position = this->size = 0;
	this->bits = (this->mode == READ_MODE) ? 8 : 0;
}
bool BitStream::AllocSpace () {
	this->data = (uint8_t*) calloc(this->size, sizeof(uint8_t));

	return (this->data == NULL) ? false : true;
}
bool BitStream::CopyData (uint8_t *inData) {
	if (!AllocSpace()) return false;

	if (this->data) {
		memcpy(this->data, inData, size);
		return true;
	}

	return false;
}
bool BitStream::CheckMode (BufferMode mode) {
	if (this->mode != mode) {
		LOGI("Mode Not Match");
		// cout << "Mode not Match" << endl;
		return false;
	}

	return true;
}
void BitStream::BitAlign () {
	if (this->mode == READ_MODE && this->bits != 8) {
		ReadBitsToInt(8 - this->bits);
	}
	else if (this->mode == WRITE_MODE && this->bits != 0) {
		WriteIntToBits(0, 8 - this->bits);
	}
}
uint8_t BitStream::ReadBit() {
	if (this->bits == 8) {
		this->currentValue = ReadByte();
		this->bits = 0;
	}

	int ret;
	this->currentValue <<= 1;
	this->bits++;
	ret = (this->currentValue & 0x100) >> 8;
	return (uint8_t) ret;
}
uint32_t BitStream::ReadByte () {
	if (this->position >= this->size)
		return 0;
	return (uint32_t) this->data[this->position++];
}
void BitStream::WriteBit (uint32_t bit) {
	this->currentValue <<= 1;
	this->currentValue |= bit;

	if (++this->bits == 8) {
		this->bits = 0;
		WriteByte((uint8_t) this->currentValue);
		this->currentValue = 0;
	}
}
void BitStream::WriteByte(uint8_t v) {
	if (this->position == this->size) {
		this->size *= 2;
		this->data = (uint8_t*) realloc(this->data, this->size);
	}

	this->data[this->position] = v;
	this->position++;
}