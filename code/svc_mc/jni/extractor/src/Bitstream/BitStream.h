#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "config.h"

namespace Bitstream {

	enum BufferMode {
		READ_MODE = 1,
		WRITE_MODE = 2
	};

	class BitStream {
	public:
		BitStream ();
		BitStream (uint8_t *inData, uint32_t size, BufferMode mode);
		~BitStream ();

		// For Testing
		void SkipBytes(uint32_t v);

		bool IsAvailableRead ();
		uint32_t GetSize ();
		uint8_t* GetData (uint32_t pos);
		uint32_t GetPosition ();
		void Done ();

		/* Read Operation */
		uint32_t ReadBitsToInt (uint32_t n);
		uint32_t ReadU8 ();
		uint32_t ReadU16 ();
		uint32_t ReadU24 ();
		uint32_t ReadU32 ();
		uint64_t ReadU64 ();
		uint32_t ReadData (uint8_t *outData, uint32_t bytes);

		/* Write Operation */
		void WriteIntToBits (int32_t v, int32_t bits);
		void WriteU8 (uint32_t v);
		void WriteU16 (uint32_t v);
		void WriteU32 (uint32_t v);
		void WriteU64 (uint64_t v);
		uint32_t WriteData (uint8_t *inData, uint32_t bytes);

	private:
		uint8_t *data;
		uint32_t size;
		uint32_t position;
		uint32_t bits;
		uint32_t currentValue;
		BufferMode mode;

		void FreeData ();
		bool AllocSpace ();
		bool CopyData (uint8_t *inData);
		bool CheckMode (BufferMode mode);
		void BitAlign ();

		/* Read */
		uint8_t ReadBit ();
		uint32_t ReadByte ();
		/* Write */
		void WriteBit (uint32_t bit);
		void WriteByte(uint8_t v);
	};
}

#endif /* BITSTREAM_H */