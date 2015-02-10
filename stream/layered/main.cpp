#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;

class NalUnitAnchor {
public:
	int type;
	int offset;
	int length;
	int did, qid, tid;
	int idr;

	NalUnitAnchor() {
		clear();
	}

	// copy constructor
	NalUnitAnchor(const NalUnitAnchor& obj) {
		type = obj.type;
		offset = obj.offset;
		length = obj.length;
		did = obj.did;
		qid = obj.qid;
		tid = obj.tid;
		idr = obj.idr;
	}

	void clear() {
		type = offset = length = -1;
		did = qid = tid = idr = -1;
	}

	static int write(FILE*& fin, FILE*& fout, const NalUnitAnchor& curNal) {
		if (curNal.length <= 0 || curNal.offset < 0)
			return 1;

		char* buf = new char[curNal.length + 1];

		fseek(fin, curNal.offset, SEEK_SET);
		fread(buf, 1, curNal.length, fin);
		fwrite(buf, 1, curNal.length, fout);

		delete[] buf;
		return 0;
	}
};

vector<NalUnitAnchor> NalUnitAnchorList;

int extract_layers(FILE *infile) // I modified SJ Park(Korea University)'s code -by ES RYU-
		{
	int temp, forb, nri, type;
	int reserved_one_bit, idr_flag, priority_id, no_inter_layer_pre_flag;
	int dependency_id, quality_id, temporal_id, use_ref_base_pic_flag;
	int discardable_flag, output_flag, reserved_three_2bits;
	int status;
	unsigned long int offset, nal_size, index;

	char *str, *typeText;

	nal_size = 4;
	status = 0;
	offset = 0;
	index = 0;
	str = "";
	NalUnitAnchor curNalUnit;

	while ((temp = fgetc(infile)) != -1) {
		offset++;
		switch (status) {
		case 0:
		case 1:
		case 2: {
			if (temp == 0x00) // NAL Header first 3 byte = "0x00 00 00"
					{
				status++;
			} else {
				status = 0;
			}
		}
			break;

		case 3: {
			if (temp == 0x01) // NAL Header 4th byte = '0x01'
					{
				if (offset != 4) // deal with the first NAL Unit (No need to output offset)
						{
					nal_size = offset - nal_size; // NAL Size

					curNalUnit.length = nal_size;
					NalUnitAnchorList.push_back(curNalUnit);

					// printf("Size = %ld \n", nal_size);
					// printf("type=%d, offset=%d, len=%d, idr=%d, did=%d, qid=%d, tid=%d\n\n",
					// 		curNalUnit.type, curNalUnit.offset,
					// 		curNalUnit.length, curNalUnit.idr, curNalUnit.did,
					// 		curNalUnit.qid, curNalUnit.tid);

					nal_size = offset;
				}
				curNalUnit.clear();
				curNalUnit.offset = offset - 4;
				status++;
			} else {
				status = 0;
			}
			break;
		}

		case 4: // 1st byte of NAL Header [F(1) NRI(2) Type(5)]
		{
			forb = (temp & 0x80) >> 7;
			nri = (temp & 0x60) >> 5;
			type = temp & 0x1F;

			curNalUnit.type = type;

			switch (type) {
			case 1:
				typeText = "CS non-IDR ";
				break;
			case 5:
				typeText = "CS IDR     ";
				break;
			case 6:
				typeText = "SEI        ";
				break;
			case 7:
				typeText = "SPS        ";
				break;
			case 8:
				typeText = "PPS        ";
				break;
			case 14:
				typeText = "PREFIX RBSP";
				break;
			case 15:
				typeText = "SUBSET RBSP";
				break;
			case 20:
				typeText = "CS S Ext   ";
				break;
			default:
				typeText = "TYPE       ";
				break;
			}

			// printf("No=%d, Offset=%d, F=%d, NRI=%d, Type=%s, ", index, offset - 5, forb, nri, typeText);

			index++;

			if (type == 20 || type == 14) {
				status++; // To Next byte
			} else {
				status = 0;
			}
			break;
		}

		case 5: // 2nd byte of NAL Header [R(1) I(1) PRID(6)]
		{
			reserved_one_bit = (temp & 0x80) >> 7;
			idr_flag = (temp & 0x40) >> 6;
			priority_id = (temp & 0x3F);

			curNalUnit.idr = idr_flag;

			// printf("R=%d, I=%d, PRID=%d, ", reserved_one_bit, idr_flag, priority_id);

			status++;  // To next byte

			break;
		}

		case 6: //3rd byte of NAL Header [N(1) DID(3) QID(4)]
		{
			no_inter_layer_pre_flag = (temp & 0x80) >> 7;
			dependency_id = (temp & 0x70) >> 4;
			quality_id = (temp & 0x0F);

			curNalUnit.did = dependency_id;
			curNalUnit.qid = quality_id;

			// printf("N=%d, DID=%d, QID=%d ", no_inter_layer_pre_flag, dependency_id, quality_id);

			status++; // To next byte
			break;
		}

		case 7: //4th byte of NAL Header [TID(3) U(1) D(1) O(1) RR(2)]
		{
			temporal_id = (temp & 0xE0) >> 5;
			use_ref_base_pic_flag = (temp & 0x10) >> 4;
			discardable_flag = (temp & 0x08) >> 3;
			output_flag = (temp & 0x04) >> 2;
			reserved_three_2bits = (temp & 0x03);

			curNalUnit.tid = temporal_id;

			// printf("TID=%d, U=%d, D=%d, O=%d, RR=%d ", temporal_id,
			// 		use_ref_base_pic_flag, discardable_flag, output_flag,
			// 		reserved_three_2bits);

			status = 0;  // to the 1st byte of header again
			break;
		}
		} // end of switch
	} // end of while

	// write the latest one
	curNalUnit.length = offset - curNalUnit.offset;

	// NalUnitAnchorList.push_back(curNalUnit);
	// printf("type=%d, offset=%d, len=%d, idr=%d, did=%d, qid=%d, tid=%d\n\n",
	// 		curNalUnit.type, curNalUnit.offset, curNalUnit.length,
	// 		curNalUnit.idr, curNalUnit.did, curNalUnit.qid, curNalUnit.tid);

	// fclose(fout);
	return 0;
}

int chop_segment(FILE *fin, char* out_file_folder, char* prefix, int segment_no, int layerID)
{	
	bool 	withHeader 		= false;
	int 	IDRcounter 		= 0;

	int 	seg 			= segment_no;
	int 	skip 			= seg * 8;
	int 	until 			= skip + 8;

	char 	output_file[128];

	if (skip == 0) {
		withHeader = true;
		sprintf(output_file, "%s/%s_h_%d.264", out_file_folder, prefix, layerID);
	}
	else {
		withHeader = false;
		sprintf(output_file, "%s/%s_%d_%d.264", out_file_folder, prefix, seg, layerID);
	}

	// printf("Output File Name = %s with Segment no is %d and GOP from %d to %d\n", output_file, seg, skip, until);
	
	fseek(fin, 0, SEEK_SET);
	extract_layers(fin);
	// printf("extract layer\n");

	FILE* fout = fopen(output_file, "wb");
	if (!fout)
	{
		fprintf(stderr, "Open Error! Please Check Output file location \"%s\"\n", output_file);
		return 0;
	}
	// else
	// 	printf("Output file is opened\n");

	for (int i = 0; i < NalUnitAnchorList.size(); ++i) {
		if (NalUnitAnchorList[i].type == 6 && NalUnitAnchorList[i + 3].type == 20)
		{
			if (NalUnitAnchorList[i + 2].type == 5)
				IDRcounter++;

			// printf("frameNo = %d, IDRcounter = %d\n", i, IDRcounter);

			if (IDRcounter < skip) {
				// printf("skip\n");
				i += 5;
				continue;
			}
			if (IDRcounter == until) {
				// printf("break\n");
				break;
			}

			if (NalUnitAnchorList[i + 1].tid < 5) {
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i]);
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 1]);
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 2]);
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 3]);

				if (layerID >= 1)
					NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 4]);

				if (layerID >= 2)
					NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 5]);
			}
			i += 5;
		}
		else if (withHeader) {
			NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i]);
			printf("set header\n");
		}
	}

	fclose(fout);
	printf("Segment %d is saved, at %s\n\n", seg, output_file);
}

/*
 * argv[1] = mode (0: All segments, 1: Specific Segment)
 * argv[2] = Source video file
 * argv[3] = Output Segment folder
 * argv[4] = Segment Prefix
 * argv[5] = LayerID (0 is based layer ~ 2 is second enhancement layer)
 *
 * if mode == 1
 * 		argv[6] = Segment Number (0 ~ 70, 0 is init segment)
 */
int main(int argc, char** argv)
{
	char 	source_file[128];

	int		layerID 	= 0;
	int 	segmentNo 	= 0;

	if (argc < 2)
	{
		printf("Usage: a.out 0 <source file> <output folder> <output file prefix> <layerID>\n");
		printf("\t-> Generating all Segments\n");
		printf("Or\n");
		printf("Usage: a.out 1 <source file> <output folder> <output file prefix> <layerID> <Segment Number>\n");
		printf("\t-> Generating Secific Segment\n");
		return -1;
	}

	int mode = atoi(argv[1]);
	// Check
	if (mode == 0 && argc < 6)
	{
		fprintf(stderr, "Usage: a.out %d <source file> <output folder> <output file prefix> <layerID>\n", mode);
		return -1;
	}
	else if (mode == 1 && argc < 7)
	{
		fprintf(stderr, "Usage: a.out %d <source file> <output folder> <output file prefix> <layerID> <Segment Number>\n", mode);
		return -1;
	}

	// Set Layer ID
	layerID = atoi(argv[5]);
	if (layerID < 0 || layerID > 2) {
		fprintf(stderr, "Error! Layer ID value is between 0 and 2\n");
		return -1;
	}

	// Set Segment Number
	if (mode == 1)
	{
		segmentNo = atoi(argv[6]);
		if (segmentNo < 0 || segmentNo > 70) {
			fprintf(stderr, "Error! Segment number is between 0 (init segment) and 70\n");
			return -1;
		}
	}

	FILE* fin = fopen(argv[2], "rb");
	if (!fin)
	{
		fprintf(stderr, "Open Error! Please check Source file location \"%s\"\n", argv[2]);
		return -1;
	}
	else
		printf("Source video file is opened\n");

	/*
	 * Chop entire video
	 * ./a.out 0 [video_name] [output_file_folder] [output_file_prefix] [layerID (0~2)]
	 */
	if (mode == 0)
	{
		for (int seg = 0; seg <= 70; seg++)
		{
			chop_segment(fin, argv[3], argv[4], seg, layerID);		
		}
	}

	/*
	 * Chop one video with specific segment and layer.
	 * ./a.out 1 [video_name] [output_file_folder] [output_file_prefix] [layerID (0~2)] [segment number (0~70)]
	 */
	else if (mode == 1)
	{
		chop_segment(fin, argv[3], argv[4], segmentNo, layerID);
	}
	fclose(fin);
	printf("Close source file\n\n");
	
	return 0;
}
