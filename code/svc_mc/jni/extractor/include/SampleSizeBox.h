#ifndef SAMPLE_SIZE_BOX_H
#define SAMPLE_SIZE_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class SampleSizeBox : public BaseBox, public FullBox {
	public:
		SampleSizeBox (uint32_t size, uint64_t type);
		~SampleSizeBox ();

		void SetSampleSize (uint32_t size);
		void SetSampleCount (uint32_t count);
		// void AddSizeValue (uint32_t value);
		void SetSizeValues (uint32_t *values);

		uint32_t GetSampleSize ();
		uint32_t GetSampleCount ();
		uint32_t GetSizeValue (uint32_t index);

		void Print (uint32_t level);

	private:
		uint32_t sampleSize;
		uint32_t sampleCount;
		// std::vector<uint32_t> sizes;
		uint32_t *sizes;
	};
}

#endif /* SAMPLE_SIZE_BOX_H */