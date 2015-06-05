#ifndef MEDIA_BOX_H
#define MEDIA_BOX_H

#include "BaseBox.h"
#include "MediaHeaderBox.h"
#include "HandlerBox.h"
#include "MediaInformationBox.h"

namespace Boxes {
	class MediaBox : public BaseBox {
	public:
		MediaBox (uint32_t size, uint64_t type);
		~MediaBox ();

		void SetMediaHeaderBox (MediaHeaderBox *mdhdBox);
		void SetHandlerBox (HandlerBox *hdlrBox);
		void SetMediaInformationBox (MediaInformationBox *minfBox);

		MediaHeaderBox* GetMediaHeaderBox ();
		HandlerBox* GetHandlerBox ();
		MediaInformationBox* GetMediaInformationBox ();

		void Print (uint32_t level);

	private:
		MediaHeaderBox *header;
		HandlerBox *handler;
		MediaInformationBox *information;
	};
}

#endif /* MEDIA_BOX_H */