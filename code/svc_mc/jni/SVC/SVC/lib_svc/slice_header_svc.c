/*****************************************************************************
*
*  Open SVC Decoder developped in IETR image lab
*
*
*
*              M�d�ric BLESTEL <mblestel@insa-rennes.Fr>
*              Mickael RAULET <mraulet@insa-rennes.Fr>
*              http://www.ietr.org/
*
*
*
*
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*
*
* $Id$
*
**************************************************************************/

//AVC Files
#include "type.h"
#include "data.h"
#include "bitstream.h"
#include "cavlc.h"
#include "slice_header.h"
#include "quantization.h"
#include "memory_management.h"



//SVC Files
#include "svc_type.h"
#include "svc_data.h"
#include "set_layer.h"
#include "SwitchAdress.h"
#include "pic_list_svc.h"
#include "ErrorDetection.h"
#include "slice_data_I_svc.h"
#include "SliceHeaderExtension.h"
#include "nal_unit_header_svc_extension.h"

#include <android/log.h>
#define  LOG_TAG    "slice_header_svc"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,	 LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, 	 LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, 	 LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, 	 LOG_TAG, __VA_ARGS__)



#ifdef ENABLE_PACKET_LOSS_DETECTION


void PacketDropInSlice (SLICE *Slice, int *EndOfSlice){
	// Packet drop detection algorithm
	// NOTE: this works with similarly sliced frame.
	// In the beginning there shouldn't be error until the cycle is ready

	if(!Slice -> slice_cycle_ready)
	{
		int i, first, tmp;

		// Set value to table
		Slice -> first_mb[Slice -> first_mb_index] = Slice -> first_mb_in_slice;

		// Check whether the cycle is ready
		if(Slice -> first_mb_index > 0)
		{
			first = Slice -> first_mb[0];
			for(i = 1; i < (Slice -> first_mb_index+1); i++)
			{
				tmp = Slice -> first_mb[i];
				// Slice cycle found
				if(tmp == first){
					Slice -> slice_cycle_ready = 1;
					Slice -> num_of_slices_per_frame = i;
					break;
				}
			}
		}

		Slice -> first_mb_index++;

		// Start index from the beginning if cycle is ready
		if(	Slice -> slice_cycle_ready ) {
			Slice -> first_mb_index = 0;
		}

		// for sanity check
		if(Slice -> first_mb_index == MAX_SLICES_PER_FRAME)
		{
			Slice -> slice_cycle_ready = 1; // table is full and no cycle found. packet loss detection not possible
		}
	}

	if (Slice -> slice_cycle_ready &&
		(Slice -> num_of_slices_per_frame != 0)) {

		if(Slice -> first_mb_index >= (Slice -> num_of_slices_per_frame)) {
			Slice -> first_mb_index = 0;
		}
		Slice -> first_mb_in_slice_expected = Slice -> first_mb[Slice -> first_mb_index];

		// Check whether expexted slice is the same than received
		if(Slice -> first_mb_in_slice_expected != Slice -> first_mb_in_slice)
		{
			//printf("err Slice -> first_mb_in_slice %i\n", Slice -> first_mb_in_slice);
			printf("err Slice -> first_mb_in_slice_expected %i\n", Slice -> first_mb_in_slice_expected);

			// increase index because of lost packet
			Slice -> first_mb_index++;

			// Packet loss found. Do packet loss error handling
			*EndOfSlice = 1;
		}
		Slice -> first_mb_index++;
	}
}

#endif



//Slice header for SVC extension

int slice_header_svc(int NbLayers, unsigned char *data, SPS *sps_id, PPS *pps_id, int *entropy_coding_flag,
					 W_TABLES *quantif, LIST_MMO *Current_pic, SPS *sps, PPS *pps,
					 int *position, SLICE *Slice, MMO *Mmo, LIST_MMO *RefPicListL0,
					 LIST_MMO *RefPicListL1, NAL *Nal, int *EndOfSlice, int *ImgToDisplay,
					 int *xsize, int *ysize, int *AddressPic, int *Crop)
{

	//This test allows to avoid crashing when
	//Sps and IDR frame are missing
	if(Mmo -> MemoryAllocation && Nal -> LayerId < NbLayers){
		PPS *pt_pps_id ;
		SPS *pt_sps_id ;
		int PicParameterId;
		int SeqParameterId;
		LAYER_MMO *LayerMmo = &Mmo -> LayerMMO [Nal -> LayerId];
		int LastFrameNum = LayerMmo -> frame_num;
		int PrevEndOfSlice = *EndOfSlice;
		int FirstMbInSlice = Slice -> first_mb_in_slice;


	#ifdef ERROR_DETECTION
		//Missing prefix (NalUnitType == 14)
		if((Nal -> NalUnitType == 1 || Nal -> NalUnitType == 5) && Nal -> DqId){
			Nal -> DqId = Nal -> LayerId =  Nal -> DependencyId = Nal -> QualityId =
			Nal -> TemporalId =	Nal -> UseRefBasePicFlag = Nal -> DiscardableFlag = 0;
			Nal -> no_inter_layer_pred_flag = 1;
		}
	#endif


		//Read header data
		Slice -> first_mb_in_slice = (short) read_ue(data, position);
	#ifdef ENABLE_PACKET_LOSS_DETECTION
		PacketDropInSlice(Slice, EndOfSlice);
	#endif

		Current_pic -> slice_type = Slice -> slice_type = (unsigned char) read_ue(data, position);
		if ( Slice -> slice_type > 4 ) {
			Current_pic -> slice_type = Slice -> slice_type -= 5 ;
		}

		PicParameterId = read_ue(data, position);
		Nal -> pic_parameter_id[Nal -> LayerId] = PicParameterId = CLIP3(0, PPS_BUF_SIZE - 1, PicParameterId);
		pt_pps_id = &pps[PicParameterId];
		//Should be in slice header
		//Sometimes Subset SPS are after PPS (rtsp)
		if(pt_pps_id -> seq_parameter_set_id > sps[0] . MaxSpsId){
			pt_pps_id -> seq_parameter_set_id = sps[0] . MaxSpsId;
		}

		SeqParameterId = pt_pps_id -> seq_parameter_set_id + (Nal -> LayerId ? 16: 0);
		pt_sps_id = &sps[SeqParameterId];
		LayerMmo -> frame_num = (short) getNbits(data, position, pt_sps_id -> log2_max_frame_num );
		*entropy_coding_flag = pt_pps_id -> entropy_coding_mode_flag;

		//EndOfSlice is set to one, if the VLD has detected that not enough decoded macroblock,
		//And that the the frame_num did not change, and if we are still in the same layer
		if( Nal -> DqId > Nal -> DqIdToDisplay){
			//Layer not decoded, so PrevEndOfSlice not taken into account
			*EndOfSlice = (LayerMmo -> frame_num == LastFrameNum && FirstMbInSlice != Slice -> first_mb_in_slice && Nal -> DqId == Nal -> LastDqId)? 1: 0;
		}else{
			*EndOfSlice = (PrevEndOfSlice && LayerMmo -> frame_num == LastFrameNum && FirstMbInSlice != Slice -> first_mb_in_slice && Nal -> DqId == Nal -> LastDqId)? 1: 0;
		}

	#ifdef ERROR_DETECTION
		//Sps problem
		if(!pt_sps_id -> PicSizeInMbs){
			Nal -> NalToSkip = 1;
			Nal -> PicToDecode = Nal -> PicToDisplay = 0;
			return 0;
		}
		// following function are call FinishFrameSVCError() in FinishFramSVC.c
		DetectSliceError(Nal, sps, pps, Current_pic, Mmo, *EndOfSlice, PrevEndOfSlice, Crop, ImgToDisplay, AddressPic, xsize, ysize);
		DetecMissingTopLayer(Nal, sps, pps, Current_pic, Mmo, Crop, ImgToDisplay, AddressPic, xsize, ysize);
	#endif

		Mmo -> LayerId = Nal -> LayerId;

		//Read Frame parameter
		FrameHeaderData(data, position, Nal -> NalRefIdc, Nal -> IdrFlag, Nal -> LayerId, *EndOfSlice,
			Current_pic, LayerMmo, pt_sps_id, pt_pps_id, Mmo, RefPicListL0, RefPicListL1);


		//Compute POC and get memory addresses
		if ( !*EndOfSlice){
			pic_order_process(pt_sps_id, Nal -> IdrFlag * 5, Nal -> NalRefIdc, Mmo, LayerMmo, Current_pic, RefPicListL0, RefPicListL1);
	#ifdef ERROR_DETECTION
			DetectPocProblem(Nal, Mmo, Current_pic, LayerMmo, pt_sps_id);
	#endif
			ProvideMemAddress(Current_pic, Mmo, LayerMmo);
		}

		if (!*EndOfSlice){
			//Allow to switch of layer when first slice
			switch_layer(Nal);
		}


		//Save the memory
		Nal -> LastMemoryAddress = Nal -> MemoryAddress;
		Nal -> MemoryAddress = Current_pic -> MemoryAddress;
		Nal -> MvMemoryAddress = Current_pic -> MvMemoryAddress;

		//Get the reference listes
		SvcReadReferenceListProcessing(data, position, Nal, Slice, pt_sps_id, pt_pps_id, LayerMmo, Current_pic, Mmo, RefPicListL0, RefPicListL1);

		if (!*EndOfSlice){
			direct_ref_list_init(Slice, RefPicListL0, RefPicListL1, Current_pic, LayerMmo);
		}

		//Parse Qp, deblocking information and inter-layer prediction.
		QpAndDeblockProcessing(data, position, Slice, pt_pps_id, pt_sps_id);
		SvcReadLayerPrediction(data, position, Nal, pt_sps_id, pt_pps_id, Slice, Current_pic, Mmo, *EndOfSlice);

		//Initialize up-sampling parameters
		NalParamInit(Nal, pps, sps, pt_sps_id);
		if(Nal -> LayerId && !*EndOfSlice && Nal -> UsedSpatial [Nal -> BaseLayerId][0] ==  -1){
			SetSpatialUsed(Nal);
		}

		//Initialize the Nal decoding process parameters
		SetPicToDisplay(Nal);
		IsPicToDecode(Nal);

		//Read default parameters for VLD.
		SvcReadDefaultParameters(data, position, Nal, Slice, pt_sps_id);


		Nal -> SpatialScalabilityType = SpatialScalabilityType(sps_id, Nal);
		init_quantization (quantif, pt_sps_id -> scaling_matrix_present, pt_pps_id -> scaling_matrix4, pt_pps_id -> scaling_matrix8 );


		//Ghost Picture Detected
		if( Nal -> NalToSkip){
			Current_pic -> displayed = 255;
		}

		Nal -> LastLayerId = Nal -> LayerId;
		Nal -> LastDqId = Nal -> DqId;
		Nal -> LastTemporalId = Nal -> TemporalId;
		Mmo -> nb_img_display = sps[SeqParameterId] . nb_img_disp;
		Slice -> slice_num++ ;
		*pps_id = pps[PicParameterId];
		*sps_id = sps[SeqParameterId];
	}
	else {
		Nal -> NalToSkip = 1;
		Nal -> PicToDecode = Nal -> PicToDisplay = 0;

		//
		Current_pic -> MemoryAddress = Mmo -> FreeMemoryAddress [Mmo -> NumFreeAddress - 1];
		Mmo -> FreeMemoryAddress  [Mmo -> NumFreeAddress - 1] = -1;
		Mmo -> NumFreeAddress--;
	}

	return 0;
}



