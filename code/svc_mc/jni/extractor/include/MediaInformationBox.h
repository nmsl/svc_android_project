#ifndef MEDIA_INFORMATION_BOX_H
#define MEDIA_INFORMATION_BOX_H

#include "BaseBox.h"
#include "VideoMediaHeaderBox.h"
#include "DataInformationBox.h"
#include "SampleTableBox.h"

namespace Boxes {
	class MediaInformationBox : public BaseBox {
	public:
		MediaInformationBox (uint32_t size, uint64_t type);
		~MediaInformationBox ();

		void SetVideoMediaHeaderBox (VideoMediaHeaderBox *vmhdBox);
		void SetDataInformationBox (DataInformationBox *drefBox);
		void SetSampleTableBox (SampleTableBox *stblBox);

		VideoMediaHeaderBox* GetVideoMediaHeaderBox ();
		DataInformationBox* GetDataInformationBox ();
		SampleTableBox* GetSampleTableBox ();

		void Print (uint32_t level);

	private:
		VideoMediaHeaderBox *infoHeader;
		DataInformationBox *dataInfo;
		SampleTableBox *sampleTable;
	};
}

#endif /* MEDIA_INFORMATION_BOX_H */