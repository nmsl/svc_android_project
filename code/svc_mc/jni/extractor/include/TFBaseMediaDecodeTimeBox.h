#ifndef TRACK_FRAGMENT_BASE_MEDIA_DECODE_TIME_BOX_H
#define TRACK_FRAGMENT_BASE_MEDIA_DECODE_TIME_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class TFBaseMediaDecodeTimeBox : public BaseBox, public FullBox {
	public:
		TFBaseMediaDecodeTimeBox (uint32_t size, uint64_t type);
		~TFBaseMediaDecodeTimeBox ();

		void SetMediaDecodeTime (uint64_t time);

		uint64_t GetMediaDecodeTime ();

		void Print(uint32_t level);

	private:
		uint64_t baseMediaDecodeTime;
	};
}

#endif /* TRACK_FRAGMENT_BASE_MEDIA_DECODE_TIME_BOX_H */