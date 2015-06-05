#ifndef MPEG_VISUAL_SAMPLE_ENTRY_BOX_H
#define MPEG_VISUAL_SAMPLE_ENTRY_BOX_H

#include "BaseBox.h"
#include "AVCConfigurationBox.h"
#include "MPEG4BitrateBox.h"

namespace Boxes {
	class MPEGVisualSampleEntryBox : public BaseBox {
	public:
		MPEGVisualSampleEntryBox (uint32_t size, uint64_t type);
		~MPEGVisualSampleEntryBox ();

		void SetDataReferenceIndex (uint16_t index);
		void SetVersion (uint16_t v);
		void SetRevision (uint16_t r);
		void SetVendor (uint32_t vendor);
		void SetTemporal (uint32_t t);
		void SetSpatial (uint32_t s);
		void SetWidth (uint16_t w);
		void SetHeight (uint16_t h);
		void SetHorizResolution (uint32_t res);
		void SetVertResolution (uint32_t res);
		void SetEntryDataSize (uint32_t size);
		void SetFramePerSample (uint16_t fps);
		void SetBitDepth (uint16_t depth);
		void SetColorTableIndex (uint16_t index);
		void SetAVCConfig (AVCConfigurationBox *conf);
		void SetSVCConfig (AVCConfigurationBox *conf);
		void SetBitrateBox (MPEG4BitrateBox *btrtBox);

		uint8_t* GetReserved ();
		uint16_t GetDataReferenceIndex ();
		uint16_t GetVersion ();
		uint16_t GetRevision ();
		uint32_t GetVendor ();
		uint32_t GetTemporal ();
		uint32_t GetSpatial ();
		uint16_t GetWidth ();
		uint16_t GetHeight ();
		uint32_t GetHorizResolution ();
		uint32_t GetVertResolution ();
		uint32_t GetEntryDataSize ();
		uint16_t GetFramePerSample ();
		uint8_t* GetCompressorName ();
		uint16_t GetBitDepth ();
		uint16_t GetColorTableIndex ();
		AVCConfigurationBox* GetAVCConfig ();
		AVCConfigurationBox* GetSVCConfig ();
		MPEG4BitrateBox* GetBitrateBox ();

		void Print (uint32_t level);

	private:
		uint8_t reserved[6];
		uint16_t dataReferenceIndex;
		uint16_t version;
		uint16_t revision;
		uint32_t vendor;
		uint32_t temporal_quality;
		uint32_t spatial_quality;
		uint16_t width;
		uint16_t height;
		uint32_t horiz_res;
		uint32_t vert_res;
		uint32_t entry_data_size;
		uint16_t frames_per_sample;
		uint8_t compressor_name[33];
		uint16_t bit_depth;
		uint16_t color_table_index;
		AVCConfigurationBox *avc_config;
		AVCConfigurationBox *svc_config;
		MPEG4BitrateBox *bitrate;
	};
}

#endif /* MPEG_VISUAL_SAMPLE_ENTRY_BOX_H */