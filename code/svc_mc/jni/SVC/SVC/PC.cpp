#include <android/bitmap.h>
#include <cpu-features.h>
#include <errno.h>
#include <iostream>
#include <jni.h>
#include <prof.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <vector>

#include <android/log.h>
#define  LOG_TAG    "libsvc (PC.cpp)"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,   LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,   LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,    LOG_TAG, __VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,    LOG_TAG, __VA_ARGS__)

extern "C" {
  #include "GetHttpStream.h"
  #include "main_data.h"
  #include "Read_h264.h"
  #include "svc_type.h"
  #include "type.h"
}

// New Class header
#include "DASHHandler.h"
#include "FrameStore.h"
#include "Manager.h"
#include "MyMonitor.h"
#include "SDLEventHandler.h"

using namespace std;
using namespace dashhandler;
using namespace Bitstream;

/* Type Definded */
#define uchar         unsigned char
#define ushort        unsigned short
#define ulong         unsigned long
#define uint          unsigned int
#define prec_synchro  int
#define stream        unsigned char
#define image_type    unsigned char
#define dpb_type      unsigned char

typedef unsigned char       byte;   // Byte is a char
typedef unsigned short int  word16; // 16-bit word is a short int
typedef unsigned int        word32; // 32-bit word is an int

/* Ring Buffer size Defended */
#define maxThreshold    15728640      // Not used, we want to use this to lock request thread. Test Value: 12582912, 5242880, 15728640
#define minThreshold    1572864       // when available data size small than this value will be locked. Test Value: 2097152, 1572864
#define MaxFileSize     20*1024*1024  // the size of ringBuffer called StreamBuffer. Test Value: 20*1024*1024 (Default : 10185706)
#define RepeatSegments  20            // 70 is total segments of test file

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

//---- Multi-Threading ----//
// #define THREAD_NUM 3
// static pthread_t ThreadList[THREAD_NUM];
int THREAD_NUM = 1;
pthread_t *ThreadList = NULL;
pthread_t request_thread;

// CRITICAL_SECTION       FrameCounterMutex   = PTHREAD_MUTEX_INITIALIZER;  // protect race condition when set the FramecCounter

CRITICAL_SECTION    DownloadMutex     = PTHREAD_MUTEX_INITIALIZER; // lock until free size of StreamBuffer can put new data
CONDITION_VARIABLE  CanDownloadCond   = PTHREAD_COND_INITIALIZER;  // related condition

CRITICAL_SECTION    StreamBufferMutex = PTHREAD_MUTEX_INITIALIZER; // lock until data enough
CONDITION_VARIABLE  CanPlayCond       = PTHREAD_COND_INITIALIZER;  // related condition

//static CRITICAL_SECTION FirstDownloadMutex = PTHREAD_MUTEX_INITIALIZER;
//static CONDITION_VARIABLE FirstDownloadCond = PTHREAD_COND_INITIALIZER;

CRITICAL_SECTION    BufferReallocMutex  = PTHREAD_MUTEX_INITIALIZER;     // protect race condition when copy downloaded data to StreamBuffer
CRITICAL_SECTION    DataSizeMutex       = PTHREAD_MUTEX_INITIALIZER;     // protect race condition when set the available data size
CRITICAL_SECTION    EnhIDRMutex         = PTHREAD_MUTEX_INITIALIZER;     // protect when set enh_IDRindex

CRITICAL_SECTION    RetriveMutex        = PTHREAD_MUTEX_INITIALIZER;
CRITICAL_SECTION    DirtyMutex          = PTHREAD_MUTEX_INITIALIZER;
CRITICAL_SECTION    CurrentDisplayMutex = PTHREAD_MUTEX_INITIALIZER;
CRITICAL_SECTION    FrameBufferMutex    = PTHREAD_MUTEX_INITIALIZER;

CONDITION_VARIABLE  CurrentDisplayCond  = PTHREAD_COND_INITIALIZER;
CONDITION_VARIABLE  FillFrameCond       = PTHREAD_COND_INITIALIZER;
CONDITION_VARIABLE  FrameBufferCond     = PTHREAD_COND_INITIALIZER;

//static char* FILE_PATH; // Get file segment from HTTP Get request
//---- Used for requesting segment ----//
int   availableDataSize = 0;
int   displayTimes      = 0;
char  *seqBuf           = NULL;
char  *prefix           = NULL;
char  *base_url         = NULL;

extern char *StreamBuffer;
extern int  file_size;
extern int  StreamBufferPos;
extern int  currentBufferPos;

//---- For Switch Event ----//
static int    LAYER_ID;
static int    TEMPORAL_ID;
// static int    *Temporal_changed;    // if is True then change temporal ID
// static int    *Layer_changed;     // if it's True then change layer ID
static int    download_enhance;   // set number means to start downloading which enhancement layer.
static int    pre_download_enhance;   // set the number previous enhancement layer.
static int    switchFrameNumber;
int           CurrentDisplayGOP = 0;
int           FillFrameIndex    = 0;
/*
 * indicate when will switch to enhancement layer
 * if enh_IDRindex[0] is True, then can switch to enhancement layer one.
 * if enh_IDRindex[1] is True, then can switch to enhancement layer two.
 */
static int enh_IDRindex[2];

//---- For Measurement ----//
/* Decode */
// static long time_start;
// static timeval  *start_decode_time  = NULL;
// static timeval  *end_decode_time    = NULL;
// static int    *setStartDecodeTime = NULL;
// static int    *setDecodeTime      = NULL;
/* Network */
static timeval  time_start; // Used for getting network delay
// long   network_delay              = 0;
static float  totalDataSize       = 0;
static int    time_start_download = 0;

static int i;
static int init = 0;
static void* pixels[64];
static AndroidBitmapInfo info;
static int getMinBytes = 1622016; // default 1622016, each run will get this to do some operations.
static int framecounter;
static int bitmapFilledCounter, bitmapFilledMax;
static int flagError;

// static FrameStore FrameStoreList[THREAD_NUM];
static FrameStore *FrameStoreList = NULL;
// static vector<FrameStore> FrameStoreList;

// static SVCinfo SVCinfoList[THREAD_NUM];
static SVCinfo* SVCinfoList = NULL;
// static int ThreadFinishSignal;
// static sem_t ReturnSem;

// Bear for stack size
pthread_attr_t attr;
int firstDecodeThread = -1;

//---- For SDL2 Event Handler ----//
bool quit = false;
MyMonitor *monitor = NULL;

//---- For DASH Handler ----//
DASHHandler *handler = NULL;
Manager *extractor   = NULL;

int streamingMODE = -1;

extern "C" {
  /* SVC Decoder Functions */
  void init_svc_vectors(SVC_VECTORS *svc);
  void decode_init_vlc(VLC_TABLES *VLc);
  void vector_main_init(MAIN_STRUCT_PF *pf);
  //void readh264_init(int argc,char **argv);
  void get_layer(int argc, char **argv, ConfigSVC *NumLayer);
  void choose_layer(ConfigSVC *NumLayer);
  void ParseFirstAU(int StreamSize, uchar *Buffer, ConfigSVC *StreamType);
  int  GetNalBytesAuSVC(uchar *data, stream *rbsp, int *NalInRbsp, ConfigSVC *StreamType, NALSTREAMER *NalSize, SPS *Sps, PPS *Pps);
  void init_int(int *tab);
  void init_mmo(int num_of_layers, MMO *mmo_stru);
  void init_slice(SLICE *slice);
  void init_pps(PPS *sps);
  void init_sps(SPS *sps);
  void InitListMmo(LIST_MMO *RefPicListL0);
  void slice_header_svc(int NbLayers, const stream *data, SPS *sps_id, PPS *pps_id, int *entropy_coding_flag, W_TABLES *quantif, LIST_MMO *current_pic, SPS *sps, PPS *pps, int *position, SLICE *slice, MMO *mmo, LIST_MMO RefPicListL0[], LIST_MMO RefPicListL1[], NAL *nal, int *end_of_slice, int *ImgToDisplay, int *xsize, int *ysize, int *AddressPic, int *Crop);
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
  void seq_parameter_set(stream *data, SPS *sps);
  void FlushSVCFrame(SPS *sps, NAL *nal, MMO *mmo, int *address_pic, int *x_size, int *y_size, int *Crop, int *img_to_display);
  void PrefixNalUnit(stream *data, int *NalinRbsp, NAL *nal, MMO *mmo, SPS *sps, int *EndOfSlice);
  void subset_sps(stream *data, int * NalInRbsp, SPS *sps, NAL *nal);
  void NalUnitHeader(const stream *data, int *pos, NAL *nal, int *EndOfSlice);
  void slice_data_in_scalable_extension_cavlc(const int size_mb, const stream *ai_pcData, int * NalInRbsp, const int *ai_piPosition, const NAL *nal, const SPS *ai_pstSps, PPS *ai_pstPps, const VLC_TABLES *vlc, uchar *ai_slice_group_id, SLICE *aio_pstSlice, uchar *aio_tiMbToSliceGroupMap, uchar *aio_tiSlice_table, DATA *aio_tstTab_block, RESIDU *residu, int * aio_piEnd_of_slice);
  void SliceCabac(const int size_mb, uchar *data, int *position, int *NalBytesInNaluniinit_nal_structt, const NAL *Nal, SPS *sps, PPS *pps, uchar *ai_slice_group_id, short *mv_cabac_l0, short *mv_cabac_l1, short *ref_cabac_l0, short *ref_cabac_l1, SLICE *slice, uchar *MbToSliceGroupMap, uchar *slice_table, DATA *Tab_block, RESIDU *picture_residu, int *end_of_slice);
  void slice_base_layer_cavlc(const stream *ai_pcData, int * NalInRbsp, const int *ai_piPosition, const SPS *ai_pstSps, PPS *ai_pstPps, const VLC_TABLES *Vlc, uchar *ai_slice_group_id, LIST_MMO *Current_pic, LIST_MMO *RefListl1, NAL *Nal, SLICE *aio_pstSlice, uchar *aio_tiMbToSliceGroupMap, uchar *aio_tiSlice_table, DATA *aio_tstTab_block, RESIDU *picture_residu, int * aio_piEnd_of_slice, short *mv_io, short *mvl1_io, short *ref_io, short *refl1_io);
  void slice_base_layer_cabac(uchar *data, int *position, int *NalBytesInNalunit, SPS *sps, PPS *pps, uchar *ai_slice_group_id, LIST_MMO *Current_pic, LIST_MMO *RefListl1, NAL *Nal, short *mv_cabac_l0, short *mv_cabac_l1, short *ref_cabac_l0, short *ref_cabac_l1, SLICE *slice, uchar *MbToSliceGroupMap, uchar *slice_table, DATA *Tab_block, RESIDU *picture_residu, int *end_of_slice, short *mvl0_io, short *mb_l1_io, short *refl0_io, short *refl1_io);
  void Decode_P_avc(const SPS *ai_pstSps, const PPS *ai_pstPps, const SLICE *ai_pstSlice, const uchar *ai_tiSlice_table, const RESIDU *picture_residu, const STRUCT_PF *pf, const LIST_MMO *ai_pstRefPicListL0, const LIST_MMO *ai_pstCurrent_pic, W_TABLES *quantif_tab, NAL *Nal, short *aio_tiMv, short *aio_tiRef, uchar *aio_tucDpb_luma, uchar *aio_tucDpb_Cb, uchar *aio_tucDpb_Cr, short *Residu_Img, short *Residu_Cb, short *Residu_Cr);
  void Decode_B_avc(SPS *ai_stSps, PPS *ai_stPps, SLICE *ai_stSlice, uchar *ai_tSlice_table, RESIDU *picture_residu, MAIN_STRUCT_PF *main_vector, LIST_MMO *ai_pRefPicListL0, LIST_MMO *ai_pRefPicListL1, LIST_MMO *ai_pCurrent_pic, W_TABLES *quantif, NAL *Nal, short *aio_tMv_l0, short *aio_tMv_l1, short *aio_tref_l0, short *aio_tref_l1, uchar *aio_tDpb_luma, uchar *aio_tDpb_Cb, uchar *aio_tDpb_Cr, short *Residu_img, short *Residu_Cb, short *Residu_Cr);
  void Decode_I_avc(SPS *sps, PPS *pps, SLICE *slice, uchar *slice_table, RESIDU *picture_residu, STRUCT_PF *pf, W_TABLES *quantif_tab, NAL *Nal, uchar *image, uchar *image_Cb, uchar *image_Cr);
  void FinishFrameSVC(const int NbMb, NAL *Nal, SPS *Sps, PPS *Pps, LIST_MMO *Current_pic, SLICE *Slice, int EndOfSlice, uchar *SliceTab, DATA *TabBlbock, RESIDU *Residu, short *MvL0, short *MvL1, short *RefL0, short *RefL1, int *Crop, int *ImgToDisplay, int *AdressPic, MMO *Mmo, unsigned char *RefY, unsigned char *RefU, unsigned char *RefV, int *xsize, int *ysize);
  void Decode_P_svc(const int size, const SPS *ai_pstSps, const PPS *ai_pstPps, const SLICE *ai_pstSlice, const NAL *nal, const uchar *ai_tiSlice_table, const DATA *ai_tstTab_Block, RESIDU *residu, STRUCT_PF *baseline_vector, const LIST_MMO *ai_pstRefPicListL0, const LIST_MMO *ai_pstCurrent_pic, W_TABLES *quantif_tab, SVC_VECTORS *svc, short *px, short *py, short *Upsampling_tmp, short *xk16, short *xp16, short *yk16, short* yp16, short *aio_tiMv, short *aio_tiRef, uchar *aio_tucDpb_luma, uchar *aio_tucDpb_Cb, uchar *aio_tucDpb_Cr, short *Residu_Img, short *Residu_Cb, short *Residu_Cr);
  void Decode_B_svc(const int size, const SPS *ai_pstSps, const PPS *ai_pstPps, const SLICE *ai_pstSlice, const NAL *nal, const uchar *ai_tiSlice_table, const DATA *ai_tstTab_Block, RESIDU *residu, MAIN_STRUCT_PF *baseline_vector, const LIST_MMO *ai_pstRefPicListL0, const LIST_MMO *ai_pstRefPicListL1, const LIST_MMO *ai_pstCurrent_pic, W_TABLES *quantif_tab, SVC_VECTORS *svc, short *px, short *py, short *Upsampling_tmp, short *k16, short *p16, short *yk16, short *yp16, short *aio_tiMv_l0, short *aio_tMv_l1, short *aio_tiRef_l0, short *aio_tiRef_l1, uchar *aio_tucDpb_luma, uchar *aio_tucDpb_Cb, uchar *aio_tucDpb_Cr, short *Residu_Img, short *Residu_Cb, short *Residu_Cr);
  void Decode_I_svc(const int size, SPS *sps, PPS *pps, SLICE *slice, NAL *nal, uchar *slice_table, DATA *Block, RESIDU *residu, STRUCT_PF *vector, LIST_MMO *Current_pic, W_TABLES *quantif, unsigned char *aio_tucImage, unsigned char *aio_tucImage_Cb, unsigned char *aio_tucImage_Cr);
  void extract_picture(int xsize, int ysize, int edge, int Crop, uchar *img_luma_in, uchar *img_Cb_in, uchar *img_Cr_in, int address_pic, uchar *img_luma_out, uchar *img_Cb_out, uchar *img_Cr_out);
  int UserDefinedSearchFrameToDisplay(NAL *Nal, MMO *Mmo, int *x_size, int *y_size, int *Crop, int *ImgToDisplay, int *address_pic);
}


// void retrieveFrame(int threadID) {
int retrieveFrame(int threadID) {
  int r = 0;
  if (SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0] == 1 || SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0] == 2) {
    // EnterCriticalSection(&DirtyMutex);
    int XDIM
      = ((int *) SVCinfoList[threadID].Display_1_Extract_Image_Y_o)[0]
      = *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o;

    int YDIM
      = ((int *) SVCinfoList[threadID].Display_1_Extract_Image_Y_o)[1]
      = *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o;

    uchar *Y = SVCinfoList[threadID].Display_1_Extract_Image_Y_o + 8;
    uchar *U = Y + (XDIM + 32) * YDIM;
    uchar *V = U + (XDIM + 32) * YDIM / 4;

    extract_picture(XDIM, YDIM, 16,
        SVCinfoList[threadID].decoder_svc_VideoParameter_Crop[0],
        SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
        SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
        SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
        SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o[0],
        Y, U, V);

    // fill data to frame store
    // LOGI("FC : Thread %d: w=%d * nw=%d\n", threadID, FrameStoreList[threadID].frameWidth, *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o);
    // LOGI("FC : Thread %d: h=%d * nh=%d\n", threadID, FrameStoreList[threadID].frameHeight, *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o);
    FrameStoreList[threadID].frameWidth  = *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o;
    FrameStoreList[threadID].frameHeight = *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o;

    // LOGI("[retrieveFrame] - DIM[X, Y] = [%d, %d], frame[Width, Height] = [%d, %d]", XDIM, YDIM, FrameStoreList[threadID].frameWidth, FrameStoreList[threadID].frameHeight);

    // LOGE("[retrieveFrame] - FillFrame in retrieveFrame by Thread_%d", threadID);
    FrameStoreList[threadID].FillFrame(&FrameStoreList[threadID], Y, U, V);
    // LeaveCriticalSection(&DirtyMutex);
  }
  return r;
}

void copyNewDataIntoStreamBuffer (char *newData, int dataSize) {
  LOGI("[copyNewDataIntoStreamBuffer] dataSize = %d", dataSize);

  //---- If free space not enough for storing new segment ----//
  
  if (file_size - availableDataSize < dataSize) {
    EnterCriticalSection(&StreamBufferMutex);
    WakeAllConditionVariable(&CanPlayCond); // wake up all decoding threads
    LOGV("[copyNewDataIntoStreamBuffer] - Wake up DecoderThreads locked by \"CanPlayCond\" => free space is not enough");
    LOGV("[copyNewDataIntoStreamBuffer] - Lock RequestThread by \"CanDownloadCond\" => free space is not enough");
    WaitConditionVariableCS(&CanDownloadCond, &StreamBufferMutex); // wait for waking up
    LeaveCriticalSection(&StreamBufferMutex);
  }
  

  //---- rewrite to begin ----//
  if (StreamBufferPos + dataSize >= file_size) {
    int first_part     = file_size - StreamBufferPos;
    int remainder_part = dataSize - first_part;

    EnterCriticalSection(&StreamBufferMutex);
    LOGV("[copyNewDataIntoStreamBuffer] memcpy1 to StreamBuffer[%d, %d]", StreamBufferPos, StreamBufferPos+first_part-1);
    memcpy(&StreamBuffer[StreamBufferPos], newData, first_part * sizeof(char));

    LOGV("[copyNewDataIntoStreamBuffer] memcpy2 to StreamBuffer[%d, %d]", 0, remainder_part-1);
    memcpy(StreamBuffer, &newData[first_part], remainder_part * sizeof(char));

    availableDataSize += dataSize;
    LOGV("[copyNewDataIntoStreamBuffer] - availableDataSize => from %d to %d [%.2f] (+%d)", availableDataSize-dataSize, availableDataSize, (float)availableDataSize/file_size, dataSize);
    LeaveCriticalSection(&StreamBufferMutex);
  }
  //---- write into buffer with normal case ----//
  else {
    EnterCriticalSection(&StreamBufferMutex);
    LOGV("[copyNewDataIntoStreamBuffer] memcpy to StreamBuffer[%d, %d]", StreamBufferPos, StreamBufferPos+dataSize-1);
    memcpy(&StreamBuffer[StreamBufferPos], newData, dataSize * sizeof(char));

    availableDataSize += dataSize;
    LOGV("[copyNewDataIntoStreamBuffer] - availableDataSize => from %d to %d [%.2f] (+%d)", availableDataSize-dataSize, availableDataSize, (float)availableDataSize/file_size, dataSize);
    LeaveCriticalSection(&StreamBufferMutex);
  }

  StreamBufferPos = (StreamBufferPos + dataSize) % file_size;
  // LeaveCriticalSection(&StreamBufferMutex);

  // totalDataSize += dataSize;
  // LOGD("[CopyData] - StreamBufferPos=%d, currentBufferPos=%d", StreamBufferPos, currentBufferPos);

  //---- set available data dataSize ----//
  // EnterCriticalSection(&StreamBufferMutex);
  // EnterCriticalSection(&DataSizeMutex);
  // if (StreamBufferPos >= currentBufferPos) {
  //  availableDataSize = StreamBufferPos - currentBufferPos;
  // }
  // else {
  //  availableDataSize = (file_size) - (currentBufferPos - StreamBufferPos);
  // }
  // LeaveCriticalSection(&DataSizeMutex);


  // If available data dataSize is enough (larger than minThreshold)  
  if (availableDataSize > minThreshold) {
    EnterCriticalSection(&StreamBufferMutex);
    WakeAllConditionVariable(&CanPlayCond);
    LeaveCriticalSection(&StreamBufferMutex);
    LOGV("[copyNewDataIntoStreamBuffer] - Wake up DecoderThreads locked by \"CanPlayCond\" => availble data is enough");
  }

  // if (!network_delay) {
  //   timeval tp;
  //   gettimeofday(&tp, 0);
  //   network_delay = ((tp.tv_sec - time_start.tv_sec)*1000000) + ((tp.tv_usec - time_start.tv_usec));
  // }
}


/*
 * Get segments from HTTP server
 * each segment has eight GOPs
 */
// void streamFromHttp (int displayTimes) {
//   char url[100];
//   char file_name[30]; //---- The file_name format is : videoName_segmentNumber_layerNumber ----//
//   int seqSize = 0;

//   if (seqBuf == NULL) {
//     seqBuf = (char*) calloc(30000000, sizeof(char)); // 30 MB
//     if (seqBuf == NULL) LOGE("[Error] - seqBuf calloc Error");
//   }

//   /* hard code temporary, dump all 264 streams */

//   //---- This is first time to download segment ----//
//   if (displayTimes == 0) {
//     //---- initial segment, only can request once ----//
//     sprintf(file_name, "%s_h_%d.264", prefix, download_enhance);
//   }
//   else {
//     //---- The 6th segment of talking_head video has problem, repeat 5th segment ----//
//     // if ((displayTimes % RepeatSegments) == 6 && !strcmp(prefix, "talking_head")) {
//     //  sprintf(file_name, "%s_5_%d.264", prefix, download_enhance);
//     // }
//     // else {
//     //  sprintf(file_name, "%s_%d_%d.264", prefix, (displayTimes % RepeatSegments), download_enhance);
//     // }

//     sprintf(file_name, "%s_%d_%d.264", prefix, (displayTimes % RepeatSegments), download_enhance);

//     if(download_enhance && !enh_IDRindex[download_enhance - 1]) {
//       enh_IDRindex[download_enhance - 1] = displayTimes * 8;
//     }
//   }

//   sprintf(url, "%s/%s/%s", base_url, prefix, file_name); // Normal Case
//   // sprintf(url, "%s/%s/dash/%s", base_url, prefix, file_name); // DASH Testing

//   // LOGE("[streamFromHttp] - Start to Request %s", file_name);
//   GetHttpStream(url, seqBuf, &seqSize);

//   if (seqSize < 1000) {
//     LOGE("[streamFromHttp] - Request %s Failed!", url);
//     return;
//   }
//   monitor->AddDataBytes(seqSize);

//   LOGE("[streamFromHttp] - Request %s %dth segment, size is %d", file_name, displayTimes, seqSize);

//   copyNewDataIntoStreamBuffer(seqBuf, seqSize);

//   // //---- If free space not enough to store downloaded segment ----//
//   // if (file_size - availableDataSize < seqSize) {
//   //  EnterCriticalSection(&StreamBufferMutex);
//   //    WakeAllConditionVariable(&CanPlayCond); // wake up all decoding thread
//   //    LOGE("[streamFromHttp] - Wake up DecoderThreads due to free space is not enough");
//   //  LeaveCriticalSection(&StreamBufferMutex);

//   //  EnterCriticalSection(&DownloadMutex);
//   //    LOGE("[streamFromHttp] - RequestThread is Locked");
//   //    WaitConditionVariableCS(&CanDownloadCond, &DownloadMutex); // wait for waking up
//   //    LOGE("[streamFromHttp] - RequestThread keeps going");
//   //  LeaveCriticalSection(&DownloadMutex);
//   // }

//   // //---- rewrite to begin ----//
//   // if (StreamBufferPos + seqSize > file_size) {
//   //  int first_part    = file_size - StreamBufferPos;
//   //  int remainder_part  = seqSize - first_part;

//   //  // EnterCriticalSection(&BufferReallocMutex);
//   //    // LOGE("test_buffer : memcpy1(StreamBuffer[%d], seqBuf[0], %d) in streamFromHttp", StreamBufferPos, first_part);
//   //    memcpy(&StreamBuffer[StreamBufferPos], seqBuf, first_part * sizeof(char));

//   //    // LOGE("test_buffer : memcpy2(StreamBuffer[0], seqBuf[%d], %d) in streamFromHttp", first_part, remainder_part);
//   //    memcpy(StreamBuffer, &seqBuf[first_part], remainder_part * sizeof(char));

//   //    StreamBufferPos = (StreamBufferPos + seqSize) % file_size;
//   //  // LeaveCriticalSection(&BufferReallocMutex);
//   // }
//   // //---- write into buffer with normal case ----//
//   // else {
//   //  // EnterCriticalSection(&BufferReallocMutex);
//   //    // LOGE("test_buffer : memcpy(StreamBuffer[%d], seqBuf[0], %d) in streamFromHttp", StreamBufferPos, seqSize);
//   //    memcpy(&StreamBuffer[StreamBufferPos], seqBuf, seqSize * sizeof(char));

//   //    StreamBufferPos = (StreamBufferPos + seqSize) % file_size;
//   //  // LeaveCriticalSection(&BufferReallocMutex);
//   // }
//   // totalDataSize += seqSize;
//   // // LOGE("[streamFromHttp] - Copy segment data into StreamBuffer");

//   // //---- set available data size ----//
//   // if (StreamBufferPos >= currentBufferPos) {
//   //  EnterCriticalSection(&DataSizeMutex);
//   //    availableDataSize = StreamBufferPos - currentBufferPos;
//   //  LeaveCriticalSection(&DataSizeMutex);
//   // }
//   // else {
//   //  EnterCriticalSection(&DataSizeMutex);
//   //    availableDataSize = (file_size - 1) - (currentBufferPos - StreamBufferPos);
//   //  LeaveCriticalSection(&DataSizeMutex);
//   // }
//   // // LOGE("[streamFromHttp] - Current available data size is %d", availableDataSize);

//   // /* If available data size is enough (larger than minThreshold) */
//   // if (availableDataSize >= minThreshold) {
//   //  EnterCriticalSection(&StreamBufferMutex);
//   //    WakeAllConditionVariable(&CanPlayCond);
//   //    LOGE("[streamFromHttp] - Wake up DecoderThreads due to available data is enough");
//   //  LeaveCriticalSection(&StreamBufferMutex);
//   // }

//   // if (!network_delay) {
//   //  timeval tp;
//   //  gettimeofday(&tp, 0);
//   //  network_delay = ((tp.tv_sec - time_start.tv_sec)*1000000) + ((tp.tv_usec - time_start.tv_usec));
//   //  // LOGE("TIME : network delay: %ld", network_delay);
//   // }

//   // // LOGE("test_buffer : StreamBufferPos = %d, currentBufferPos = %d\n", StreamBufferPos, currentBufferPos);
// }

void DashStreaming () {
  BitStream *data = NULL;
  uint32_t layerid = 0;
  uint32_t sampleCount = 0;
  handler->onStartButtonPressed();
  extractor = handler->GetExtractor();

  while (!quit) {
    handler->onSettingsChanged(0, 0, monitor->GetSIDIndex());
    // handler->onSettingsChanged(0, 0, qualityLevel);
    data = extractor->GetDecodableSegment(sampleCount, layerid);
    if (data == NULL) LOGE("[DashStreaming]:%d - data is null", __LINE__);
    // LOGI("[DashStreaming] - sampleCount = %d, layerID = %d", sampleCount, layerid);
    // LOGI("[DashStreaming] - Get Decodable Segment size %d", data->GetSize());
    monitor->AddDownloadData(sampleCount, layerid, data->GetSize());
    monitor->SetTransferEndTime();

    copyNewDataIntoStreamBuffer ((char*) data->GetData(0), data->GetSize());
    delete data;
    data = NULL;
  }
}

void* getStreams(void *arg) {
  // time_start_download = time(NULL);
  if (monitor) monitor->SetNetworkStartTime();
  DashStreaming();

  // if (streamingMODE == 0) {
  //   for (displayTimes = 0; !quit; displayTimes++) {
  //     streamFromHttp(displayTimes);
  //   }
  // }
  // else if (streamingMODE == 1) {
  //   DashStreaming();
  // }
}

void fileStreamInit() {
  file_size       = MaxFileSize;
  StreamBufferPos = 0;

  StreamBuffer = (char*) calloc(file_size, sizeof(char));
  if (StreamBuffer == NULL) LOGE("[Error] - StreamBuffer calloc Error");

  // extern char* StreamBuffer;
  // extern int file_size;
  // extern int StreamBufferPos;

  // int seqSize;
  // char *seqBuf = (char*) malloc(sizeof(char) * 10000000);

  //---- hard code temporarybufferCrash ----//
  //---- it should read from manifest file ----//

  // StreamBufferPtr = getStreamBufferPtr();
  // FileSizePtr = getFileSizePtr();
  // StreamBufferPosPtr = getStreamBufferPosPtr();

  // file_size = 20971520;  // 20971520 = 20MB , 1048576 = 1MB, default = 10185706
  // StreamBufferPos = 0;
  // StreamBuffer = (char*) malloc(file_size * sizeof(char));

  /** segment 0, only can get once **/
  // GetHttpStream("http://140.114.79.80/test_0.264", seqBuf, &seqSize);
  // memcpy(&StreamBuffer[StreamBufferPos], seqBuf, seqSize * sizeof(char));
  // StreamBufferPos += seqSize;
  // LOGE("TESTSEG : got test_0.264 with 0 segment");

  pthread_create(&request_thread, NULL, getStreams, NULL);

  // streamFromHttp();
}

void initialize(int threadID) {
  LOGI("[initialize] - Init time = %lld", (long long) time(NULL));

  // FILE* yfile = fopen("/sdcard/f1.yuv","w+");

  int argc = 7;
  char **argv = (char **) malloc(sizeof(char *) * 7);
  argv[1] = (char *) "-h264";
  // argv[2] = FILE_PATH;
  argv[2] = (char *) "";
  // argv[2] = "/sdcard/doc-reality.264";
  // argv[2] = "/sdcard/video_3.264";
  // argv[2] = "/sdcard/doc-reality-short.264";
  argv[3] = (char *) "-layer";
  argv[4] = (char *) malloc(sizeof(char) * 5);
  // sprintf(argv[4], "%d", LAYER_ID);
  sprintf(argv[4], "%d", monitor->GetSID());
  argv[5] = (char *) "-tempId";
  argv[6] = (char *) malloc(sizeof(char) * 5);
  // sprintf(argv[6], "%d", TEMPORAL_ID);
  sprintf(argv[6], "%d", monitor->GetTID());

  init_svc_vectors(SVCinfoList[threadID].decoder_svc_Svc_Vectors_PC_Svc_Vectors);
  decode_init_vlc(SVCinfoList[threadID].decoder_svc_VlcTab_PC_o);
  vector_main_init(SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o);
  *SVCinfoList[threadID].ReadAU_ReadBytes = getMinBytes;
  readh264_init(argc, argv);    //comment out with stream buffer
  get_layer(argc, argv, SVCinfoList[threadID].GetNalBytesAu_StreamType);

  // EnterCriticalSection(&StreamBufferMutex);
    if ((threadID == 0) && (availableDataSize < getMinBytes)) {
      EnterCriticalSection(&StreamBufferMutex);
      LOGV("[initialize]:%d - DecoderThreads is Locked by \"CanPlayCond\"", __LINE__);
      WaitConditionVariableCS(&CanPlayCond, &StreamBufferMutex);
      LeaveCriticalSection(&StreamBufferMutex);
    }
  // LeaveCriticalSection(&StreamBufferMutex);

  ParseFirstAU(getMinBytes, SVCinfoList[threadID].ReadAU_DataFile_o, SVCinfoList[threadID].GetNalBytesAu_StreamType);

  init_nal_struct(SVCinfoList[threadID].DqIdNextNal_Nal_o, 4);
  init_int(SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice);
  init_int(SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay);
  init_int(SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o);
  init_int(SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o);
  init_int(SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o);
  init_int(SVCinfoList[threadID].decoder_svc_VideoParameter_Crop);
  init_slice(SVCinfoList[threadID].decoder_svc_Residu_Slice);
  init_sps(SVCinfoList[threadID].decoder_svc_Residu_SPS);
  init_pps(SVCinfoList[threadID].decoder_svc_Residu_PPS);
  InitListMmo(SVCinfoList[threadID].decoder_svc_Residu_RefL0);
  InitListMmo(SVCinfoList[threadID].decoder_svc_Residu_RefL1);
  InitListMmo(SVCinfoList[threadID].decoder_svc_Residu_Current_pic);
  init_mmo(4, SVCinfoList[threadID].decoder_svc_Residu_Mmo);
  SVCinfoList[threadID].decoder_svc_Residu_Current_pic->frame_num += threadID;
  SVCinfoList[threadID].iFrame = -1;
  SVCinfoList[threadID].EndOfStream = 0;
  decoderh264_init(1920, 1088);
}

void* Thread_Decoding(void *argu) {
  //---- Get my thread ID ----//
  int threadID = (int) argu;

  //---- Bear, play with stack size ----//
  // size_t mystacksize;
  // pthread_attr_getstacksize(&attr, &mystacksize);
  // LOGE("Thread %d: stack size = %li bytes\n", threadID, mystacksize);

  int isFinished = 0;

  //---- exit when already reach EOF and played ----//
  // if(SVCinfoList[threadID].EndOfStream && FrameStoreList[threadID].isEmpty(&FrameStoreList[threadID])==0)
  //  return NULL;

  for (; !isFinished; ) {
    // LOGI("[Thread_Decoding] - DecoderThread[%d] start decoding", threadID);

    if (SVCinfoList[threadID].EndOfStream) return NULL;

    //---- data size is not enough for decoding ----//
    // EnterCriticalSection(&StreamBufferMutex);
    if (availableDataSize < minThreshold || availableDataSize < file_size/2) {

      // LOGI("[Thread_Decoding] - RequestThread waked up by DecoderThread_%d Locked by \"CanDownloadCond\"", threadID);
      // WakeConditionVariable(&CanDownloadCond); // wake up donwload thread

      EnterCriticalSection(&StreamBufferMutex);
      WakeConditionVariable(&CanDownloadCond); // wake up donwloading thread
      WaitConditionVariableCS(&CanPlayCond, &StreamBufferMutex); // waiting for data is larger enough
      LeaveCriticalSection(&StreamBufferMutex);

      // choose_layer(SVCinfoList[threadID].GetNalBytesAu_StreamType);
      // SVCinfoList[threadID].GetNalBytesAu_StreamType -> SetLayer = 2;
      // SVCinfoList[threadID].GetNalBytesAu_StreamType -> TemporalCom = 0;
    }
    // LeaveCriticalSection(&StreamBufferMutex);

    if (quit) return NULL;

    // if (firstDecodeThread < 0) firstDecodeThread = threadID;

    // if (!setStartDecodeTime[threadID]) {
    //   gettimeofday(&start_decode_time[threadID], 0);
    //   setStartDecodeTime[threadID] = 1;
    // }

    EnterCriticalSection(&StreamBufferMutex);
      // LOGI("[Thread_Decoding] - DecoderThread[%d] Enter StreamBufferMutex-1 CS", threadID);
      ReadAuH264(getMinBytes, SVCinfoList[threadID].ReadAU_DataFile_o,
          SVCinfoList[threadID].ReadAU_ReadBytes[0],
          SVCinfoList[threadID].ReadAU_pos_o);
    // EnterCriticalSection(&StreamBufferMutex);
      SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size[0] = GetNalBytesAuSVC(
          SVCinfoList[threadID].ReadAU_DataFile_o,
          SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
          SVCinfoList[threadID].ReadAU_ReadBytes,
          SVCinfoList[threadID].GetNalBytesAu_StreamType,
          SVCinfoList[threadID].GetNalBytesAu_NalStreamer,
          SVCinfoList[threadID].GetNalBytesAu_SPS,
          SVCinfoList[threadID].GetNalBytesAu_PPS);
    LeaveCriticalSection(&StreamBufferMutex);
    // LOGI("[Thread_Decoding] - DecoderThread[%d] Leave StreamBufferMutex-1 CS", threadID);

    EnterCriticalSection(&DirtyMutex); // Bear
      NextNalDqId(SVCinfoList[threadID].GetNalBytesAu_StreamType, SVCinfoList[threadID].DqIdNextNal_Nal_o);

      NalUnitSVC(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
          SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io,
          SVCinfoList[threadID].decoder_svc_NalUnit_NalRefIdc_io,
          SVCinfoList[threadID].DqIdNextNal_Nal_o);

      init_int(SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay);
    LeaveCriticalSection(&DirtyMutex); // Bear

    if (SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0] == 11) {
      SVCinfoList[threadID].EndOfStream = 1;
      // sem_post(&ReturnSem);
      return NULL;
    }

    //---- Type IDR-frame or End of stream ----//
    if (SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0] == 5 ||
      SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0] == 11 ||
      SVCinfoList[threadID].EndOfStream) {

      //---- increase IDR-Num ----//
      SVCinfoList[threadID].iFrame++;
      // LOGI("[Thread_Decoding] - Set thread_%d IFrame=%d", threadID, SVCinfoList[threadID].iFrame);

      //---- I have previous frames want to Display ----//
      if (!FrameStoreList[threadID].IsEmpty(&FrameStoreList[threadID])) {
        // LOGV("[Thread_Decoding]:%d - FrameSotreList[%d] !IsEmpty()", __LINE__, threadID);

        //---- retrieve all treasure from buffer ----//
        if (!FrameStoreList[threadID].waitForDisplay) {
          // LOGI("FC : thread %d, nw=%d nh=%d", threadID, *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o, *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o);
          EnterCriticalSection(&DirtyMutex);
          while (UserDefinedSearchFrameToDisplay(
              SVCinfoList[threadID].DqIdNextNal_Nal_o,
              SVCinfoList[threadID].decoder_svc_Residu_Mmo,
              SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o,
              SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o,
              SVCinfoList[threadID].decoder_svc_VideoParameter_Crop,
              SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay,  // return 1
              SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o)
              == 0) {
            // if (retrieveFrame(threadID) == 16) {
            //   LOGW("[Thread_Decoding] - retrieveFrame full. Break while.");
            //   // FrameStoreList[threadID].waitForDisplay = 1;
            //   break;
            // }
            retrieveFrame(threadID);
          }
          LeaveCriticalSection(&DirtyMutex);
          // LOGE("ITD : %d, after retrieveFrame", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0]);
          // LOGI("[Thread_Decoding] - DecoderThread[%d] After retrieveFrame", threadID);
        }

        //---- FC = frameCounter, CD = CurrentDisplayGOP, IDR = IDRindex, L = LayerID, T = TemporalID ----//
        // LOGI("[Thread_Decoding] - DecoderThread[%d] Ready To Display, FrameCounter=%d, CurrentDisplayGOP=%d, IDR=%d, ENIDR[%d]=%d, L=%d and T=%d",
        //     threadID,
        //     FrameStoreList[threadID].frameCounter,
        //     CurrentDisplayGOP,
        //     FrameStoreList[threadID].IDRindex,
        //     download_enhance - 1,
        //     download_enhance ? enh_IDRindex[download_enhance - 1] : 0,
        //     SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer,
        //     SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalId);

        //---- Critical region to access CurrentDisplayGOP ----//
        // LOGD("[Thread_Decoding:DeadLock] : Thread_%d, Before CS", threadID);
        EnterCriticalSection(&CurrentDisplayMutex);
        {
          // LOGD("[Thread_Decoding:DeadLock] : Thread_%d, Inside CS", threadID);
          LOGI("[Thread_Decoding] - DecoderThread_%d Ready To Display, FrameCounter=%d, CurrentDisplayGOP=%d, IDR=%d, SwitchAt=%d, L=%d and T=%d",
            threadID,
            FrameStoreList[threadID].frameCounter, 
            CurrentDisplayGOP, FrameStoreList[threadID].IDRindex,
            monitor->GetSwitchableGOPNo(), 
            SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer, 
            SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalId);

          // LOGV("[Thread_Decoding] - DecoderThread[%d] Enter CurrentDisplayMutex-1 CS", threadID);
          // LOGE("deadlock : Thread[%d], in CR CurrentDisplayMutex", threadID);

          int isFinishDisplay = 0;

          while(!isFinishDisplay) {

            //---- if this IDRindex is my job and display speed is too slow, so skip this IDR. ----//
            if( (CurrentDisplayGOP < FrameStoreList[threadID].IDRindex) && (CurrentDisplayGOP % THREAD_NUM == threadID) ) {
              LOGI("[Thread_Decoding] - DecoderThread_%d skip. {CurrentDisplayGOP=%d, IDR=%d}", threadID, CurrentDisplayGOP, FrameStoreList[threadID].IDRindex);

              // CurrentDisplayGOP++;
              // FrameStoreList[threadID].IDRindex++;
              LOGI("[Thread_Decoding] - Increasing GOP to %d and thread_%d IDR to %d", CurrentDisplayGOP, threadID, FrameStoreList[threadID].IDRindex);
              
              // // LOGE("deadlock : Thread %d, skip IDR %d", threadID, CurrentDisplayGOP);
              // // LOGE("deadlock : Thread[%d], before CR FrameBufferMutex to skip", threadID);
              // EnterCriticalSection(&FrameBufferMutex);
              //  // LOGE("deadlock : Thread[%d], in CR FrameBufferMutex to skip", threadID);
              //  // :D :D :D :D :D :D :-(
              //  // LOGE("deadlock : Thread[%d], Wake up every FrameBufferCond",threadID);
              //  WakeAllConditionVariable(&FrameBufferCond);
              //  LOGI("[Thread_Decoding] - DecoderThread[%d] wake up JavaThread at skip case", threadID);
              // LeaveCriticalSection(&FrameBufferMutex);
              // // LOGE("deadlock : Thread[%d], EXIT CR FrameBufferMutex to skip", threadID);
              // // sem_post(&ReturnSem);

              // SDL_Event event;
              // SDL_zero(event);
              // event.type = SKIP_FRAME_EVENT;
              // SDL_PushEvent(&event);
              // LOGD("Push SKIP Event");
            }
            //---- normal situation ----//
            else if(CurrentDisplayGOP == FrameStoreList[threadID].IDRindex) {
              //---- check resource ----//
              FrameStoreList[threadID].CheckResolution(&FrameStoreList[threadID]);

              // LOGE("deadlock : Thread[%d], before CR FrameBufferMutex to normal", threadID);
              // EnterCriticalSection(&FrameBufferMutex);
                // LOGE("[Thread_Decoding] - DecoderThread[%d] Enter CR with FrameBufferMutex in normal case", threadID);

                // if(!flagError){
                  //---- display ----//
                  LOGI("[Thread_Decoding] - DecoderThread_%d Display. {CurrentDisplayGOP=%d, IDR=%d}", threadID, CurrentDisplayGOP, FrameStoreList[threadID].IDRindex);
                  FrameStoreList[threadID].Display(&FrameStoreList[threadID], CurrentDisplayGOP);
                  // monitor->FinishedDecoding(threadID);
                  monitor->SetDecodingDelay(FrameStoreList[threadID].GetDecodingDelay(&FrameStoreList[threadID]), threadID);
                  // if (monitor->GetDecodingDelay() == 0.0) {
                  //   if (firstDecodeThread == threadID) {
                  //     monitor->SetDecodingDelay(FrameStoreList[threadID].GetDecodingDelay(&FrameStoreList[threadID]), threadID);
                  //   }    
                  // }
                  

                  // if (!setDecodeTime[threadID]) {
                  //   gettimeofday(&end_decode_time[threadID], 0);
                  //   setDecodeTime[threadID] = ((end_decode_time[threadID].tv_sec - start_decode_time[threadID].tv_sec)*1000000) + ((end_decode_time[threadID].tv_usec - start_decode_time[threadID].tv_usec));
                  //   // LOGE("TIME : decode first frame with thread[%d]: %d", threadID, setDecodeTime[threadID]);
                  //   // LOGI("[Thread_Decoding] - DecoderThread[%d] Set Decoding Time Done", threadID);
                  // }
                  // LOGE("Thread %d, display finished, displayCounter %d", threadID, FrameStoreList[threadID].displayCounter);

                  
                  FrameStoreList[threadID].IDRindex += THREAD_NUM;
                  LOGI("[Thread_Decoding] - DecoderThread_%d reset IDRindex to %d", threadID, FrameStoreList[threadID].IDRindex);

                  isFinishDisplay = 1;
                  // CurrentDisplayGOP++;
                  WakeAllConditionVariable(&CurrentDisplayCond);
                  
                // }else {
                  // LOGE("TESTDISPLAY : ERROR~~%d", threadID);
                // }

                // LOGE("deadlock : Thread[%d], Wake up every FrameBufferCond",threadID);

                /* If frame Copy into Buffer, wake up JavaThread*/
                // WakeAllConditionVariable(&FrameBufferCond);
                // LOGI("[Thread_Decoding] - DecoderThread[%d] wake up JavaThread at normal case", threadID);
              // LeaveCriticalSection(&FrameBufferMutex);
              // LOGE("deadlock : Thread[%d], EXIT CR FrameBufferMutex to normal", threadID);

              // LOGE("Thread %d, post", threadID);
              //sem_post(&ReturnSem);
            }
            // else if (CurrentDisplayGOP > FrameStoreList[threadID].IDRindex) {
            //  SDL_Event event;
            //  SDL_zero(event);
            //  event.type = RESET_GOP_NUMBER;
            //  SDL_PushEvent(&event);
            //  LOGD("Push RESET Event");
            // }

            //---- block until next signal while I'm fast!! ----//
            if(!isFinishDisplay) {
              LOGI("[Thread_Decoding:DeadLock] - Thread_%d is locked due to Decoder too fast. {CurrentDisplayGOP=%d, IDR=%d}", threadID, CurrentDisplayGOP, FrameStoreList[threadID].IDRindex);
              WaitConditionVariableCS(&CurrentDisplayCond, &CurrentDisplayMutex);
              // LOGD("[Thread_Decoding:DeadLock] : Thread_%d, Wakeup in CS", threadID);
              // timeval now;
              // gettimeofday(&now, NULL);
              // timespec to;
              // to.tv_sec = now.tv_sec;
              // to.tv_nsec = (now.tv_usec + 100*1000) * 1000; // 100 ms
              // TimedWaitConditionVariableCS(&CurrentDisplayCond, &CurrentDisplayMutex, &to);
              // LOGI("[Thread_Decoding] - DecoderThread[%d] is waking up", threadID);
            }
            // else {
            //  LOGV("[Thread_Decoding] - DecoderThread[%d] will be lock by semaphore", threadID);
            //  LeaveCriticalSection(&CurrentDisplayMutex);
            //  int v = 0;

            //  if (!sem_getvalue(&FrameStoreList[threadID].eachFrameSem, &v)) LOGV("[Thread_Decoding] - Semaphore Value = %d", v);
            //  if (!sem_wait(&FrameStoreList[threadID].eachFrameSem)) LOGV("[Thread_Decoding] - DecoderThread[%d] Leave this sempahore", threadID);
            //  if (!sem_getvalue(&FrameStoreList[threadID].eachFrameSem, &v)) LOGV("[Thread_Decoding] - Semaphore Value = %d", v);

            //  EnterCriticalSection(&CurrentDisplayMutex);
            // }

            if(SVCinfoList[threadID].EndOfStream || quit)
              return NULL;
          } // End of Display While

        }
        LeaveCriticalSection(&CurrentDisplayMutex);
        // LOGD("[Thread_Decoding:DeadLock] : Thread_%d, Leave CS", threadID);

      } // End of if
      // else {
        // LOGV("[Thread_Decoding]:%d - FrameSotreList[%d] IsEmpty()", __LINE__, threadID);
      // }

      // ---- set IDRindex ----//
      if (SVCinfoList[threadID].iFrame != -1 && SVCinfoList[threadID].iFrame % THREAD_NUM == threadID) {
        FrameStoreList[threadID].IDRindex = SVCinfoList[threadID].iFrame;
        LOGI("[Thread_Decoding] - DecoderThread_%d reset IDRindex to %d", threadID, FrameStoreList[threadID].IDRindex);
      }

      // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Switching test
      // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // EnterCriticalSection(&CurrentDisplayMutex);


      //---- auto setting ----//
      //++++ Temporal ++++//
      // if (SVCinfoList[threadID].iFrame == 5) {
      //  TEMPORAL_ID = 32;
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalId = TEMPORAL_ID;
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalCom = 2; //set specific temporal ID
      // }
      // else {
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalCom = 0;
      // }

      //++++ Layer ++++//
      // if (SVCinfoList[threadID].iFrame == 3) {
      //  LAYER_ID = 16;
      //  FrameStoreList[threadID].FrameDestory(&FrameStoreList[threadID]);
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;
      // }
      // if (SVCinfoList[threadID].iFrame == 9) {
      //  LAYER_ID = 32;
      //  FrameStoreList[threadID].FrameDestory(&FrameStoreList[threadID]);
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;
      // }
      // else if (SVCinfoList[threadID].iFrame == 12) {
      //  LAYER_ID = 16;
      //  FrameStoreList[threadID].FrameDestory(&FrameStoreList[threadID]);
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;
      // }
      //---- end of auto setting ----//

      //---- set temporal ID ----//
      // if (Temporal_changed[threadID] == 1) {
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalId = TEMPORAL_ID;
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalCom = 2; //set specific temporal ID
      //  Temporal_changed[threadID] = 0;
      //  LOGE("[Thread_Decoding] - DecoderThread[%d] changes Temporal ID to %d", threadID, SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalId);
      // }
      // else {
      //  SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalCom = 0;
      // }

      //---- set layer ID ----//
      // if (Layer_changed[threadID]) {
      //  // Swtich to lower layer
      //  if (pre_download_enhance > download_enhance) {
      //    FrameStoreList[threadID].FrameDestory(&FrameStoreList[threadID]);
      //    SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;

      //    Layer_changed[threadID] = 0;
      //    // enh_IDRindex[download_enhance - 1] = 0;
      //    LOGE("[Thread_Decoding] - DecoderThread[%d] changes layer to %d", threadID, LAYER_ID);
      //  }
      //  // Swithc to higher layer
      //  else if (download_enhance && enh_IDRindex[download_enhance - 1]) {
      //    if ( (SVCinfoList[threadID].iFrame >= enh_IDRindex[download_enhance - 1]) ) {
      //      FrameStoreList[threadID].FrameDestory(&FrameStoreList[threadID]);
      //      SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;

      //      Layer_changed[threadID] = 0;
      //      LOGE("[Thread_Decoding] - DecoderThread[%d] changes layer to %d", threadID, LAYER_ID);
      //    }
      //  }
      // }

      /* Temporal Switching */
      // int g_tid = monitor->GetTID();
      // if (g_tid != FrameStoreList[threadID].cur_TID) {
      //   SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalId = g_tid;
      //   SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalCom = 2; //set specific temporal ID
      //   FrameStoreList[threadID].cur_TID = g_tid;
      // }
      // else {
      //   SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalCom = 0;
      // }

      /* Spatial Switching */
      int g_sid = monitor->GetSID();
      int switchGOPNo = monitor->GetSwitchableGOPNo();
      // Switch Down
      if (g_sid < FrameStoreList[threadID].cur_SID) {
        LOGE("[Thread_Decoding] - Layer can switch down");
        FrameStoreList[threadID].FrameDestory(&FrameStoreList[threadID]);
        SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = g_sid;
        FrameStoreList[threadID].cur_SID = g_sid;
      }
      // Switch Up
      else if (g_sid > FrameStoreList[threadID].cur_SID) {
        if (switchGOPNo > 0 && FrameStoreList[threadID].IDRindex > switchGOPNo) {
          LOGI("[Thread_Decoding]:error - g_sid = %d, curSID = %d", g_sid, FrameStoreList[threadID].cur_SID);
          LOGE("[Thread_Decoding] - Layer can switch up");
          FrameStoreList[threadID].FrameDestory(&FrameStoreList[threadID]);
          SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = g_sid;
          FrameStoreList[threadID].cur_SID = g_sid;
        }
      }

      // LeaveCriticalSection(&CurrentDisplayMutex);
      // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // end of switching test
      // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      //LOGE("Thread %d, leave IDR proc", threadID);
    } // End check decoder_svc_NalUnit_NalUnitType_io or EndOfStream

    FrameStoreList[threadID].SetStartDecodingTime(&FrameStoreList[threadID]);
    // monitor->SetDecodeStartTime(threadID);

    // LOGI("[Thread_Decoding] - DecoderThread[%d] Goto switch cases", threadID);
    //------------------------------------------------Nal Unit Type---------------------------------------------------------------
    switch (SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0]) {
      // non-IDR
      case 1: {
        if (SVCinfoList[threadID].iFrame % THREAD_NUM != threadID) break;  // not my job

        // LOGE("Thread %d, case 1", threadID);
        *SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_Set_Pos_Pos = 8;

        // LOGE("Thread-%d, before slice_header_svc in [parsing non-IDR Frame]", threadID);
        slice_header_svc(4, SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_sps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_pps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_entropy_coding_flag,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_quantif,
            SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
            SVCinfoList[threadID].decoder_svc_Residu_SPS,
            SVCinfoList[threadID].decoder_svc_Residu_PPS,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_Set_Pos_Pos,
            SVCinfoList[threadID].decoder_svc_Residu_Slice,
            SVCinfoList[threadID].decoder_svc_Residu_Mmo,
            SVCinfoList[threadID].decoder_svc_Residu_RefL0,
            SVCinfoList[threadID].decoder_svc_Residu_RefL1,
            SVCinfoList[threadID].DqIdNextNal_Nal_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay,
            SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_Crop);
        // LOGE("ITD : %d, after slice_header_svc() in case 1", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0]);

        // printf("Bear A1: frame_num = %d, poc = %d\n", SVCinfoList[threadID].decoder_svc_Residu_Current_pic->frame_num, SVCinfoList[threadID].decoder_svc_Residu_Current_pic->poc);
        //------------------------------------------------entropy_coding_mode_flag---------------------------------------------------------------
        switch (SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_entropy_coding_flag[0]) {
          // Exp-init_nal_structGolomb coded
          case 0: {
            // LOGE("Thread %d, case 1.0", threadID);
            slice_base_layer_cavlc(
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_Set_Pos_Pos,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_pps_id,
                SVCinfoList[threadID].decoder_svc_VlcTab_PC_o,
                SVCinfoList[threadID].decoder_svc_Residu_SliceGroupId,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Residu_RefL1,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_MbToSliceGroupMap,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref);
            break;
          }
          case 1: { //CABAC
            // LOGE("Thread %d, case 1.1", threadID);
            slice_base_layer_cabac(
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_Set_Pos_Pos,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_pps_id,
                SVCinfoList[threadID].decoder_svc_Residu_SliceGroupId,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Residu_RefL1,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_slice_layer_CABAC_mv_cabac_l0_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_slice_layer_CABAC_mv_cabac_l1_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_slice_layer_CABAC_ref_cabac_l0_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_slice_layer_CABAC_ref_cabac_l1_o,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_MbToSliceGroupMap,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref);
            break;
          }
        }

        //------------------------------------------------Slice Type---------------------------------------------------------------
        *SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_Slice_type_SliceType_o = SVCinfoList[threadID].decoder_svc_Residu_Slice[0].slice_type;
        switch (SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_Slice_type_SliceType_o[0]) {
          case 0: { // P slice
            // LOGE("Thread-%d, parsing P slice", threadID);
            // LOGE("Thread %d, case 1.2", threadID);
            Decode_P_avc(
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_pps_id,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o->baseline_vectors,
                SVCinfoList[threadID].decoder_svc_Residu_RefL0,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_quantif,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                &SVCinfoList[threadID].decoder_svc_MvBuffer_Mv[SVCinfoList[threadID].decoder_svc_Residu_Current_pic->MvMemoryAddress],
                &SVCinfoList[threadID].decoder_svc_MvBuffer_Ref[SVCinfoList[threadID].decoder_svc_Residu_Current_pic->MvMemoryAddress >> 1],
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefV);
            break;
          }
          case 1: { // B slice
            // LOGE("Thread-%d, parsing B slice", threadID);
            // LOGE("Thread %d, case 1.3", threadID);
            Decode_B_avc(
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_pps_id,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o,
                SVCinfoList[threadID].decoder_svc_Residu_RefL0,
                SVCinfoList[threadID].decoder_svc_Residu_RefL1,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_quantif,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefV);
            break;
          }
          case 2: { // I slice
            // LOGE("Thread-%d, parsing I slice", threadID);
            // LOGE("Thread %d, case 1.4", threadID);
            Decode_I_avc(
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_pps_id,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o->baseline_vectors,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_quantif,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                &SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY[SVCinfoList[threadID].decoder_svc_Residu_Current_pic->MemoryAddress],
                &SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU[SVCinfoList[threadID].decoder_svc_Residu_Current_pic->MemoryAddress >> 2],
                &SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV[SVCinfoList[threadID].decoder_svc_Residu_Current_pic->MemoryAddress >> 2]);
            break;
          }
        }

        FinishFrameSVC(1920 * 1088 / 256,
            SVCinfoList[threadID].DqIdNextNal_Nal_o,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_sps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_SliceHeader_pps_id,
            SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
            SVCinfoList[threadID].decoder_svc_Residu_Slice,
            SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice[0],
            SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
            SVCinfoList[threadID].decoder_svc_Residu_Block,
            SVCinfoList[threadID].decoder_svc_Residu_Residu,
            SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
            SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
            SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
            SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref,
            SVCinfoList[threadID].decoder_svc_VideoParameter_Crop,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay,
            SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o,
            SVCinfoList[threadID].decoder_svc_Residu_Mmo,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
            SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o);
        // LOGE("ITD : %d, after FinishFrameSVC() in case 1", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0]);
        break;
      }

      // Indicates a IDR-frame!
      case 5: {
        if (SVCinfoList[threadID].iFrame % THREAD_NUM != threadID) break;  // not my job

        // LOGE("Thread-%d, parsing I Frame", threadID);
        // LOGE("Thread %d, case 5", threadID);

        svc_calculate_dpb((2186240 * (10 + 10 + 4 - 1)),
            (1920 * 1088 / 8 * 4 * (10 + 1)), 4,
            SVCinfoList[threadID].decoder_svc_Residu_Mmo,
            SVCinfoList[threadID].decoder_svc_Residu_SPS,
            SVCinfoList[threadID].DqIdNextNal_Nal_o);

        *SVCinfoList[threadID].decoder_svc_Nal_Compute_SetPos_Pos = 8;

        // LOGE("Thread-%d, before slice_header_svc in [parsing I Frame]", threadID);
        slice_header_svc(4, SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_sps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_pps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_entropy_coding_flag,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_quantif,
            SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
            SVCinfoList[threadID].decoder_svc_Residu_SPS,
            SVCinfoList[threadID].decoder_svc_Residu_PPS,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SetPos_Pos,
            SVCinfoList[threadID].decoder_svc_Residu_Slice,
            SVCinfoList[threadID].decoder_svc_Residu_Mmo,
            SVCinfoList[threadID].decoder_svc_Residu_RefL0,
            SVCinfoList[threadID].decoder_svc_Residu_RefL1,
            SVCinfoList[threadID].DqIdNextNal_Nal_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay,
            SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_Crop);
        // LOGE("ITD : %d, after slice_header_svc() in case 5", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0]);

        // printf("Bear A2: frame_num = %d, poc = %d\n", SVCinfoList[threadID].decoder_svc_Residu_Current_pic->frame_num, SVCinfoList[threadID].decoder_svc_Residu_Current_pic->poc);
        //------------------------------------------------entropy_coding_mode_flag---------------------------------------------------------------
        switch (SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_entropy_coding_flag[0]) {
          case 0: {
            // LOGE("Thread %d, case 5.0", threadID);
            slice_base_layer_cavlc(
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_SetPos_Pos,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_pps_id,
                SVCinfoList[threadID].decoder_svc_VlcTab_PC_o,
                SVCinfoList[threadID].decoder_svc_Residu_SliceGroupId,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Residu_RefL1,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_MbToSliceGroupMap,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref);
            break;
          }

          case 1: {
            // LOGE("Thread %d, case 5.1", threadID);
            slice_base_layer_cabac(
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_SetPos_Pos,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_pps_id,
                SVCinfoList[threadID].decoder_svc_Residu_SliceGroupId,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Residu_RefL1,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_main_CABAC_mv_cabac_l0_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_main_CABAC_mv_cabac_l1_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_main_CABAC_ref_cabac_l0_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_main_CABAC_ref_cabac_l1_o,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_MbToSliceGroupMap,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref);
            break;
          }
        }

        //------------------------------------------------Slice Type---------------------------------------------------------------
        // I slice
        // LOGE("Thread %d, case 5.2", threadID);
        Decode_I_avc(
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_sps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_pps_id,
            SVCinfoList[threadID].decoder_svc_Residu_Slice,
            SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
            SVCinfoList[threadID].decoder_svc_Residu_Residu,
            SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o->baseline_vectors,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_quantif,
            SVCinfoList[threadID].DqIdNextNal_Nal_o,
            &SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY[SVCinfoList[threadID].decoder_svc_Residu_Current_pic->MemoryAddress],
            &SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU[SVCinfoList[threadID].decoder_svc_Residu_Current_pic->MemoryAddress >> 2],
            &SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV[SVCinfoList[threadID].decoder_svc_Residu_Current_pic->MemoryAddress >> 2]);

        FinishFrameSVC(1920 * 1088 / 256,
            SVCinfoList[threadID].DqIdNextNal_Nal_o,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_sps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_SliceHeaderIDR_pps_id,
            SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
            SVCinfoList[threadID].decoder_svc_Residu_Slice,
            SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice[0],
            SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
            SVCinfoList[threadID].decoder_svc_Residu_Block,
            SVCinfoList[threadID].decoder_svc_Residu_Residu,
            SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
            SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
            SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
            SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref,
            SVCinfoList[threadID].decoder_svc_VideoParameter_Crop,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay,
            SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o,
            SVCinfoList[threadID].decoder_svc_Residu_Mmo,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
            SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o);
        // LOGE("ITD : %d, after FinishFrameSVC() in case 5.2", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0]);
        break;
      }

      // Supplemental enhancement information (SEI)
      case 6: {
        // LOGE("Thread-%d, parsing SEI", threadID);
        // LOGE("Thread %d, case 6", threadID);

        EnterCriticalSection(&DirtyMutex); // Bear
          sei_rbsp(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
              SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size[0],
              SVCinfoList[threadID].decoder_svc_Residu_SPS,
              SVCinfoList[threadID].decoder_svc_Nal_Compute_sei_rbsp_Sei);
        LeaveCriticalSection(&DirtyMutex); // Bear
        break;
      }

      // Sequence parameter set (SPS)
      case 7: {
        // LOGE("Thread-%d, parsing SPS", threadID);
        // LOGE("Thread %d, case 7", threadID);

        EnterCriticalSection(&DirtyMutex); // Bear
          seq_parameter_set(SVCinfoList[threadID].GetNalBytesAu_rbsp_o, SVCinfoList[threadID].decoder_svc_Residu_SPS);
        LeaveCriticalSection(&DirtyMutex); // Bear
        break;
      }

      // Picture parameter set (PPS)
      case 8: {
        // LOGE("Thread-%d, parsing PPS", threadID);
        // LOGE("Thread %d, case 8", threadID);

        EnterCriticalSection(&DirtyMutex); // Bear
          pic_parameter_set(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
              SVCinfoList[threadID].decoder_svc_Residu_SliceGroupId,
              SVCinfoList[threadID].decoder_svc_Residu_PPS,
              SVCinfoList[threadID].decoder_svc_Residu_SPS,
              SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size[0]);
        LeaveCriticalSection(&DirtyMutex); // Bear
        break;
      }

      case 9: {
        // LOGE("Thread %d, case 9", threadID);
        // printf("Damn great!\n");
        // exit(0);
        break;
      }

      // Indicates the end of video..
      case 11: {
        // LOGE("Thread %d, case 11", threadID);

        EnterCriticalSection(&DirtyMutex); // Bear
          FlushSVCFrame(SVCinfoList[threadID].decoder_svc_Residu_SPS,
              SVCinfoList[threadID].DqIdNextNal_Nal_o,
              SVCinfoList[threadID].decoder_svc_Residu_Mmo,
              SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o,
              SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o,
              SVCinfoList[threadID].decoder_svc_VideoParameter_Crop,
              SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay,
              SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o);
        LeaveCriticalSection(&DirtyMutex); // Bear
        isFinished = 1;
        SVCinfoList[threadID].EndOfStream = 1;
        // sem_post(&ReturnSem);
        return NULL;
        break;
      }

      // Prefix NAL unit in scalable extension
      case 14: {
        // LOGE("Thread %d, case 14", threadID);

        EnterCriticalSection(&DirtyMutex); // Bear
          PrefixNalUnit(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
              SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
              SVCinfoList[threadID].DqIdNextNal_Nal_o,
              SVCinfoList[threadID].decoder_svc_Residu_Mmo,
              SVCinfoList[threadID].decoder_svc_Residu_SPS,
              SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice);
        LeaveCriticalSection(&DirtyMutex); // Bear
        break;
      }

      // Subset sequence parameter set
      case 15: {
        // LOGE("Thread-%d, parsing Subset SPS", threadID);
        // LOGE("Thread %d, case 15", threadID);

        EnterCriticalSection(&DirtyMutex); // Bear
          subset_sps(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
              SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
              SVCinfoList[threadID].decoder_svc_Residu_SPS,
              SVCinfoList[threadID].DqIdNextNal_Nal_o);
        LeaveCriticalSection(&DirtyMutex); // Bear
        break;
      }

      // Coded slice in scalable extension
      case 20: {
        if (SVCinfoList[threadID].iFrame % THREAD_NUM != threadID) break;  // not my job
        // LOGE("Thread-%d, parsing Scalable Extension", threadID);
        // LOGE("Thread %d, case 20", threadID);

        NalUnitHeader(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_nal_unit_header_svc_ext_20_pos_o,
            SVCinfoList[threadID].DqIdNextNal_Nal_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice);

        // LOGE("Thread-%d, before slice_header_svc in [parsing Scalable Extension]", threadID);
        slice_header_svc(4, SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_sps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_pps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_entropy_coding_flag,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_quantif,
            SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
            SVCinfoList[threadID].decoder_svc_Residu_SPS,
            SVCinfoList[threadID].decoder_svc_Residu_PPS,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_nal_unit_header_svc_ext_20_pos_o,
            SVCinfoList[threadID].decoder_svc_Residu_Slice,
            SVCinfoList[threadID].decoder_svc_Residu_Mmo,
            SVCinfoList[threadID].decoder_svc_Residu_RefL0,
            SVCinfoList[threadID].decoder_svc_Residu_RefL1,
            SVCinfoList[threadID].DqIdNextNal_Nal_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay,
            SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_Crop);
        // LOGE("FC : ImgToDisplay = %d, after slice_header_svc() in case 20 with thread %d", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0], threadID);

        // printf("ThreadID = %d A3: frame_num = %d, poc = %d\n",threadID, SVCinfoList[threadID].decoder_svc_Residu_Current_pic->frame_num, SVCinfoList[threadID].decoder_svc_Residu_Current_pic->poc);
        switch (SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_entropy_coding_flag[0]) {
          case 0: {
            // LOGE("Thread %d, case 20.1.0", threadID);

            slice_data_in_scalable_extension_cavlc(1920 * 1088 / 256,
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_nal_unit_header_svc_ext_20_pos_o,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_pps_id,
                SVCinfoList[threadID].decoder_svc_VlcTab_PC_o,
                SVCinfoList[threadID].decoder_svc_Residu_SliceGroupId,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_MbToSliceGroupMap,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice);
            break;
          }
          case 1: {
            // LOGE("Thread %d, case 20.1.1", threadID);

            SliceCabac(1920 * 1088 / 256,
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_nal_unit_header_svc_ext_20_pos_o,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_sps_id,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_pps_id,
                SVCinfoList[threadID].decoder_svc_Residu_SliceGroupId,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_Layer_SliceLayerCabac_mv_cabac_l0_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_Layer_SliceLayerCabac_mv_cabac_l1_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_Layer_SliceLayerCabac_ref_cabac_l0_o,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_Layer_SliceLayerCabac_ref_cabac_l1_o,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].decoder_svc_Residu_MbToSliceGroupMap,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice);
            break;
          }
        }
        *SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_type_SliceType_o = SVCinfoList[threadID].decoder_svc_Residu_Slice[0].slice_type;

        switch (SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Slice_type_SliceType_o[0]) {
          case 0: {
            // LOGE("Thread %d, case 20.1.2", threadID);

            Decode_P_svc(1920 * 1088 / 256,
                SVCinfoList[threadID].decoder_svc_Residu_SPS,
                SVCinfoList[threadID].decoder_svc_Residu_PPS,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o->baseline_vectors,
                SVCinfoList[threadID].decoder_svc_Residu_RefL0,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_quantif,
                SVCinfoList[threadID].decoder_svc_Svc_Vectors_PC_Svc_Vectors,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_px,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_py,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_Upsampling_tmp,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_xk16,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_xp16,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_yk16,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_P_svc_yp16,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefV);
            break;
          }
          case 1: {
            // LOGE("Thread %d, case 20.2.1", threadID);

            Decode_B_svc(1920 * 1088 / 256,
                SVCinfoList[threadID].decoder_svc_Residu_SPS,
                SVCinfoList[threadID].decoder_svc_Residu_PPS,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o,
                SVCinfoList[threadID].decoder_svc_Residu_RefL0,
                SVCinfoList[threadID].decoder_svc_Residu_RefL1,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_quantif,
                SVCinfoList[threadID].decoder_svc_Svc_Vectors_PC_Svc_Vectors,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_px,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_py,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_Upsampling_tmp,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_xk16,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_xp16,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_yk16,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_Decode_IPB_svc_Decode_B_svc_yp16,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
                SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
                SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_ResiduBuffer_RefV);
            // LOGE("Thread %d, case 20.3-After", threadID);
            break;
          }
          case 2: {
            // LOGE("Thread %d, case 20.2.2", threadID);

            Decode_I_svc(1920 * 1088 / 256,
                SVCinfoList[threadID].decoder_svc_Residu_SPS,
                SVCinfoList[threadID].decoder_svc_Residu_PPS,
                SVCinfoList[threadID].decoder_svc_Residu_Slice,
                SVCinfoList[threadID].DqIdNextNal_Nal_o,
                SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
                SVCinfoList[threadID].decoder_svc_Residu_Block,
                SVCinfoList[threadID].decoder_svc_Residu_Residu,
                SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o->baseline_vectors,
                SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
                SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_quantif,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
                SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV);
            break;
          }
        }
        // LOGE("Thread %d, case 20.5", threadID);

        FinishFrameSVC(1920 * 1088 / 256,
            SVCinfoList[threadID].DqIdNextNal_Nal_o,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_sps_id,
            SVCinfoList[threadID].decoder_svc_Nal_Compute_slice_layer_in_scalable_ext_20_slice_header_21_pps_id,
            SVCinfoList[threadID].decoder_svc_Residu_Current_pic,
            SVCinfoList[threadID].decoder_svc_Residu_Slice,
            SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice[0],
            SVCinfoList[threadID].decoder_svc_Residu_SliceTab,
            SVCinfoList[threadID].decoder_svc_Residu_Block,
            SVCinfoList[threadID].decoder_svc_Residu_Residu,
            SVCinfoList[threadID].decoder_svc_MvBuffer_Mv,
            SVCinfoList[threadID].decoder_svc_MvBuffer_1_Mv,
            SVCinfoList[threadID].decoder_svc_MvBuffer_Ref,
            SVCinfoList[threadID].decoder_svc_MvBuffer_1_Ref,
            SVCinfoList[threadID].decoder_svc_VideoParameter_Crop,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay,
            SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o,
            SVCinfoList[threadID].decoder_svc_Residu_Mmo,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
            SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
            SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o,
            SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o);
        // LOGE("FC : ImgToDisplay = %d, after FinishFrameSVC() in case 20 with thread %d", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0], threadID);
        // LOGE("Thread %d, case 20.5-After", threadID);

        // if(SVCinfoList[threadID].iFrame%THREAD_NUM!=threadID) break;  // not my job
        // printf("ThreadID = %d A4: frame_num = %d, poc = %d\n",threadID, SVCinfoList[threadID].decoder_svc_Residu_Current_pic->frame_num, SVCinfoList[threadID].decoder_svc_Residu_Current_pic->poc);

        // retrieve
        // EnterCriticalSection(&DirtyMutex); // Bear
        // LOGE("FC : ImgToDisplay = %d, before switch with thread %d", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0], threadID);
        switch (SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0]) {
          case 1: {
            EnterCriticalSection(&DirtyMutex); // Bear
            {
              // LOGE("Thread %d, case 20.3.1", threadID);
              int XDIM
                = ((int *) SVCinfoList[threadID].Display_1_Extract_Image_Y_o)[0]
                = *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o;

              int YDIM
                = ((int *) SVCinfoList[threadID].Display_1_Extract_Image_Y_o)[1]
                = *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o;

              uchar *Y = SVCinfoList[threadID].Display_1_Extract_Image_Y_o + 8;
              uchar *U = Y + (XDIM + 32) * YDIM;
              uchar *V = U + (XDIM + 32) * YDIM / 4;

              // LOGE("start extract");
              extract_picture(XDIM, YDIM, 16,
                  SVCinfoList[threadID].decoder_svc_VideoParameter_Crop[0],
                  SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
                  SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
                  SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
                  SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o[0],
                  Y, U, V);
              // LOGE("end extract");

              // fill data to frame store
              FrameStoreList[threadID].frameWidth = *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o;
              FrameStoreList[threadID].frameHeight = *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o;

              // LOGD("[Thread_Decoding] - FillFrame in case 20.6 by Thread_%d]", threadID);
              FrameStoreList[threadID].FillFrame(&FrameStoreList[threadID], Y, U, V);
              // LOGE("end fill");
            }
            LeaveCriticalSection(&DirtyMutex);  // Bear
            break;
          }
          case 2: {
            EnterCriticalSection(&DirtyMutex);  // Bear
            {
              // LOGE("Thread %d, case 20.3.2", threadID);
              int XDIM
                = ((int *) SVCinfoList[threadID].Display_1_Extract_1_Image_Y_o)[0]
                = *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o;
              int YDIM
                = ((int *) SVCinfoList[threadID].Display_1_Extract_1_Image_Y_o)[1]
                = *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o;

              uchar *Y = SVCinfoList[threadID].Display_1_Extract_1_Image_Y_o + 8;
              uchar *U = Y + (XDIM + 32) * YDIM;
              uchar *V = U + (XDIM + 32) * YDIM / 4;

              // LOGE("start extract");

              extract_picture(XDIM, YDIM, 16,
                  SVCinfoList[threadID].decoder_svc_VideoParameter_Crop[0],
                  SVCinfoList[threadID].decoder_svc_PictureBuffer_RefY,
                  SVCinfoList[threadID].decoder_svc_PictureBuffer_RefU,
                  SVCinfoList[threadID].decoder_svc_PictureBuffer_RefV,
                  SVCinfoList[threadID].decoder_svc_VideoParameter_address_pic_o[0],
                  Y, U, V);

              // LOGE("end extract");
              // fill data to frame store
              FrameStoreList[threadID].frameWidth  = *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o;
              FrameStoreList[threadID].frameHeight = *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o;

              // LOGD("[Thread_Decoding] - FillFrame in case 20.7 by Thread_%d", threadID);
              FrameStoreList[threadID].FillFrame(&FrameStoreList[threadID], Y, U, V);
              // LOGE("end fill");
            }
            LeaveCriticalSection(&DirtyMutex);  // Bear
            break;
          }
        }
        // LeaveCriticalSection(&DirtyMutex); // Bear

        break;
      }
    }

    // monitor->SetDecodeEndTime(threadID);
    FrameStoreList[threadID].ComputeDecodeTime(&FrameStoreList[threadID]);
    // LOGI("[Thread_Decoding] - DecoderThread[%d] Leave switch cases", threadID);
    // LOGI("[Thread_Decoding] - DecoderThread[%d] Finished one packet", threadID);
  } // End main for loopl

  // LOGE("Thread %d, Finish ALL", threadID);
  return NULL;
}


/*
 * These functions are for SVC Player
 */
extern "C" JNIEXPORT jint JNICALL
// Java_my_app_SVCPlayer_nativeDecodeSVC(JNIEnv * env, jobject obj, jobjectArray bitmapArray, jint bitmapNum) {
// Java_my_app_SVCPlayer_nativeDecodeSVC(JNIEnv * env, jobject obj) {
Java_my_app_SVCPlayer_nativeStartSVCDecoder(JNIEnv * env, jobject obj, jint mode) {
  // Start Download segments
  streamingMODE = mode;
  fileStreamInit();

  int tid = monitor->GetTID();
  int sid = monitor->GetSID();

  for (i = 0; i < THREAD_NUM; ++i) {
    // Value Setting
    FrameStoreConstructor(&FrameStoreList[i]);
    FrameStoreList[i].ThreadID = i;
    FrameStoreList[i].IDRindex = i;
    FrameStoreList[i].cur_TID  = tid;
    FrameStoreList[i].cur_SID  = sid;

    // setDecodeTime[i] = 0;
    // setStartDecodeTime[i] = 0;

    // Start Deocding
    initialize(i);
    pthread_create(&ThreadList[i], &attr, Thread_Decoding, (void*) i);
  }
}
extern "C" JNIEXPORT void JNICALL
Java_my_app_SVCPlayer_nativeSVCInit(JNIEnv * env, jobject obj, jint bitmapNum) {
  LOGI("[nativeSVCInit] - GOP Frames = %d", bitmapNum);

  bitmapFilledCounter = 0;
  CurrentDisplayGOP   = 0;
  bitmapFilledMax     = bitmapNum;

  /* init thread data */
  ThreadList  = (pthread_t*) calloc(THREAD_NUM, sizeof(pthread_t));
  if (ThreadList == NULL) LOGE("[Error] - ThreadList calloc Error");
  
  SVCinfoList = (SVCinfo*) calloc(THREAD_NUM, sizeof(SVCinfo));
  if (SVCinfoList == NULL) LOGE("[Error] - SVCinfoList calloc Error");
  
  FrameStoreList = (FrameStore*) calloc(THREAD_NUM, sizeof(FrameStore));
  if (FrameStoreList == NULL) LOGE("[Error] - FrameStoreList calloc Error");

  // time_start = time(NULL);
  framecounter = 0;
  flagError    = 0;

  // Bear, increase stack size
  size_t stacksize;
  pthread_attr_init(&attr);
  pthread_attr_getstacksize(&attr, &stacksize);
  stacksize = 1048576;  // 8MB?
  pthread_attr_setstacksize(&attr, stacksize);

  // TODO: move following array into FrameStoreList separate
  // start_decode_time  = (timeval*) calloc(THREAD_NUM, sizeof(timeval));
  // end_decode_time    = (timeval*) calloc(THREAD_NUM, sizeof(timeval));
  // setDecodeTime      = (int*) calloc(THREAD_NUM, sizeof(int));
  // setStartDecodeTime = (int*) calloc(THREAD_NUM, sizeof(int));

  // if (start_decode_time  == NULL) LOGE("[Error] - start_decode_time calloc Error");
  // if (end_decode_time    == NULL) LOGE("[Error] - end_decode_time calloc Error");
  // if (setDecodeTime      == NULL) LOGE("[Error] - setDecodeTime calloc Error");
  // if (setStartDecodeTime == NULL) LOGE("[Error] - setStartDecodeTime calloc Error");

  monitor = new MyMonitor(THREAD_NUM);
}
extern "C" JNIEXPORT jboolean JNICALL
Java_my_app_SVCPlayer_nativeIsDone (JNIEnv *env, jobject obj) {
  return quit;
}

extern "C" JNIEXPORT void JNICALL
Java_my_app_SVCPlayer_nativeSetFilePath(JNIEnv * env, jobject obj, jstring BaseUrl, jstring Prefix) {
  LOGI("[nativeSetFilePath]");

  base_url = (char*) env->GetStringUTFChars(BaseUrl, NULL);
  prefix   = (char*) env->GetStringUTFChars(Prefix, NULL);
}
extern "C" JNIEXPORT void JNICALL
Java_my_app_SVCPlayer_nativeSetOutputFilePath(JNIEnv * env, jobject obj, jstring file_path) {
  char *path = (char*) env->GetStringUTFChars(file_path, NULL);

  if (monitor) {
    monitor->SetOutputFilePath(path);
  }
  else {
    LOGE("[nativeSetOutputFilePath] - Monitor is NULL");
  }
}
extern "C" JNIEXPORT void JNICALL
Java_my_app_SVCPlayer_nativeSetLayerID(JNIEnv * env, jobject obj, jint layerID) {
  LOGI("[nativeSetLayerID] - Set LayerID to %d", layerID);

  // LAYER_ID = layerID;
  monitor->SetSID(layerID);

  pre_download_enhance = download_enhance;
  switch (layerID) {
    case 8:
    case 16:
      download_enhance = 0;
      break;
    case 32:
      download_enhance = 1;
      break;
    case 48:
      download_enhance = 2;
      break;
    default:
      LOGE("[nativeSetLayerID] - layer id setting error");
      download_enhance = 0;
      break;
  }

  // if (LAYER_ID == 16 || LAYER_ID == 8) {
  //  download_enhance = 0;
  //  // maxThreshold =  3145728;   // 3MB
  //  // MaxFileSize = 5242880;       // 5MB
  // }
  // else if (LAYER_ID == 32) {
  //  download_enhance = 1;
  //  // maxThreshold =  10485760;  // 10MB
  //  // MaxFileSize = 15728640;      // 15MB
  // }
  // else if (LAYER_ID == 48) {
  //  download_enhance = 2;
  //  // maxThreshold =  31457280;  // 30MB
  //  // MaxFileSize = 47185920;      // 40MB
  // }

  // TODO: move this array into FrameStoreList separate
  // for (int i = 0; i < THREAD_NUM; i++) {
  //   Layer_changed[i] = 1;
  // }

  // TODO: move this array into MyMonitor.cpp
  enh_IDRindex[0] = enh_IDRindex[1] = 0;
}
extern "C" JNIEXPORT void JNICALL
Java_my_app_SVCPlayer_nativeSetTemporalID(JNIEnv * env, jobject obj, jint temporalID) {
  LOGI("[nativeSetTemporalID] - Set LayerID to %d", temporalID);

  // TEMPORAL_ID = temporalID;
  monitor->SetTID(temporalID);

  // // TODO: move this array into FrameStoreList separate
  // for (int i = 0; i < THREAD_NUM; i++) {
  //   Temporal_changed[i] = 1;
  // }
}
extern "C" JNIEXPORT void JNICALL
Java_my_app_SVCPlayer_nativeSetThreadNum(JNIEnv * env, jobject obj, jint threadNum) {
  THREAD_NUM = threadNum;
  LOGI("[nativeSetThreadNum] - set %d Decoder thread(s)", THREAD_NUM);

  // Temporal_changed = (int*) calloc(THREAD_NUM, sizeof(int));
  // Layer_changed    = (int*) calloc(THREAD_NUM, sizeof(int));

  // if (Temporal_changed == NULL) LOGE("[Error] - Temporal_changed calloc Error");
  // if (Layer_changed    == NULL) LOGE("[Error] - Layer_changed calloc Error");

  pre_download_enhance = download_enhance = 0;
  enh_IDRindex[0]      = enh_IDRindex[1]  = 0;
}
extern "C" JNIEXPORT void JNICALL
Java_my_app_SVCPlayer_nativeSetStartTime(JNIEnv * env, jobject obj) {
  //  struct timeval tp;
  //  time_start = tp.tv_sec*1000000 + tp.tv_usec;
  //  LOGE("TIME : start time: %d s + %d ns", tp.tv_sec, tp.tv_usec);
  //  time_start = time(NULL);

  gettimeofday(&time_start, 0);
}

extern "C" JNIEXPORT jint JNICALL
Java_my_app_SVCPlayer_nativeGetWidth(JNIEnv * env, jobject obj) {
  if (init) {
    return FrameStoreList[CurrentDisplayGOP % THREAD_NUM].frameWidth;
  }
  return 0;
}
extern "C" JNIEXPORT jint JNICALL
Java_my_app_SVCPlayer_nativeGetHeight(JNIEnv * env, jobject obj) {
  if (init) {
    return FrameStoreList[CurrentDisplayGOP % THREAD_NUM].frameHeight;
  }
  return 0;
}
extern "C" JNIEXPORT jint JNICALL
Java_my_app_SVCPlayer_nativeGetLayerID(JNIEnv * env, jobject obj) {
  // return LAYER_ID;
  return monitor->GetSID();
}
extern "C" JNIEXPORT jint JNICALL
Java_my_app_SVCPlayer_nativeGetTemporalID(JNIEnv * env, jobject obj) {
  // return TEMPORAL_ID;
  return monitor->GetTID();
}
extern "C" JNIEXPORT jfloat JNICALL
Java_my_app_SVCPlayer_nativeGetNetworkDelay(JNIEnv * env, jobject obj) {
  // return (network_delay/1000);
  return monitor->GetTransferDelay();
}
extern "C" JNIEXPORT jint JNICALL
Java_my_app_SVCPlayer_nativeGetSwitchGOP(JNIEnv * env, jobject obj) {
  // return (download_enhance ? enh_IDRindex[download_enhance - 1] : -1);
  return monitor->GetSwitchableGOPNo();
}
extern "C" JNIEXPORT jint JNICALL
Java_my_app_SVCPlayer_nativeGetDownloadedGOPs(JNIEnv * env, jobject obj) {
  return monitor->GetDownloadedGOPs();
}
extern "C" JNIEXPORT jint JNICALL
Java_my_app_SVCPlayer_nativeGetDecodedGOPs(JNIEnv * env, jobject obj) {
  // return monitor->GetFrames();
  return CurrentDisplayGOP;
}
extern "C" JNIEXPORT jfloat JNICALL
Java_my_app_SVCPlayer_nativeGetDecodeDelay(JNIEnv * env, jobject obj) {
  return monitor->GetDecodingDelay();
}
extern "C" JNIEXPORT jfloat JNICALL
Java_my_app_SVCPlayer_nativeGetThroughput(JNIEnv * env, jobject obj) {
 // int now_time = time(NULL);
 // float result = 0.0;
 // //  LOGE("TIME : tds:%f, tsd:%d, nt:%d", totalDataSize, time_start_download, now_time);
 // if ((totalDataSize != 0.0) && (now_time != time_start_download)) {
 //   result = ((8.0*totalDataSize) / (now_time - time_start_download));
 // }

 // totalDataSize = 0.0;
 // time_start_download = time(NULL);

 // return result;
 return monitor->GetThroughputNoResetInMbit();
}
extern "C" JNIEXPORT jfloat JNICALL
Java_my_app_SVCPlayer_nativeGetCurrentFPS (JNIEnv *env, jobject obj) {
  return monitor->GetGOPFPS();
}
extern "C" JNIEXPORT jfloat JNICALL
Java_my_app_SVCPlayer_nativeGetCumulativeFPS (JNIEnv *env, jobject obj) {
  return monitor->GetTotalFPS();
}

// extern "C" JNIEXPORT void JNICALL
// Java_my_app_SVCPlayer_nativeClear(JNIEnv * env, jobject obj) {
void CloseAll () {
  if (handler) handler->onStopButtonPressed();
  // if (extractor) extractor->SetEOS(true);

  JoinThread(&request_thread);
  if (handler) delete handler;

  EnterCriticalSection(&StreamBufferMutex);
  WakeAllConditionVariable(&CanPlayCond);
  LeaveCriticalSection(&StreamBufferMutex);

  EnterCriticalSection(&CurrentDisplayMutex);
  WakeAllConditionVariable(&CurrentDisplayCond);
  LeaveCriticalSection(&CurrentDisplayMutex);

  for (i = 0; i < THREAD_NUM; i++) {
    JoinThread(&ThreadList[i]);
  }

  free(SVCinfoList);
  free(ThreadList);
  // free(Temporal_changed);
  LOGE("clear done");
}

extern "C" JNIEXPORT jboolean JNICALL
Java_my_app_DASHSelector_nativeInitDASH(JNIEnv * env, jobject obj) {
  handler = new DASHHandler();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_my_app_DASHSelector_nativeParseMPD(JNIEnv * env, jobject obj, jstring url) {
  char *parsing_url = (char*) env->GetStringUTFChars(url, NULL);
  string ss(parsing_url);
  
  if (!handler) {
    handler = new DASHHandler();
  }

  handler->onDownloadMPDPressed(ss);
  if (!handler->GetMPD()) return false;

  return true;
}

extern "C" JNIEXPORT void JNICALL
Java_my_app_DASHSelector_nativeOnSettingChanged(JNIEnv * env, jobject obj, jint qualityLevel) {
  if (!handler) {
    LOGE("[nativeOnSettingChanged] - DASH Handler is NULL");
  }
  else {
    handler->onSettingsChanged(0, 0, qualityLevel);
  }
}

/*
 * These functions are for SDL2
 */
int main (int argc, char* argv[]) {
  SDLEventHandler sdl_handler(monitor);
  // monitor = handler.GetMonitor();

  // StartSVCDecoder();
  sdl_handler.StartLoop();

  SDL_Quit();
  CloseAll();

  return 0;
}