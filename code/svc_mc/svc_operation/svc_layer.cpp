#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
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
	//copy constructor
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

int extract_layers(FILE *infile) //I modified SJ Park(Korea University)'s code -by ES RYU-
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
					nal_size = offset - nal_size; //NAL Size

					curNalUnit.length = nal_size;
					NalUnitAnchorList.push_back(curNalUnit);

//					printf("Size = %ld \n", nal_size);
//					printf(
//							"type=%d, offset=%d, len=%d, idr=%d, did=%d, qid=%d, tid=%d\n\n",
//							curNalUnit.type, curNalUnit.offset,
//							curNalUnit.length, curNalUnit.idr, curNalUnit.did,
//							curNalUnit.qid, curNalUnit.tid
//						);
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

//			printf("No=%d, Offset=%d, F=%d, NRI=%d, Type=%s, ", index,
//					offset - 5, forb, nri, typeText);
			index++;

			if (type == 20 || type == 14) {
				status++; // To Next byte
			} else {
				status = 0;
			}
			break;
		}

		case 5: //2nd byte of NAL Header [R(1) I(1) PRID(6)]
		{
			reserved_one_bit = (temp & 0x80) >> 7;
			idr_flag = (temp & 0x40) >> 6;
			priority_id = (temp & 0x3F);

			curNalUnit.idr = idr_flag;

//			printf("R=%d, I=%d, PRID=%d, ", reserved_one_bit, idr_flag,
//					priority_id);
			status++;  //To next byte

			break;
		}

		case 6: //3rd byte of NAL Header [N(1) DID(3) QID(4)]
		{
			no_inter_layer_pre_flag = (temp & 0x80) >> 7;
			dependency_id = (temp & 0x70) >> 4;
			quality_id = (temp & 0x0F);

			curNalUnit.did = dependency_id;
			curNalUnit.qid = quality_id;

//			printf("N=%d, DID=%d, QID=%d ", no_inter_layer_pre_flag,
//					dependency_id, quality_id);
			status++; //To next byte
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
//
//			printf("TID=%d, U=%d, D=%d, O=%d, RR=%d ", temporal_id,
//					use_ref_base_pic_flag, discardable_flag, output_flag,
//					reserved_three_2bits);

			status = 0;  //to the 1st byte of header again
			break;
		}
		} // end of switch
	} //end of while

	//write the latest one
	curNalUnit.length = offset - curNalUnit.offset;
//	NalUnitAnchorList.push_back(curNalUnit);
//	printf("type=%d, offset=%d, len=%d, idr=%d, did=%d, qid=%d, tid=%d\n\n",
//			curNalUnit.type, curNalUnit.offset, curNalUnit.length,
//			curNalUnit.idr, curNalUnit.did, curNalUnit.qid, curNalUnit.tid);

	//fclose(fout);
	return 0;
}

/*
 * argv[1] = file source
 * argv[2] = output path
 */
int main(int argc, char** argv) {
	int IDRcounter = 0;
	bool withHeader = false;
	int enhance = 0;
	int seg = 0;
	int skip = 0;
	int until = 0;
	char prefix_name[20];



//	int seg = 0;
//	= atoi(argv[2]);

//	int skip = 0;
//	= seg * 8;

//	int until = 0;
//	= skip + 8;

	/*
	 * Chop entire video
	 * ./a.out [video_name] [layerID (0~2)]
	 */
	/*
	char output_path[50];
	char seg_no[3];
	char source_file[50];
//	= {"./videos/jeux.264"};

	sprintf(prefix_name, "%s", argv[1]);
	sprintf(source_file, "./videos/%s.264", prefix_name);
	enhance = atoi(argv[2]);

	for (seg = 0; seg <= 70; seg++)
	{
		skip = seg * 8;
		until = skip + 8;

		IDRcounter = 0;

		if (skip == 0) {
			withHeader = true;
			sprintf(output_path, "/var/www/%s/%s_h_%d.264",prefix_name, prefix_name, enhance);
		}
		else {
			withHeader = false;
			sprintf(output_path, "/var/www/%s/%s_%d_%d.264",prefix_name,prefix_name, seg, enhance);
		}

		printf("file name = %s, segment no %d from %d to %d\n", output_path, seg, skip, until);

		FILE* fin = fopen(source_file, "rb");
		printf("open read file\n");
		extract_layers(fin);
		printf("extract layer\n");
		FILE* fout = fopen(output_path, "wb");
		printf("open write file\n");

		for (int i = 0; i < NalUnitAnchorList.size(); ++i) {
			if (NalUnitAnchorList[i].type == 6 && NalUnitAnchorList[i + 3].type == 20)
			{
				if (NalUnitAnchorList[i + 2].type == 5)
					IDRcounter++;

				printf("frameNo = %d, IDRcounter = %d\n", i, IDRcounter);

				if (IDRcounter < skip) {
					printf("skip\n");
					i += 4;
					continue;
				}
				if (IDRcounter == until) {
					printf("break\n");
					break;
				}

				if (NalUnitAnchorList[i + 1].tid < 5) {
					NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i]);
					NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 1]);
					NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 2]);
					NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 3]);

					if (enhance >= 1)
						NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 4]);

					if (enhance >= 2)
						NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 5]);
				}
				i += 5;
			}
			else if (withHeader) {
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i]);
				printf("set header\n");
			}
		}

		fclose(fin);
		fclose(fout);
		printf("close read and write files\n\n");
	}
	*/

	/*
	 * Chop one video with specific segment and layer.
	 * ./a.out [source file path] [des file path] [segmentNo (0~70)] [layerID (0~2)]
	 */
//	/*
	FILE* fin = fopen(argv[1], "rb");
	extract_layers(fin);
	FILE* fout = fopen(argv[2], "wb");
	seg = atoi(argv[3]);
	skip = seg * 8;
	until = skip + 8;
	enhance = atoi(argv[4]);

//	sprintf(output_path, "/var/www/doc-reality/doc-reality_%d_%d.264", seg, enhance);
//	FILE* fout = fopen(output_path, "wb");

	for (int i = 0; i < NalUnitAnchorList.size(); ++i) {
		//---- 6: SEI 描述資訊, 20: coded slice ----//
		//---- check weather this a frame or not -//
		if (NalUnitAnchorList[i].type == 6 && NalUnitAnchorList[i + 3].type == 20)
		{
			// is IDR means begin of GOP
			if (NalUnitAnchorList[i + 2].type == 5)
				IDRcounter++;

			printf("frameNo = %d, IDRcounter = %d\n", i, IDRcounter);

			//---- this IDR (or GOP) we don't want ----//
			if (IDRcounter < skip) {
				printf("skip\n");
				i += 5;
				continue;
			}
			//---- until this IDR (or GOP) end of chop ----//
			if (IDRcounter == until) {
				printf("break\n");
				break;
			}

			// if temporal ID < 5 we skip this frame
			if (NalUnitAnchorList[i + 1].tid < 5) {
				//---- base layer from i to i+3 ----//
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i]);
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 1]);
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 2]);
				NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 3]);

				//---- first enhancement layer plus i+4  ----//
				if (enhance >= 1)
					NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 4]);

				//---- second enhancement layer plus i+5  ----//
				if (enhance >= 2)
					NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i + 5]);
			}
			i += 5;
		}
		else if (withHeader) {
			NalUnitAnchor::write(fin, fout, NalUnitAnchorList[i]);
			printf("set header\n");
		}
	}
//	printf("total GOP size= %d\n", IDRcounter);
	printf("file name = %s with segment: %d, layer: %d\n", argv[2], seg, enhance);

	fclose(fin);
	fclose(fout);
//	*/

	return 0;
}
