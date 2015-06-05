#ifndef ALLBOXES_H
#define ALLBOXES_H

#include "BaseBox.h"
#include "FullBox.h"

/* Init Segment */
#include "SegmentIndexBox.h"
#include "SIDXBoxRef.h"
#include "MovieFragmentBox.h"
#include "MovieFragmentHeaderBox.h"
#include "TrackFragmentBox.h"
#include "TrackFragmentHeaderBox.h"
#include "TFBaseMediaDecodeTimeBox.h"
#include "TrackFragmentRunBox.h"
#include "TrunEntry.h"
#include "MediaDataBox.h"
#include "MediaDataEntry.h"

/* Data Segment */
#include "FileTypeBox.h"
#include "FreeSpaceBox.h"
#include "MovieBox.h"
#include "MovieHeaderBox.h"
#include "MovieExtendsBox.h"
#include "MovieExtendsHeaderBox.h"
#include "TrackExtendsBox.h"
#include "TrackBox.h"
#include "TrackHeaderBox.h"
#include "MediaBox.h"
#include "MediaHeaderBox.h"
#include "HandlerBox.h"
#include "MediaInformationBox.h"
#include "VideoMediaHeaderBox.h"
#include "DataInformationBox.h"
#include "DataReferenceBox.h"
#include "DataEntryURLBox.h"
#include "SampleTableBox.h"
#include "SampleDescriptionBox.h"
#include "MPEGVisualSampleEntryBox.h"
#include "AVCConfigurationBox.h"
#include "MPEG4BitrateBox.h"
#include "TimeToSampleBox.h"
#include "STTSEntry.h"
#include "SampleToChunkBox.h"
#include "STSCEntry.h"
#include "SampleSizeBox.h"
#include "ChunkOffsetBox.h"

#define C2I(a,b,c,d) ( ((a)<<24) | ((b)<<16) | ((c)<<8) | (d) )

namespace Boxes {
	enum BoxType {
		DINF = C2I('d', 'i', 'n', 'f'),
		DREF = C2I('d', 'r', 'e', 'f'),
		URL  = C2I('u', 'r', 'l', ' '),
		MDIA = C2I('m', 'd', 'i', 'a'),
		MDHD = C2I('m', 'd', 'h', 'd'),
		MDAT = C2I('m', 'd', 'a', 't'),
		MOOV = C2I('m', 'o', 'o', 'v'),
		MVHD = C2I('m', 'v', 'h', 'd'),
		MINF = C2I('m', 'i', 'n', 'f'),
		HDLR = C2I('h', 'd', 'l', 'r'),
		FREE = C2I('f', 'r', 'e', 'e'),
		FTYP = C2I('f', 't', 'y', 'p'),
		TRAK = C2I('t', 'r', 'a', 'k'),
		TKHD = C2I('t', 'k', 'h', 'd'),
		TREF = C2I('t', 'r', 'e', 'f'),
		VMHD = C2I('v', 'm', 'h', 'd'),
		STBL = C2I('s', 't', 'b', 'l'),
		STSD = C2I('s', 't', 's', 'd'),
		STTS = C2I('s', 't', 't', 's'),
		STSC = C2I('s', 't', 's', 'c'),
		STSZ = C2I('s', 't', 's', 'z'),
		STCO = C2I('s', 't', 'c', 'o'),

		/* AVC/H264 extension */
		AVC1 = C2I('a', 'v', 'c', '1'),
		AVCC = C2I('a', 'v', 'c', 'C'),
		BTRT = C2I('b', 't', 'r', 't'),
		SVC1 = C2I('s', 'v', 'c', '1'),
		SVCC = C2I('s', 'v', 'c', 'C'),

		/* Movie Fragment */
		MVEX = C2I('m', 'v', 'e', 'x'),
		MEHD = C2I('m', 'e', 'h', 'd'),
		MOOF = C2I('m', 'o', 'o', 'f'),
		TREX = C2I('t', 'r', 'e', 'x'),
		MFHD = C2I('m', 'f', 'h', 'd'),
		TRAF = C2I('t', 'r', 'a', 'f'),
		TFHD = C2I('t', 'f', 'h', 'd'),
		TRUN = C2I('t', 'r', 'u', 'n'),

		/* 3GPP Adaptive Streaming extensions */
		TFDT = C2I('t', 'f', 'd', 't'),
		SIDX = C2I('s', 'i', 'd', 'x'),

		SBAS = C2I('s', 'b', 'a', 's'),
		SCAL = C2I('s', 'c', 'a', 'l'),
		// REFT = C2I('R', 'E', 'F', 'T')
	};
}

#endif /* ALLBOXES_H */