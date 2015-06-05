#ifndef MANAGER_H
#define MANAGER_H

#include "config.h"
#include "SegmentContainer.h"
#include "BitStream.h"
#include "Reader.h"

#include "MultiThreading.h"

#include <deque>

using namespace Boxes;

class SegmentInfo {
	public:
		SegmentInfo (Bitstream::BitStream *input, uint32_t frames, uint32_t layerid) {
			segment = input;
			samples = frames;
			layerID = layerid;
		}
		~SegmentInfo () {
			if (segment) delete segment;
		}

		Bitstream::BitStream *segment;
		uint32_t samples;
		uint32_t layerID;
};

namespace Bitstream {
	class Manager {
	public:
		Manager (uint32_t SegmentSize);
		~Manager ();

		void SetBitStream (uint8_t* data, uint32_t dataSize);
		void SetSegmentType (SegmentType type);
		void StartParse ();
		void DeleteAllDataSegs ();
		// BitStream* StartExtract ();
		void StartExtract ();

		uint32_t GetBoxesSize ();
		SegmentContainer* GetInitSegment ();
		SegmentContainer* GetDataSegment (uint32_t index);
		uint32_t GetFrameCount ();
		// BitStream* GetDecodableSegment ();
		BitStream* GetDecodableSegment (uint32_t &samples, uint32_t &id);

		void SetEOS (bool value);

	private:
		SegmentType type;
		Reader *reader;
		BitStream *bs;
		BitStream *rawVideo;
		SegmentContainer *initSeg;
		// Store all layers of Segment
		std::vector<SegmentContainer*> dataSegs;
		uint32_t totalSize;

		// Store Extraced Segment
		uint32_t curSamples;
		uint32_t curLayers;
		uint32_t maxSegmentSize;
		// std::deque<BitStream*> segmentList;
		// std::deque<uint32_t> segmentSampleList;
		std::deque<SegmentInfo*> segmentInfoList;
		bool eos;

		CRITICAL_SECTION monitorMutex;
		CONDITION_VARIABLE full;
		CONDITION_VARIABLE empty;


		uint32_t IsDataSegmentValid ();
		bool WriteParameterSets (uint32_t maxLayer);
		bool WriteData (uint32_t maxLayer);
		void WriteSample (BitStream *bs, uint32_t trackID);

		void ShowErrorMessage (std::string msg);
		void ShowWarningMessage (std::string msg);

		void ExtractedNotify();

		AVCConfigurationBox* GetConfiguation (uint32_t trackID);
		MediaDataBox* GetMediaData (uint32_t trackID);
		uint32_t GetSampleCount (MediaDataBox *mdat);
	};
}

#endif /* MANAGER_H */