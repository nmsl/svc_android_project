#ifndef CHUNK_OFFSET_BOX_H
#define CHUNK_OFFSET_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class ChunkOffsetBox : public BaseBox, public FullBox {
	public:
		ChunkOffsetBox (uint32_t size, uint64_t type);
		~ChunkOffsetBox ();

		void SetEntryCount (uint32_t count);
		// void AddOffset (uint32_t value);
		void SetOffsets (uint32_t *o);

		uint32_t GetEntryCount ();
		uint32_t GetOffset (uint32_t index);

		void Print(uint32_t level);

	private:
		uint32_t entry_count;
		// std::vector<uint32_t> offsets;
		uint32_t *offsets;
	};
}

#endif /* CHUNK_OFFSET_BOX_H */