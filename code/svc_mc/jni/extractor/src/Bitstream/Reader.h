#ifndef READER_H
#define READER_H

#include "BitStream.h"
#include "SegmentContainer.h"
#include "AllBoxes.h"

using namespace Boxes;

namespace Bitstream {
	class Reader {
	public:
		Reader (BitStream *b, SegmentType t);
		~Reader ();

		SegmentContainer* SegmentRead ();

	private:
		BitStream *bs;
		SegmentContainer *seg;

		BaseBox* ParseBox (BaseBox* box);
		BaseBox* NewBox (uint32_t size, uint64_t type);
		void ReadBox (BaseBox* box, BaseBox* ref);
		void FullBoxRead (FullBox* box);

		/* Init Segment */
		void ftypRead(BaseBox* box);
		void freeRead(BaseBox* box);
		void moovRead(BaseBox* box);
		void mvhdRead(BaseBox* box);
		void mvexRead(BaseBox* box);
		void mehdRead(BaseBox* box);
		void trexRead(BaseBox* box);
		void trakRead(BaseBox* box);
		void tkhdRead(BaseBox* box);
		void trefRead(BaseBox* box);
		void REFTRead(BaseBox* box);
		void mdiaRead(BaseBox* box);
		void mdhdRead(BaseBox* box);
		void hdlrRead(BaseBox* box);
		void minfRead(BaseBox* box);
		void vmhdRead(BaseBox* box);
		void dinfRead(BaseBox* box);
		void drefRead(BaseBox* box);
		void urlRead(BaseBox* box);
		void stblRead(BaseBox* box);
		void stsdRead(BaseBox* box);
		void AVC_SVC_Read(BaseBox* box);
		void AVCC_SVCC_Read(BaseBox* box);
		void btrtRead(BaseBox* box);
		void sttsRead(BaseBox* box);
		void stscRead(BaseBox* box);
		void stszRead(BaseBox* box);
		void stcoRead(BaseBox* box);

		// BaseBox* ftypNew(uint32_t size, uint64_t type);
		// BaseBox* freeNew(uint32_t size, uint64_t type);
		// BaseBox* moovNew(uint32_t size, uint64_t type);
		// BaseBox* mvhdNew(uint32_t size, uint64_t type);
		// BaseBox* mvexNew(uint32_t size, uint64_t type);
		// BaseBox* mehdNew(uint32_t size, uint64_t type);
		// BaseBox* trexNew(uint32_t size, uint64_t type);
		// BaseBox* trakNew(uint32_t size, uint64_t type);
		// BaseBox* tkhdNew(uint32_t size, uint64_t type);
		// BaseBox* mdiaNew(uint32_t size, uint64_t type);
		// BaseBox* mdhdNew(uint32_t size, uint64_t type);
		// BaseBox* hdlrNew(uint32_t size, uint64_t type);
		// BaseBox* minfNew(uint32_t size, uint64_t type);
		// BaseBox* vmhdNew(uint32_t size, uint64_t type);
		// BaseBox* dinfNew(uint32_t size, uint64_t type);
		// BaseBox* drefNew(uint32_t size, uint64_t type);
		// BaseBox* urlNew(uint32_t size, uint64_t type);
		// BaseBox* stblNew(uint32_t size, uint64_t type);
		// BaseBox* stsdNew(uint32_t size, uint64_t type);
		// BaseBox* AVC_SVC_New(uint32_t size, uint64_t type);
		// BaseBox* AVCC_SVCC_New(uint32_t size, uint64_t type);
		// BaseBox* btrtNew(uint32_t size, uint64_t type);
		// BaseBox* trefNew(uint32_t size, uint64_t type);

		/* Data segment */
		void sidxRead(BaseBox* box);
		void moofRead(BaseBox* box);
		void mfhdRead(BaseBox* box);
		void trafRead(BaseBox* box);
		void tfhdRead(BaseBox* box);
		void tfdtRead(BaseBox* box);
		void trunRead(BaseBox* box);
		void mdatRead(BaseBox* box, BaseBox* ref);

		// BaseBox* box sidxNew(uint32_t size, uint64_t type);
		// BaseBox* box moofNew(uint32_t size, uint64_t type);
		// BaseBox* box mfhdNew(uint32_t size, uint64_t type);
		// BaseBox* box trafNew(uint32_t size, uint64_t type);
		// BaseBox* box tfhdNew(uint32_t size, uint64_t type);
		// BaseBox* box tfdtNew(uint32_t size, uint64_t type);
		// BaseBox* box trunNew(uint32_t size, uint64_t type);
		// BaseBox* box mdatNew(uint32_t size, uint64_t type);
	};
}

#endif /* READER_H */