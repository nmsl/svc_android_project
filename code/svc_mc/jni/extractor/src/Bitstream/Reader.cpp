#include "Reader.h"

using namespace Boxes;
using namespace Bitstream;
using namespace std;

Reader::Reader (BitStream *b, SegmentType t) :
		bs (b),
		seg (new SegmentContainer(t)) {}
Reader::~Reader () {}

SegmentContainer* Reader::SegmentRead () {
	if (!this->bs) return NULL;
	TrackFragmentRunBox *trun = NULL;
	MovieFragmentBox *moof = NULL;

	while (bs->IsAvailableRead()) {
		BaseBox *box = ParseBox((BaseBox*) trun);

		switch (box->GetType()) {
			case SIDX:
				seg->SetSegmentIndexBox((SegmentIndexBox*) box);
				break;
			case MOOF:
				seg->SetMovieFragmentBox((MovieFragmentBox*) box);
				moof = seg->GetMovieFragmentBox();
				if (moof) {
					trun = moof->GetTrackFragmentBox(0)->GetTrackFragmentRunBox();
				}
				else {
					cout << "[Warning] - MovieFragmentBox is NULL" << endl;
				}
				break;
			case MDAT:
				seg->SetMediaDataBox((MediaDataBox*) box);
				break;

			case FTYP:
				seg->SetFileTypeBox((FileTypeBox*) box);
				break;
			case FREE:
				seg->SetFreeSpaceBox((FreeSpaceBox*) box);
				break;
			case MOOV:
				seg->SetMovieBox((MovieBox*) box);
				break;
			default:
				break;
		}
	}

	return seg;
}

BaseBox* Reader::ParseBox (BaseBox* ref) {
	uint32_t size = bs->ReadU32();
	uint64_t type = (uint64_t) bs->ReadU32();

	BaseBox *box = NewBox(size, type);
	ReadBox(box, ref);

	return box;
}

BaseBox* Reader::NewBox (uint32_t size, uint64_t type) {
	switch (type) {
		// Data Segment
		case SIDX:
			return (BaseBox*) new SegmentIndexBox(size, type);
		case MOOF:
			return (BaseBox*) new MovieFragmentBox(size, type);
		case MFHD:
			return (BaseBox*) new MovieFragmentHeaderBox(size, type);
		case TRAF:
			return (BaseBox*) new TrackFragmentBox(size, type);
		case TFHD:
			return (BaseBox*) new TrackFragmentHeaderBox(size, type);
		case TFDT:
			return (BaseBox*) new TFBaseMediaDecodeTimeBox(size, type);
		case TRUN:
			return (BaseBox*) new TrackFragmentRunBox(size, type);
		case MDAT:
			return (BaseBox*) new MediaDataBox(size, type);

		// Init Segment
		case FTYP:
			return (BaseBox*) new FileTypeBox(size, type);
		case FREE:
			return (BaseBox*) new FreeSpaceBox(size, type);
		case MOOV:
			return (BaseBox*) new MovieBox(size, type);
		case MVHD:
			return (BaseBox*) new MovieHeaderBox(size, type);
		case MVEX:
			return (BaseBox*) new MovieExtendsBox(size, type);
		case MEHD:
			return (BaseBox*) new MovieExtendsHeaderBox(size, type);
		case TREX:
			return (BaseBox*) new TrackExtendsBox(size, type);
		case TRAK:
			return (BaseBox*) new TrackBox(size, type);
		case TKHD:
			return (BaseBox*) new TrackHeaderBox(size, type);
		case TREF:
			return (BaseBox*) new TrackReferenceBox(size, type);
		case SBAS:
		case SCAL:
			return (BaseBox*) new TrackReferenceTypeBox(size, type);
		case MDIA:
			return (BaseBox*) new MediaBox(size, type);
		case MDHD:
			return (BaseBox*) new MediaHeaderBox(size, type);
		case HDLR:
			return (BaseBox*) new HandlerBox(size, type);
		case MINF:
			return (BaseBox*) new MediaInformationBox(size, type);
		case VMHD:
			return (BaseBox*) new VideoMediaHeaderBox(size, type);
		case DINF:
			return (BaseBox*) new DataInformationBox(size, type);
		case DREF:
			return (BaseBox*) new DataReferenceBox(size, type);
		case URL:
			return (BaseBox*) new DataEntryURLBox(size, type);
		case STBL:
			return (BaseBox*) new SampleTableBox(size, type);
		case STSD:
			return (BaseBox*) new SampleDescriptionBox(size, type);
		case AVC1:
		case SVC1:
			return (BaseBox*) new MPEGVisualSampleEntryBox(size, type);
		case AVCC:
		case SVCC:
			return (BaseBox*) new AVCConfigurationBox(size, type);
		case BTRT:
			return (BaseBox*) new MPEG4BitrateBox(size, type);
		case STTS:
			return (BaseBox*) new TimeToSampleBox(size, type);
			break;
		case STSC:
			return (BaseBox*) new SampleToChunkBox(size, type);
			break;
		case STSZ:
			return (BaseBox*) new SampleSizeBox(size, type);
			break;
		case STCO:
			return (BaseBox*) new ChunkOffsetBox(size, type);
			break;

		default:
			return new BaseBox(size, type);
	}
}

void Reader::ReadBox (BaseBox* box, BaseBox* ref) {
	switch (box->GetType()) {
		case SIDX:
			sidxRead(box);
			break;
		case MOOF:
			moofRead(box);
			break;
		case MFHD:
			mfhdRead(box);
			break;
		case TRAF:
			trafRead(box);
			break;
		case TFHD:
			tfhdRead(box);
			break;
		case TFDT:
			tfdtRead(box);
			break;
		case TRUN:
			trunRead(box);
			break;
		case MDAT:
			mdatRead(box, ref);
			break;

		case FTYP:
			ftypRead(box);
			break;
		case FREE:
			freeRead(box);
			break;
		case MOOV:
			moovRead(box);
			break;
		case MVHD:
			mvhdRead(box);
			break;
		case MVEX:
			mvexRead(box);
			break;
		case MEHD:
			mehdRead(box);
			break;
		case TREX:
			trexRead(box);
			break;
		case TRAK:
			trakRead(box);
			break;
		case TKHD:
			tkhdRead(box);
			break;
		case TREF:
			trefRead(box);
			break;
		case SBAS:
		case SCAL:
			REFTRead(box);
			break;
		case MDIA:
			mdiaRead(box);
			break;
		case MDHD:
			mdhdRead(box);
			break;
		case HDLR:
			hdlrRead(box);
			break;
		case MINF:
			minfRead(box);
			break;
		case VMHD:
			vmhdRead(box);
			break;
		case DINF:
			dinfRead(box);
			break;
		case DREF:
			drefRead(box);
			break;
		case URL:
			urlRead(box);
			break;
		case STBL:
			stblRead(box);
			break;
		case STSD:
			stsdRead(box);
			break;
		case AVC1:
		case SVC1:
			AVC_SVC_Read(box);
			break;
		case AVCC:
		case SVCC:
			AVCC_SVCC_Read(box);
			break;
		case BTRT:
			btrtRead(box);
			break;
		case STTS:
			sttsRead(box);
			break;
		case STSC:
			stscRead(box);
			break;
		case STSZ:
			stszRead(box);
			break;
		case STCO:
			stcoRead(box);
			break;

		default:
			bs->SkipBytes(box->HasData());
			break;
	}
}

void Reader::FullBoxRead (FullBox* box) {
	box->SetVersion((uint8_t) bs->ReadU8());
	box->SetFlags(bs->ReadU24());
}

void Reader::sidxRead (BaseBox *box) {
	SegmentIndexBox *ptr = (SegmentIndexBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetReferenceID(bs->ReadU32());
	ptr->SetTimeScale(bs->ReadU32());
	ptr->DecRemaind(8);

	if (ptr->GetVersion() == 0) {
		ptr->SetPresentationTime((uint64_t) bs->ReadU32());
		ptr->SetFirstOffset((uint64_t) bs->ReadU32());
		ptr->DecRemaind(8);
	}
	else {
		ptr->SetPresentationTime(bs->ReadU64());
		ptr->SetFirstOffset(bs->ReadU64());
		ptr->DecRemaind(16);
	}

	bs->ReadU16(); // Reserved
	uint32_t nb_refs = bs->ReadU16();
	ptr->DecRemaind(4);

	for (uint32_t i = 0; i < nb_refs; i++) {
		SIDXBoxRef *ref = new SIDXBoxRef();
		ref->SetType((bool) bs->ReadBitsToInt(1));
		ref->SetSize(bs->ReadBitsToInt(31));
		ref->SetDuration(bs->ReadU32());
		ref->SetSAP((bool) bs->ReadBitsToInt(1));
		ref->SetSAPType(bs->ReadBitsToInt(3));
		ref->SetSAPDeltaTime(bs->ReadBitsToInt(28));
		ptr->DecRemaind(12);

		ptr->SetReference(ref);
	}

	// ptr->Print(0);
}

void Reader::moofRead (BaseBox *box) {
	MovieFragmentBox *ptr = (MovieFragmentBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case MFHD:
				ptr->SetMovieFragmentHeaderBox((MovieFragmentHeaderBox*) subbox);
				break;
			case TRAF:
				ptr->AddTrackFragmentBox((TrackFragmentBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::mfhdRead(BaseBox* box) {
	MovieFragmentHeaderBox *ptr = (MovieFragmentHeaderBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetSequenceNumber(bs->ReadU32());
	ptr->DecRemaind(4);

	// ptr->Print(0);
}

void Reader::trafRead(BaseBox* box) {
	TrackFragmentBox *ptr = (TrackFragmentBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case TFHD:
				ptr->SetTrackFragmentHeaderBox((TrackFragmentHeaderBox*) subbox);
				break;
			case TFDT:
				ptr->SetTFBaseMediaDecodeTimeBox((TFBaseMediaDecodeTimeBox*) subbox);
				break;
			case TRUN:
				ptr->SetTrackFragmentRunBox((TrackFragmentRunBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::tfhdRead(BaseBox* box) {
	TrackFragmentHeaderBox *ptr = (TrackFragmentHeaderBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetTrackID(bs->ReadU32());
	ptr->DecRemaind(4);

	if (ptr->GetFlags() & TRAF_BASE_OFFSET) {
		ptr->SetBaseOffset(bs->ReadU64());
		ptr->DecRemaind(8);
	}
	if (ptr->GetFlags() & TRAF_SAMPLE_DESC) {
		ptr->SetSampleDescIndex(bs->ReadU32());
		ptr->DecRemaind(4);
	}
	if (ptr->GetFlags() & TRAF_SAMPLE_DUR) {
		ptr->SetSampleDuration(bs->ReadU32());
		ptr->DecRemaind(4);
	}
	if (ptr->GetFlags() & TRAF_SAMPLE_SIZE) {
		ptr->SetSampleSize(bs->ReadU32());
		ptr->DecRemaind(4);
	}
	if (ptr->GetFlags() & TRAF_SAMPLE_FLAGS) {
		ptr->SetSampleFlags(bs->ReadU32());
		ptr->DecRemaind(4);
	}
}

void Reader::tfdtRead(BaseBox* box) {
	TFBaseMediaDecodeTimeBox *ptr = (TFBaseMediaDecodeTimeBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	if (ptr->GetVersion() == 0) {
		ptr->SetMediaDecodeTime((uint64_t) bs->ReadU32());
		ptr->DecRemaind(4);
	}
	else {
		ptr->SetMediaDecodeTime(bs->ReadU64());
		ptr->DecRemaind(8);
	}
}

void Reader::trunRead(BaseBox* box) {
	TrackFragmentRunBox *ptr = (TrackFragmentRunBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetSampleCount(bs->ReadU32());
	ptr->DecRemaind(4);

	if (ptr->GetFlags() & TRUN_DATA_OFFSET) {
		ptr->SetDataOffset(bs->ReadU32());
		ptr->DecRemaind(4);
	}
	if (ptr->GetFlags() & TRUN_FIRST_FLAG) {
		ptr->SetFirstSampleFlags(bs->ReadU32());
		ptr->DecRemaind(4);
	}

	for (uint32_t i = 0; i < ptr->GetSampleCount(); i++) {
		TrunEntry *entry = new TrunEntry();
		uint32_t trun_size = 0;

		if (ptr->GetFlags() & TRUN_DURATION) {
			entry->SetDuration(bs->ReadU32());
			trun_size += 4;
		}
		if (ptr->GetFlags() & TRUN_SIZE) {
			entry->SetSize(bs->ReadU32());
			trun_size += 4;
		}
		if (ptr->GetFlags() & TRUN_FLAGS) {
			entry->SetFlags(bs->ReadU32());
			trun_size += 4;
		}
		if (ptr->GetFlags() & TRUN_CTS_OFFSET) {
			entry->SetCTSOffset(bs->ReadU32());
			trun_size += 4;
		}
		ptr->AddEntry(entry);
		ptr->DecRemaind(trun_size);
	}
}

void Reader::mdatRead(BaseBox* box, BaseBox* ref) {
	MediaDataBox *ptr = (MediaDataBox*) box;
	TrackFragmentRunBox *refBox = (TrackFragmentRunBox*) ref;

	ptr->SetRefTRUN(refBox);

	uint32_t nb_sample = refBox->GetSampleCount();
	for (uint32_t i = 0; i < nb_sample; i++) {
		TrunEntry *trunEntry = refBox->GetEntry(i);
		MediaDataEntry *mediaEntry = new MediaDataEntry();
		mediaEntry->SetNumber(i+1);
		mediaEntry->SetSize(trunEntry->GetSize());
		mediaEntry->SetDataOffset(bs->GetPosition());

		// uint8_t* data = (uint8_t*) calloc(mediaEntry->GetSize(), sizeof(uint8_t));
		uint8_t *data = new uint8_t[mediaEntry->GetSize()];
		bs->ReadData(data, mediaEntry->GetSize());
		mediaEntry->SetData(data);

		ptr->DecRemaind(mediaEntry->GetSize());
		ptr->AddMediaData(mediaEntry);
	}

	// ptr->Print(0);
}

void Reader::ftypRead(BaseBox* box) {
	FileTypeBox *ptr = (FileTypeBox*) box;

	ptr->SetMajorBrand(bs->ReadU32());
	ptr->SetMinorVersion(bs->ReadU32());
	ptr->DecRemaind(8);

	uint32_t count = ptr->HasData() / 4;
	for (uint32_t i = 0; i < count; i++) {
		ptr->AddAltBrand(bs->ReadU32());
	}

	// ptr->Print(0);
}

void Reader::freeRead(BaseBox* box) {
	FreeSpaceBox *ptr = (FreeSpaceBox*) box;

	uint32_t size = ptr->HasData();
	if (size) {
		ptr->SetDataSize(size);
		// uint8_t *data = (uint8_t *) calloc(size, sizeof(uint8_t));
		uint8_t *data = new uint8_t[size];
		bs->ReadData(data, size);
		ptr->SetData(data);
		ptr->DecRemaind(size);
	}

	// ptr->Print(0);
}

void Reader::moovRead(BaseBox* box) {
	MovieBox *ptr = (MovieBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case MVHD:
				ptr->SetMovieHeaderBox((MovieHeaderBox*) subbox);
				break;
			case MVEX:
				ptr->SetMovieExtendsBox((MovieExtendsBox*) subbox);
				break;
			case TRAK:
				ptr->AddTrackBox((TrackBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::mvhdRead(BaseBox* box) {
	MovieHeaderBox *ptr = (MovieHeaderBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	if (ptr->GetFlags() == 0) {
		ptr->SetCreationTime((uint64_t) bs->ReadU32());
		ptr->SetModificationTime((uint64_t) bs->ReadU32());
		ptr->SetTimeScale(bs->ReadU32());
		ptr->SetDuration((uint64_t) bs->ReadU32());
		ptr->DecRemaind(16);
	}
	else {
		ptr->SetCreationTime(bs->ReadU64());
		ptr->SetModificationTime(bs->ReadU64());
		ptr->SetTimeScale(bs->ReadU32());
		ptr->SetDuration(bs->ReadU64());
		ptr->DecRemaind(28);
	}

	if (ptr->GetTimeScale() == 0) ptr->SetTimeScale(600);

	ptr->SetPreferredRate(bs->ReadU32());
	ptr->SetPreferredVolume(bs->ReadU16());
	bs->ReadData(ptr->GetReserved(), 10);
	ptr->DecRemaind(16);

	ptr->SetMatrixA(bs->ReadU32());
	ptr->SetMatrixB(bs->ReadU32());
	ptr->SetMatrixU(bs->ReadU32());
	ptr->SetMatrixC(bs->ReadU32());
	ptr->SetMatrixD(bs->ReadU32());
	ptr->SetMatrixV(bs->ReadU32());
	ptr->SetMatrixX(bs->ReadU32());
	ptr->SetMatrixY(bs->ReadU32());
	ptr->SetMatrixW(bs->ReadU32());
	ptr->DecRemaind(36);

	ptr->SetPreviewTime(bs->ReadU32());
	ptr->SetPreviewDuration(bs->ReadU32());
	ptr->SetPosterTime(bs->ReadU32());
	ptr->SetSelectionTime(bs->ReadU32());
	ptr->SetSelectionDuration(bs->ReadU32());
	ptr->SetCurrentTime(bs->ReadU32());
	ptr->SetNextTrackID(bs->ReadU32());
	ptr->DecRemaind(28);

	// ptr->Print(0);
}

void Reader::mvexRead(BaseBox* box) {
	MovieExtendsBox *ptr = (MovieExtendsBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case MEHD:
				ptr->SetMovieExtendsHeaderBox((MovieExtendsHeaderBox*) subbox);
				break;
			case TREX:
				ptr->AddTrackExtendsBox((TrackExtendsBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::mehdRead(BaseBox* box) {
	MovieExtendsHeaderBox *ptr = (MovieExtendsHeaderBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	if (ptr->GetFlags() == 0) {
		ptr->SetFragmentDuration((uint64_t) bs->ReadU32());
		ptr->DecRemaind(4);
	}
	else {
		ptr->SetFragmentDuration(bs->ReadU32());
		ptr->DecRemaind(8);
	}
}

void Reader::trexRead(BaseBox* box) {
	TrackExtendsBox *ptr = (TrackExtendsBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetTrackID(bs->ReadU32());
	ptr->SetSampleDescIndex(bs->ReadU32());
	ptr->SetSampleDuration(bs->ReadU32());
	ptr->SetSampleSize(bs->ReadU32());
	ptr->SetSampleFlags(bs->ReadU32());
	ptr->DecRemaind(20);

	if (ptr->GetSampleDescIndex() == 0)
		ptr->SetSampleDescIndex(1);
}

void Reader::trakRead(BaseBox* box) {
	TrackBox *ptr = (TrackBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case TKHD:
				ptr->SetTrackHeaderBox((TrackHeaderBox*) subbox);
				break;
			case TREF:
				ptr->SetReferenceBox((TrackReferenceBox*) subbox);
				break;
			case MDIA:
				ptr->SetMediaBox((MediaBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::tkhdRead(BaseBox* box) {
	TrackHeaderBox *ptr = (TrackHeaderBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	if (ptr->GetVersion() == 0) {
		ptr->SetCreationTime((uint64_t) bs->ReadU32());
		ptr->SetModificationTime((uint64_t) bs->ReadU32());
		ptr->SetTrackID(bs->ReadU32());
		ptr->SetReserved1(bs->ReadU32());
		ptr->SetDuration((uint64_t) bs->ReadU32());
		ptr->DecRemaind(20);
	}
	else {
		ptr->SetCreationTime(bs->ReadU64());
		ptr->SetModificationTime(bs->ReadU64());
		ptr->SetTrackID(bs->ReadU32());
		ptr->SetReserved1(bs->ReadU32());
		ptr->SetDuration(bs->ReadU64());
		ptr->DecRemaind(32);
	}

	ptr->SetReserved2(bs->ReadU32(), 0);
	ptr->SetReserved2(bs->ReadU32(), 1);
	ptr->DecRemaind(8);

	ptr->SetLayer(bs->ReadU16());
	ptr->SetAlternateGroup(bs->ReadU16());
	ptr->SetVolume(bs->ReadU16());
	ptr->SetReserved3(bs->ReadU16());
	ptr->DecRemaind(8);

	ptr->SetMatrix(bs->ReadU32(), 0);
	ptr->SetMatrix(bs->ReadU32(), 1);
	ptr->SetMatrix(bs->ReadU32(), 2);
	ptr->SetMatrix(bs->ReadU32(), 3);
	ptr->SetMatrix(bs->ReadU32(), 4);
	ptr->SetMatrix(bs->ReadU32(), 5);
	ptr->SetMatrix(bs->ReadU32(), 6);
	ptr->SetMatrix(bs->ReadU32(), 7);
	ptr->SetMatrix(bs->ReadU32(), 8);
	ptr->DecRemaind(36);

	ptr->SetWidth(bs->ReadU32());
	ptr->SetHeight(bs->ReadU32());
	ptr->DecRemaind(8);

	// ptr->Print(0);
}

void Reader::trefRead(BaseBox *box) {
	TrackReferenceBox *ptr = (TrackReferenceBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case SBAS:
			case SCAL:
				ptr->SetReferenceBox((TrackReferenceTypeBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::REFTRead(BaseBox* box) {
	TrackReferenceTypeBox *ptr = (TrackReferenceTypeBox*) box;

	uint32_t count = ptr->HasData()/4;
	ptr->SetTrackCount(count);
	// uint32_t *data = (uint32_t*) calloc(count, sizeof(uint32_t));
	uint32_t *data = new uint32_t[count];

	for (uint32_t i = 0; i < count; i++) {
		data[i] = bs->ReadU32();
		ptr->DecRemaind(4);
	}
	ptr->SetTrackID(data);
}

void Reader::mdiaRead(BaseBox* box) {
	MediaBox *ptr = (MediaBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case MDHD:
				ptr->SetMediaHeaderBox((MediaHeaderBox*) subbox);
				break;
			case HDLR:
				ptr->SetHandlerBox((HandlerBox*) subbox);
				break;
			case MINF:
				ptr->SetMediaInformationBox((MediaInformationBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}
}

void Reader::mdhdRead(BaseBox* box) {
	MediaHeaderBox *ptr = (MediaHeaderBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	if (ptr->GetVersion() == 0) {
		ptr->SetCreationTime((uint64_t) bs->ReadU32());
		ptr->SetModificationTime((uint64_t) bs->ReadU32());
		ptr->SetTimeScale(bs->ReadU32());
		ptr->SetDuration((uint64_t) bs->ReadU32());
		ptr->DecRemaind(12);
	}
	else {
		ptr->SetCreationTime(bs->ReadU64());
		ptr->SetModificationTime(bs->ReadU64());
		ptr->SetTimeScale(bs->ReadU32());
		ptr->SetDuration(bs->ReadU64());
		ptr->DecRemaind(28);
	}

	if (ptr->GetTimeScale() == 0)
		ptr->SetTimeScale(90000);

	bs->ReadBitsToInt(1);
	ptr->SetLanguage((uint8_t) bs->ReadBitsToInt(5), 0);
	ptr->SetLanguage((uint8_t) bs->ReadBitsToInt(5), 1);
	ptr->SetLanguage((uint8_t) bs->ReadBitsToInt(5), 2);
	ptr->CheckpackedLanguage();
	ptr->DecRemaind(4);

	ptr->SetReserved(bs->ReadU16());
	ptr->DecRemaind(2);

	// ptr->Print(0);
}

void Reader::hdlrRead(BaseBox* box) {
	HandlerBox *ptr = (HandlerBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetReserved1(bs->ReadU32());
	ptr->SetHandlerType(bs->ReadU32());
	bs->ReadData(ptr->GetReserved2(), 12);
	ptr->DecRemaind(20);

	uint32_t size = ptr->HasData();
	if (size) {
		ptr->SetNameUTF8Size(size);
		// uint8_t *data = (uint8_t*) calloc(size+1, sizeof(uint8_t));
		uint8_t *data = new uint8_t[size+1];
		bs->ReadData(data, size);
		data[size] = 0;
		ptr->SetNameUTF8(data);
		ptr->DecRemaind(size);
	}

	// ptr->Print(0);
}

void Reader::minfRead(BaseBox* box) {
	MediaInformationBox *ptr = (MediaInformationBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case VMHD:
				ptr->SetVideoMediaHeaderBox((VideoMediaHeaderBox*) subbox);
				break;
			case DINF:
				ptr->SetDataInformationBox((DataInformationBox*) subbox);
				break;
			case STBL:
				ptr->SetSampleTableBox((SampleTableBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::vmhdRead(BaseBox* box) {
	VideoMediaHeaderBox *ptr = (VideoMediaHeaderBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetReserved(bs->ReadU64());
	ptr->DecRemaind(8);

	// ptr->Print(0);
}

void Reader::dinfRead(BaseBox* box) {
	DataInformationBox *ptr = (DataInformationBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case DREF:
				ptr->SetDataReferenceBox((DataReferenceBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::drefRead(BaseBox* box) {
	DataReferenceBox *ptr = (DataReferenceBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	// count =
	bs->ReadU32();
	ptr->DecRemaind(4);

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case URL:
				ptr->AddDataEntryURLBox((DataEntryURLBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}
}

void Reader::urlRead(BaseBox* box) {
	DataEntryURLBox *ptr = (DataEntryURLBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	uint32_t count = ptr->HasData();
	if (count > 0) {
		ptr->SetCharCount(count);
		// uint8_t *data = (uint8_t*) calloc(count, sizeof(uint8_t));
		uint8_t *data = new uint8_t[count];
		bs->ReadData(data, count);
		ptr->DecRemaind(count);
	}
}

void Reader::stblRead(BaseBox* box) {
	SampleTableBox *ptr = (SampleTableBox*) box;

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case STSD:
				ptr->SetSampleDescriptionBox((SampleDescriptionBox*) subbox);
				break;
			case STTS:
				ptr->SetTimeToSampleBox((TimeToSampleBox*) subbox);
				break;
			case STSC:
				ptr->SetSampleToChunkBox((SampleToChunkBox*) subbox);
				break;
			case STSZ:
				ptr->SetSampleSizeBoc((SampleSizeBox*) subbox);
				break;
			case STCO:
				ptr->SetChunkOffsetBox((ChunkOffsetBox*) subbox);
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::stsdRead(BaseBox* box) {
	SampleDescriptionBox *ptr = (SampleDescriptionBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetEntryCount(bs->ReadU32());
	ptr->DecRemaind(4);

	for (uint32_t i = 0; i < ptr->GetEntryCount(); i++) {
		BaseBox *subbox = ParseBox(NULL);
		if (subbox->GetType() == AVC1 || subbox->GetType() == SVC1) {
			ptr->AddEntry((MPEGVisualSampleEntryBox*) subbox);
		}

		ptr->DecRemaind(subbox->GetSize());
	}

	// ptr->Print(0);
}

void Reader::AVC_SVC_Read(BaseBox* box) {
	MPEGVisualSampleEntryBox *ptr = (MPEGVisualSampleEntryBox*) box;

	bs->ReadData(ptr->GetReserved(), 6);
	ptr->SetDataReferenceIndex(bs->ReadU16());
	ptr->SetVersion(bs->ReadU16());
	ptr->SetRevision(bs->ReadU16());
	ptr->SetVendor(bs->ReadU32());
	ptr->SetTemporal(bs->ReadU32());
	ptr->SetSpatial(bs->ReadU32());
	ptr->SetWidth(bs->ReadU16());
	ptr->SetHeight(bs->ReadU16());
	ptr->SetHorizResolution(bs->ReadU32());
	ptr->SetVertResolution(bs->ReadU32());
	ptr->SetEntryDataSize(bs->ReadU32());
	ptr->SetFramePerSample(bs->ReadU16());
	uint8_t *data = ptr->GetCompressorName();
	bs->ReadData(data, 32);
	data[32] = 0;
	ptr->SetBitDepth(bs->ReadU16());
	ptr->SetColorTableIndex(bs->ReadU16());
	ptr->DecRemaind(78);

	while (ptr->HasData()) {
		BaseBox *subbox = ParseBox(NULL);

		switch (subbox->GetType()) {
			case AVCC:
				ptr->SetAVCConfig((AVCConfigurationBox*) subbox);
				break;
			case SVCC:
				ptr->SetSVCConfig((AVCConfigurationBox*) subbox);
				break;
			case BTRT:
				ptr->SetBitrateBox((MPEG4BitrateBox*) subbox);
				break;
			default:
				break;
		}

		ptr->DecRemaind(subbox->GetSize());
	}
}

void Reader::AVCC_SVCC_Read(BaseBox* box) {
	AVCConfigurationBox *ptr = (AVCConfigurationBox*) box;
	uint32_t count = 0;

	ptr->SetConfigurationVersion(bs->ReadU8());
	ptr->SetAVCProfileIndication(bs->ReadU8());
	ptr->SetProfileCompatiblitiy(bs->ReadU8());
	ptr->SetAVCLevelIndication(bs->ReadU8());
	ptr->DecRemaind(4);

	if (ptr->GetType() == AVCC) {
		bs->ReadBitsToInt(6);
	}
	else {
		ptr->SetCompleteRepresentation(bs->ReadBitsToInt(1));
		bs->ReadBitsToInt(5);
	}
	ptr->SetNALUintSize(bs->ReadBitsToInt(2)+1);
	bs->ReadBitsToInt(3);
	count = bs->ReadBitsToInt(5);
	ptr->DecRemaind(2);

	for (uint32_t i = 0; i < count; i++) {
		AVCConfigSlot *slot = new AVCConfigSlot();
		slot->SetDataSize(bs->ReadU16());
		// uint8_t *data = (uint8_t*) calloc(slot->GetSize(), sizeof(uint8_t));
		uint8_t *data = new uint8_t[slot->GetSize()];
		bs->ReadData(data, slot->GetSize());
		slot->SetData(data);
		ptr->AddSequenceParameterSet(slot);
		ptr->DecRemaind(2+slot->GetSize());
	}

	count = bs->ReadU8();
	ptr->DecRemaind(1);
	for (uint32_t i = 0; i < count; i++) {
		AVCConfigSlot *slot = new AVCConfigSlot();
		slot->SetDataSize(bs->ReadU16());
		// uint8_t *data = (uint8_t*) calloc(slot->GetSize(), sizeof(uint8_t));
		uint8_t *data = new uint8_t[slot->GetSize()];
		bs->ReadData(data, slot->GetSize());
		slot->SetData(data);
		ptr->AddPictureParameterSet(slot);
		ptr->DecRemaind(2+slot->GetSize());
	}
}

void Reader::btrtRead(BaseBox* box) {
	MPEG4BitrateBox *ptr = (MPEG4BitrateBox*) box;

	ptr->SetBufferSizeDB(bs->ReadU32());
	ptr->SetMaxBitrate(bs->ReadU32());
	ptr->SetAvgBitrate(bs->ReadU32());

	ptr->DecRemaind(12);
}

void Reader::sttsRead(BaseBox* box) {
	TimeToSampleBox *ptr = (TimeToSampleBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetEntryCount(bs->ReadU32());
	ptr->DecRemaind(4);

	for (uint32_t i = 0; i < ptr->GetEntryCount(); i++) {
		STTSEntry *entry = new STTSEntry();
		entry->SetSampleCount(bs->ReadU32());
		entry->SetSampleDelta(bs->ReadU32());
		ptr->DecRemaind(8);

		ptr->AddCurrentSampleNum(entry->GetSampleCount());
		ptr->AddLastDTS(entry->GetSampleCount()*entry->GetSampleDelta());
		ptr->AddEntry(entry);
	}

	if (ptr->GetEntryCount() > 0) {
		uint32_t lastDTS = ptr->GetLastDTS();
		STTSEntry *lastEntry = ptr->GetEntry(ptr->GetEntryCount()-1);
		ptr->SetLastDTS(lastDTS - lastEntry->GetSampleDelta());
	}

	// ptr->Print(0);
}

void Reader::stscRead(BaseBox* box) {
	SampleToChunkBox *ptr = (SampleToChunkBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetEntryCount(bs->ReadU32());
	ptr->DecRemaind(4);

	for (uint32_t i = 0; i < ptr->GetEntryCount(); i++) {
		STSCEntry *newentry = new STSCEntry();
		newentry->SetFirstChunk(bs->ReadU32());
		newentry->SetSamplePerChunk(bs->ReadU32());
		newentry->SetSampleDescIndex(bs->ReadU32());
		newentry->SetEdited(false);
		newentry->SetNextChunk(0);
		ptr->DecRemaind(12);

		STSCEntry *lastEntry = ptr->GetEntry(ptr->GetEntryCount()-1);
		if (lastEntry) {
			lastEntry->SetNextChunk(newentry->GetFirstChunk());
		}
	}

	// ptr->Print(0);
}

void Reader::stszRead(BaseBox* box) {
	SampleSizeBox *ptr = (SampleSizeBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	if (ptr->GetType() == STSZ) {
		ptr->SetSampleSize(bs->ReadU32());
		ptr->SetSampleCount(bs->ReadU32());
		ptr->DecRemaind(8);
	}
	else {
		bs->ReadBitsToInt(24);
		uint32_t i = bs->ReadU8();
		ptr->SetSampleCount(bs->ReadU32());
		ptr->DecRemaind(5);
		switch (i) {
			case 4:
			case 8:
			case 16:
				ptr->SetSampleSize(i);
				break;
			default:
				if (ptr->GetSampleCount() == 0) {
					ptr->SetSampleSize(16);
					return;
				}
				break;
		}
	}

	if (ptr->GetType() == STSZ) {
		if (!ptr->GetSampleSize() && ptr->GetSampleCount()) {
			// uint32_t *values = (uint32_t*) calloc(ptr->GetSampleCount(), sizeof(uint32_t));
			uint32_t *values = new uint32_t[ptr->GetSampleCount()];
			for (uint32_t i = 0; i < ptr->GetSampleCount(); i++) {
				values[i] = bs->ReadU32();
				ptr->DecRemaind(4);
			}

			ptr->SetSizeValues(values);
		}
	}
	else {
		// uint32_t *values = (uint32_t*) calloc(ptr->GetSampleCount(), sizeof(uint32_t));
		uint32_t *values = new uint32_t[ptr->GetSampleCount()];
		for (uint32_t i = 0; i < ptr->GetSampleCount();) {
			switch (ptr->GetSampleSize()) {
				case 4:
					values[i] = bs->ReadBitsToInt(4);
					if (i+1 < ptr->GetSampleCount()) {
						values[i+1] = bs->ReadBitsToInt(4);
					}
					else {
						bs->ReadBitsToInt(4);
					}
					ptr->DecRemaind(1);
					break;
				default:
					values[i] = bs->ReadBitsToInt(ptr->GetSampleSize());
					i++;
					ptr->DecRemaind(ptr->GetSampleSize());
					break;
			}
		}
		ptr->SetSizeValues(values);
	}

	// ptr->Print(0);
}

void Reader::stcoRead(BaseBox* box) {
	ChunkOffsetBox *ptr = (ChunkOffsetBox*) box;

	FullBoxRead(ptr);
	ptr->DecRemaind(4);

	ptr->SetEntryCount(bs->ReadU32());
	ptr->DecRemaind(4);

	if (ptr->GetEntryCount() > 0) {
		// uint32_t *data = (uint32_t*) calloc(ptr->GetEntryCount(), sizeof(uint32_t));
		uint32_t *data = new uint32_t[ptr->GetEntryCount()];
		for (uint32_t i = 0; i < ptr->GetEntryCount(); i++) {
			data[i] = bs->ReadU32();
			ptr->DecRemaind(4);
		}
		ptr->SetOffsets(data);
	}

	// ptr->Print(0);
}