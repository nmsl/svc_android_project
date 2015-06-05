#ifndef MPEG4_BITRATE_BOX_H
#define MPEG4_BITRATE_BOX_H

#include "BaseBox.h"

namespace Boxes {
	class MPEG4BitrateBox : public BaseBox {
	public:
		MPEG4BitrateBox (uint32_t size, uint64_t type);
		~MPEG4BitrateBox ();

		void SetBufferSizeDB (uint32_t bufferSize);
		void SetMaxBitrate (uint32_t max);
		void SetAvgBitrate (uint32_t avg);

		uint32_t GetBufferSizeDB ();
		uint32_t GetMaxBitrate ();
		uint32_t GetAvgBitrate ();

		void Print (uint32_t level);

	private:
		uint32_t bufferSizeDB;
		uint32_t maxBitrate;
		uint32_t avgBitrate;
	};
}

#endif /* MPEG4_BITRATE_BOX_H */