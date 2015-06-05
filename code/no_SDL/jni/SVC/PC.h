#ifndef H_PC_H
#define H_PC_H

#include <android/bitmap.h>
#include <sys/time.h>
#include <sys/timeb.h>

#include "type.h"
#include "main_data.h"
#include "svc_type.h"
#include "Read_h264.h"

/* Thread Handler */
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#define CRITICAL_SECTION    pthread_mutex_t
#define CONDITION_VARIABLE  pthread_cond_t

#define PortableSleep(seconds)						usleep(seconds * 1000000)
#define JoinThread(handle)							pthread_join(*(handle), NULL)
#define InitializeCriticalSection(mutex_p)			pthread_mutex_init(mutex_p, NULL)
#define DeleteCriticalSection(mutex_p)				pthread_mutex_destroy(mutex_p)
#define EnterCriticalSection(mutex_p)				pthread_mutex_lock(mutex_p)
#define LeaveCriticalSection(mutex_p)				pthread_mutex_unlock(mutex_p)
#define InitializeConditionVariable(cond_p)			pthread_cond_init(cond_p, NULL)
#define DeleteConditionVariable(cond_p)				pthread_cond_destroy(cond_p)
#define WaitConditionVariableCS(cond_p, mutex_p)	pthread_cond_wait(cond_p, mutex_p)
#define WakeConditionVariable(cond_p)				pthread_cond_signal(cond_p)
#define WakeAllConditionVariable(cond_p)			pthread_cond_broadcast(cond_p)

/* Type Definded */
#define uchar 			unsigned char
#define ushort 			unsigned short
#define ulong 			unsigned long
#define uint 			unsigned int
#define prec_synchro 	int
#define stream 			unsigned char
#define image_type 		unsigned char
#define dpb_type 		unsigned char

typedef unsigned char 		byte; 	// Byte is a char
typedef unsigned short int 	word16; // 16-bit word is a short int
typedef unsigned int 		word32; // 32-bit word is an int

/* Ring Buffer size Defended */
#define StopDownloadSpace 	15728640	// Not used, we want to use this to lock request thread. 			Test Value: 12582912, 5242880, 15728640
#define StopDecodeSpace 	1572864 	// when available data size small than this value will be locked. 	Test Value: 2097152, 1572864
#define MaxFileSize	 		20971520	// the size of ringBuffer called StreamBuffer. 						Test Value: 20971520 (Default : 10185706)
#define RepeatSegments 		70			// 70 is total segments of test file

typedef struct {
	image_type Display_1_Extract_1_Image_Y_o[3279368];
	image_type Display_1_Extract_Image_Y_o[3279368];
	NAL DqIdNextNal_Nal_o[1];
	NALSTREAMER GetNalBytesAu_NalStreamer[1];
	PPS GetNalBytesAu_PPS[255];
	SPS GetNalBytesAu_SPS[32];
	ConfigSVC GetNalBytesAu_StreamType[1];
	int GetNalBytesAu_rbsp_o_size[1];
	stream GetNalBytesAu_rbsp_o[1622016];
	uchar ReadAU_DataFile_o[1622016];
	int ReadAU_ReadBytes[1];
	int ReadAU_pos_o[1];
	short decoder_svc_MvBuffer_1_Mv[11489280];
	short decoder_svc_MvBuffer_1_Ref[5744640];
	short decoder_svc_MvBuffer_Mv[11489280];
	short decoder_svc_MvBuffer_Ref[5744640];
	int decoder_svc_NalUnit_NalRefIdc_io[1];
	int decoder_svc_NalUnit_NalUnitType_io[1];
	int decoder_svc_Nal_Compute_NalDecodingProcess_Set_Pos_Pos[1];
	int decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_entropy_coding_flag[1];
	PPS decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_pps_id[1];
	W_TABLES decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_quantif[1];
	SPS decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_sps_id[1];
	int decoder_svc_Nal_Compute_NalDecodingProcess_Slice_type_SliceType_o[1];
	short decoder_svc_Nal_Compute_NalDecodingProcess_slice_layer_CABAC_mv_cabac_l0_o[261120];
	short decoder_svc_Nal_Compute_NalDecodingProcess_slice_layer_CABAC_mv_cabac_l1_o[261120];
	short decoder_svc_Nal_Compute_NalDecodingProcess_slice_layer_CABAC_ref_cabac_l0_o[32640];
	short decoder_svc_Nal_Compute_NalDecodingProcess_slice_layer_CABAC_ref_cabac_l1_o[32640];
	int decoder_svc_Nal_Compute_SetPos_Pos[1];
	int decoder_svc_Nal_Compute_SliceHeaderIDR_entropy_coding_flag[1];
	PPS decoder_svc_Nal_Compute_SliceHeaderIDR_pps_id[1];
	W_TABLES decoder_svc_Nal_Compute_SliceHeaderIDR_quantif[1];
	SPS decoder_svc_Nal_Compute_SliceHeaderIDR_sps_id[1];
	int decoder_svc_Nal_Compute_nal_unit_header_svc_ext_20_pos_o[1];
	SEI decoder_svc_Nal_Compute_sei_rbsp_Sei[1];
	int decoder_svc_Nal_Compute_seq_parameter_set_IdOfsps_o[1];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_Upsampling_tmp[2088960];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_px[1920];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_py[1088];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_xk16[1920];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_xp16[1920];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_yk16[1088];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_yp16[1088];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_Upsampling_tmp[2088960];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_px[1920];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_py[1088];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_xk16[1920];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_xp16[1920];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_yk16[1088];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_yp16[1088];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_Layer_SliceLayerCabac_mv_cabac_l0_o[261120];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_Layer_SliceLayerCabac_mv_cabac_l1_o[261120];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_Layer_SliceLayerCabac_ref_cabac_l0_o[32640];
	short decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_Layer_SliceLayerCabac_ref_cabac_l1_o[32640];
	int decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_type_SliceType_o[1];
	int decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_entropy_coding_flag[1];
	PPS decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_pps_id[1];
	W_TABLES decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_quantif[1];
	SPS decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_sps_id[1];
	short decoder_svc_Nal_Compute_slice_layer_main_CABAC_mv_cabac_l0_o[261120];
	short decoder_svc_Nal_Compute_slice_layer_main_CABAC_mv_cabac_l1_o[261120];
	short decoder_svc_Nal_Compute_slice_layer_main_CABAC_ref_cabac_l0_o[32640];
	short decoder_svc_Nal_Compute_slice_layer_main_CABAC_ref_cabac_l1_o[32640];
	uchar decoder_svc_PictureBuffer_RefU[12570880];
	uchar decoder_svc_PictureBuffer_RefV[12570880];
	uchar decoder_svc_PictureBuffer_RefY[50283520];
	short decoder_svc_ResiduBuffer_RefU[2186240];
	short decoder_svc_ResiduBuffer_RefV[2186240];
	short decoder_svc_ResiduBuffer_RefY[8744960];
	DATA decoder_svc_Residu_Block[8160];
	LIST_MMO decoder_svc_Residu_Current_pic[1];
	uchar decoder_svc_Residu_MbToSliceGroupMap[8160];
	MMO decoder_svc_Residu_Mmo[1];
	PPS decoder_svc_Residu_PPS[255];
	LIST_MMO decoder_svc_Residu_RefL0[16];
	LIST_MMO decoder_svc_Residu_RefL1[16];
	RESIDU decoder_svc_Residu_Residu[32640];
	SPS decoder_svc_Residu_SPS[32];
	SLICE decoder_svc_Residu_Slice[1];
	uchar decoder_svc_Residu_SliceGroupId[8160];
	uchar decoder_svc_Residu_SliceTab[8160];
	SVC_VECTORS decoder_svc_Svc_Vectors_PC_Svc_Vectors[1];
	int decoder_svc_VideoParameter_Crop[1];
	int decoder_svc_VideoParameter_EndOfSlice[1];
	int decoder_svc_VideoParameter_ImgToDisplay[1];
	int decoder_svc_VideoParameter_address_pic_o[1];
	int decoder_svc_VideoParameter_xsize_o[1];
	int decoder_svc_VideoParameter_ysize_o[1];
	VLC_TABLES decoder_svc_VlcTab_PC_o[1];
	MAIN_STRUCT_PF decoder_svc_slice_main_vector_PC_Main_vector_o[1];
	int iFrame;
	int EndOfStream;
} SVCinfo;

// Frame Structure
typedef struct {
	unsigned char* RGB;
} EachFrame;

// Frame Store for each Thread
typedef struct _FrameStore {
	int frameAllocated;
	int frameCounter;
	int displayCounter;
	int edge;
	int frameWidth, frameHeight;
	int fps;
	int IDRindex;
	int waitForDisplay;
	EachFrame frameList[100]; // hard code it, I don't want to realloc it, there is no vector in C...

	// struct timeb start_decode_time;
	// struct timeb end_decode_time;
	struct timeval start_decode_time;
	struct timeval end_decode_time;
	unsigned long long tempDecodingTime;
	unsigned long long decodingTimeMS;

	// do what member function doing in C++...
	void (*frameInit)(struct _FrameStore* thz, EachFrame* newFrame);
	void (*frameDestory)(struct _FrameStore* thz);
	void (*Display)(struct _FrameStore* thz);
	void (*DisplaySingle)(struct _FrameStore* thz);
	int  (*fillFrame)(struct _FrameStore* thz, unsigned char* Y, unsigned char* U, unsigned char* V);
	int  (*isEmpty)(struct _FrameStore* thz);
	void (*checkResolution)(struct _FrameStore* thz);
} FrameStore;


/* Multi-Threading */
// #define THREAD_NUM 3
// static pthread_t ThreadList[THREAD_NUM];
static int 				THREAD_NUM		= 1;
static pthread_t 		*ThreadList 	= NULL;
static pthread_t 		request_thread;

CRITICAL_SECTION FrameCounterMutex = PTHREAD_MUTEX_INITIALIZER; // protect race condition when set the FramecCounter

static CRITICAL_SECTION DownloadMutex 		= PTHREAD_MUTEX_INITIALIZER;	// lock until free size of StreamBuffer can put new data
static CONDITION_VARIABLE CanDownloadCond 	= PTHREAD_COND_INITIALIZER;		// related condition

static CRITICAL_SECTION StreamBufferMutex 	= PTHREAD_MUTEX_INITIALIZER;	// lock until data enough
static CONDITION_VARIABLE CanPlayCond 		= PTHREAD_COND_INITIALIZER;		// related condition

//static CRITICAL_SECTION FirstDownloadMutex = PTHREAD_MUTEX_INITIALIZER;
//static CONDITION_VARIABLE FirstDownloadCond = PTHREAD_COND_INITIALIZER;

CRITICAL_SECTION BufferReallocMutex = PTHREAD_MUTEX_INITIALIZER;			// protect race condition when copy downloaded data to StreamBuffer
CRITICAL_SECTION DataSizeMutex 		= PTHREAD_MUTEX_INITIALIZER;			// protect race condition when set the available data size
CRITICAL_SECTION EnhIDRMutex 		= PTHREAD_MUTEX_INITIALIZER;			// protect when set enh_IDRindex

static CRITICAL_SECTION 	DirtyMutex 			= PTHREAD_MUTEX_INITIALIZER;
static CRITICAL_SECTION 	CurrentDisplayMutex = PTHREAD_MUTEX_INITIALIZER;
static CRITICAL_SECTION 	FrameBufferMutex 	= PTHREAD_MUTEX_INITIALIZER;
static CONDITION_VARIABLE 	CurrentDisplayCond 	= PTHREAD_COND_INITIALIZER;
static CONDITION_VARIABLE 	FrameBufferCond 	= PTHREAD_COND_INITIALIZER;

//static char* FILE_PATH;	// Get file segment from HTTP Get request
/* Used for requesting segment */
int 	data_size 		= 0;
int 	displayTimes 	= 0;
char 	*seqBuf 		= NULL;
char 	*prefix 		= NULL;
char 	*base_url 		= NULL;

/* For Switch Event */
static int 		LAYER_ID;
static int 		TEMPORAL_ID;
static int 		*Temporal_changed; 		// if is True then change temporal ID
static int 		*Layer_changed;			// if it's True then change layer ID
static int 		download_enhance;		// set number means to start downloading which enhancement layer.
static int 		pre_download_enhance; 	// set the number previous enhancement layer.
static int 		CurrentDisplay = 0;
/*
 * indicate when will switch to enhancement layer
 * if enh_IDRindex[0] is True, then can switch to enhancement layer one.
 * if enh_IDRindex[1] is True, then can switch to enhancement layer two.
 */
static int enh_IDRindex[2];

/* For Measurement */
/* Decode */
// static long time_start;
// static struct timeval 	*start_decode_time	= NULL;
// static struct timeval 	*end_decode_time	= NULL;
// static int 				*setStartDecodeTime	= NULL;
/* Network */
static struct timeval 	time_start;	// Used for getting network delay
long 					network_delay 	= 0;
static float 			totalDataSize 	= 0;
static struct timeval	time_start_download;

static int i;
static int init = 0;
static void* pixels[64];
static AndroidBitmapInfo info;
static int getMinBytes = 1622016;	// default 1622016, each run will get this to do some operations.
static int framecounter;
static int bitmapFilledCounter, bitmapFilledMax;
static int flagError;
//static FrameStore FrameStoreList[THREAD_NUM];
static FrameStore* FrameStoreList;
//static SVCinfo SVCinfoList[THREAD_NUM];
static SVCinfo* SVCinfoList;
static int ThreadFinishSignal;
//static sem_t ReturnSem;

// Bear for stack size
pthread_attr_t attr;
int firstDecodeThread = -1;

/* SVC Decoder Functions */
void init_svc_vectors(SVC_VECTORS *svc);
void decode_init_vlc(VLC_TABLES *VLc);
void vector_main_init(MAIN_STRUCT_PF *pf);
//void readh264_init(int argc,char **argv);
void get_layer(int argc, char **argv, ConfigSVC *NumLayer);
void choose_layer(ConfigSVC *NumLayer);
void ParseFirstAU(int StreamSize, uchar *Buffer, ConfigSVC *StreamType);
int GetNalBytesAuSVC(uchar *data, stream *rbsp, int *NalInRbsp, ConfigSVC *StreamType, NALSTREAMER *NalSize, SPS *Sps, PPS *Pps);
void init_int(int *tab);
void init_mmo(int num_of_layers, MMO *mmo_stru);
void init_slice(SLICE *slice);
void init_pps(PPS *sps);
void init_sps(SPS *sps);
void InitListMmo(LIST_MMO *RefPicListL0);
void slice_header_svc(int NbLayers, const stream *data, SPS *sps_id,
		PPS *pps_id, int *entropy_coding_flag, W_TABLES *quantif,
		LIST_MMO *current_pic, SPS *sps, PPS *pps, int *position, SLICE *slice,
		MMO *mmo, LIST_MMO RefPicListL0[], LIST_MMO RefPicListL1[], NAL *nal,
		int *end_of_slice, int *ImgToDisplay, int *xsize, int *ysize,
		int *AddressPic, int *Crop);
void pic_parameter_set(stream *data, uchar *ao_slice_group_id, PPS *pps, SPS *sps, const int NalBytesInNalunit);
void decoderh264_init(const int pic_width, const int pic_height);
void Init_SDL(int edge, int frame_width, int frame_height);
void SDL_Display(int edge, int frame_width, int frame_height, unsigned char *Y, unsigned char *V, unsigned char *U);
void CloseSDLDisplay();
void ReadAuH264(const int nb_octets_to_read, uchar *buffer, int ReadnewBytes, int *nb_octet_already_read);
void NextNalDqId(ConfigSVC *Buffer, NAL *NAL);
void init_nal_struct(NAL *nal, unsigned char NumOfLayer);
void NalUnitSVC(stream *data_in, int *nal_unit_type, int *nal_ref_idc, NAL *Nal);
void init_int(int *tab);
void svc_calculate_dpb(const int total_memory, const int mv_memory, int nb_of_layers, MMO *mmo_struct, SPS *sps, NAL *Nal);
void sei_rbsp(stream *data, int NalInRbsp, SPS *sps, SEI *Sei);
void seq_parameter_file_sizeset(stream *data, SPS *sps);
void FlushSVCFrame(SPS *sps, NAL *nal, MMO *mmo, int *address_pic, int *x_size, int *y_size, int *Crop, int *img_to_display);
void PrefixNalUnit(stream *data, int *NalinRbsp, NAL *nal, MMO *mmo, SPS *sps, int *EndOfSlice);
void subset_sps(stream *data, int * NalInRbsp, SPS *sps, NAL *nal);
void NalUnitHeader(const stream *data, int *pos, NAL *nal, int *EndOfSlice);
void slice_data_in_scalable_extension_cavlc(const int size_mb,
		const stream *ai_pcData, int * NalInRbsp, const int *ai_piPosition,
		const NAL *nal, const SPS *ai_pstSps, PPS *ai_pstPps,
		const VLC_TABLES *vlc, uchar *ai_slice_group_id, SLICE *aio_pstSlice,
		uchar *aio_tiMbToSliceGroupMap, uchar *aio_tiSlice_table,
		DATA *aio_tstTab_block, RESIDU *residu, int * aio_piEnd_of_slice);
void SliceCabac(const int size_mb, uchar *data, int *position,
		int *NalBytesInNaluniinit_nal_structt, const NAL *Nal, SPS *sps, PPS *pps,
		uchar *ai_slice_group_id, short *mv_cabac_l0, short *mv_cabac_l1,
		short *ref_cabac_l0, short *ref_cabac_l1, SLICE *slice,
		uchar *MbToSliceGroupMap, uchar *slice_table, DATA *Tab_block,
		RESIDU *picture_residu, int *end_of_slice);
void slice_base_layer_cavlc(const stream *ai_pcData, int * NalInRbsp,
		const int *ai_piPosition, const SPS *ai_pstSps, PPS *ai_pstPps,
		const VLC_TABLES *Vlc, uchar *ai_slice_group_id, LIST_MMO *Current_pic,
		LIST_MMO *RefListl1, NAL *Nal, SLICE *aio_pstSlice,
		uchar *aio_tiMbToSliceGroupMap, uchar *aio_tiSlice_table,
		DATA *aio_tstTab_block, RESIDU *picture_residu,
		int * aio_piEnd_of_slice, short *mv_io, short *mvl1_io, short *ref_io,
		short *refl1_io);
void slice_base_layer_cabac(uchar *data, int *position, int *NalBytesInNalunit,
		SPS *sps, PPS *pps, uchar *ai_slice_group_id, LIST_MMO *Current_pic,
		LIST_MMO *RefListl1, NAL *Nal, short *mv_cabac_l0, short *mv_cabac_l1,
		short *ref_cabac_l0, short *ref_cabac_l1, SLICE *slice,
		uchar *MbToSliceGroupMap, uchar *slice_table, DATA *Tab_block,
		RESIDU *picture_residu, int *end_of_slice, short *mvl0_io,
		short *mb_l1_io, short *refl0_io, short *refl1_io);
void Decode_P_avc(const SPS *ai_pstSps, const PPS *ai_pstPps,
		const SLICE *ai_pstSlice, const uchar *ai_tiSlice_table,
		const RESIDU *picture_residu, const STRUCT_PF *pf,
		const LIST_MMO *ai_pstRefPicListL0, const LIST_MMO *ai_pstCurrent_pic,
		W_TABLES *quantif_tab, NAL *Nal, short *aio_tiMv, short *aio_tiRef,
		uchar *aio_tucDpb_luma, uchar *aio_tucDpb_Cb, uchar *aio_tucDpb_Cr,
		short *Residu_Img, short *Residu_Cb, short *Residu_Cr);
void Decode_B_avc(SPS *ai_stSps, PPS *ai_stPps, SLICE *ai_stSlice,
		uchar *ai_tSlice_table, RESIDU *picture_residu,
		MAIN_STRUCT_PF *main_vector, LIST_MMO *ai_pRefPicListL0,
		LIST_MMO *ai_pRefPicListL1, LIST_MMO *ai_pCurrent_pic,
		W_TABLES *quantif, NAL *Nal, short *aio_tMv_l0, short *aio_tMv_l1,
		short *aio_tref_l0, short *aio_tref_l1, uchar *aio_tDpb_luma,
		uchar *aio_tDpb_Cb, uchar *aio_tDpb_Cr, short *Residu_img,
		short *Residu_Cb, short *Residu_Cr);
void Decode_I_avc(SPS *sps, PPS *pps, SLICE *slice, uchar *slice_table,
		RESIDU *picture_residu, STRUCT_PF *pf, W_TABLES *quantif_tab, NAL *Nal,
		uchar *image, uchar *image_Cb, uchar *image_Cr);
void FinishFrameSVC(const int NbMb, NAL *Nal, SPS *Sps, PPS *Pps,
		LIST_MMO *Current_pic, SLICE *Slice, int EndOfSlice, uchar *SliceTab,
		DATA *TabBlbock, RESIDU *Residu, short *MvL0, short *MvL1, short *RefL0,
		short *RefL1, int *Crop, int *ImgToDisplay, int *AdressPic, MMO *Mmo,
		unsigned char *RefY, unsigned char *RefU, unsigned char *RefV,
		int *xsize, int *ysize);
void Decode_P_svc(const int size, const SPS *ai_pstSps, const PPS *ai_pstPps,
		const SLICE *ai_pstSlice, const NAL *nal, const uchar *ai_tiSlice_table,
		const DATA *ai_tstTab_Block, RESIDU *residu, STRUCT_PF *baseline_vector,
		const LIST_MMO *ai_pstRefPicListL0, const LIST_MMO *ai_pstCurrent_pic,
		W_TABLES *quantif_tab, SVC_VECTORS *svc, short *px, short *py,
		short *Upsampling_tmp, short *xk16, short *xp16, short *yk16,
		short* yp16, short *aio_tiMv, short *aio_tiRef, uchar *aio_tucDpb_luma,
		uchar *aio_tucDpb_Cb, uchar *aio_tucDpb_Cr, short *Residu_Img,
		short *Residu_Cb, short *Residu_Cr);
void Decode_B_svc(const int size, const SPS *ai_pstSps, const PPS *ai_pstPps,
		const SLICE *ai_pstSlice, const NAL *nal, const uchar *ai_tiSlice_table,
		const DATA *ai_tstTab_Block, RESIDU *residu,
		MAIN_STRUCT_PF *baseline_vector, const LIST_MMO *ai_pstRefPicListL0,
		const LIST_MMO *ai_pstRefPicListL1, const LIST_MMO *ai_pstCurrent_pic,
		W_TABLES *quantif_tab, SVC_VECTORS *svc, short *px, short *py,
		short *Upsampling_tmp, short *k16, short *p16, short *yk16, short *yp16,
		short *aio_tiMv_l0, short *aio_tMv_l1, short *aio_tiRef_l0,
		short *aio_tiRef_l1, uchar *aio_tucDpb_luma, uchar *aio_tucDpb_Cb,
		uchar *aio_tucDpb_Cr, short *Residu_Img, short *Residu_Cb,
		short *Residu_Cr);
void Decode_I_svc(const int size, SPS *sps, PPS *pps, SLICE *slice, NAL *nal,
		uchar *slice_table, DATA *Block, RESIDU *residu, STRUCT_PF *vector,
		LIST_MMO *Current_pic, W_TABLES *quantif, unsigned char *aio_tucImage,
		unsigned char *aio_tucImage_Cb, unsigned char *aio_tucImage_Cr);
void extract_picture(int xsize, int ysize, int edge, int Crop,
		uchar *img_luma_in, uchar *img_Cb_in, uchar *img_Cr_in, int address_pic,
		uchar *img_luma_out, uchar *img_Cb_out, uchar *img_Cr_out);
int UserDefinedSearchFrameToDisplay(NAL *Nal, MMO *Mmo, int *x_size,
		int *y_size, int *Crop, int *ImgToDisplay, int *address_pic);

#endif /* H_PC_H */
