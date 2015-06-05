#ifndef VIDEO_MEDIA_HEADER_BOX_H
#define VIDEO_MEDIA_HEADER_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class VideoMediaHeaderBox : public BaseBox, public FullBox {
	public:
		VideoMediaHeaderBox (uint32_t size, uint64_t type);
		~VideoMediaHeaderBox ();

		void SetReserved (uint64_t value);

		uint64_t GetReserved ();

		void Print (uint32_t level);

	private:
		uint64_t reserved;
	};
}

#endif /* VIDEO_MEDIA_HEADER_BOX_H */