#include <android/bitmap.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <cpu-features.h>
#include <errno.h>
#include <jni.h>
#include <prof.h>
#include <pthread.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>

#include "GetHttpStream.h"
#include "main_data.h"
#include "Read_h264.h"
#include "svc_type.h"
#include "type.h"

#define  LOG_TAG    "libsvc"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,   LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,    LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,    LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,   LOG_TAG, __VA_ARGS__)

/* Thread Handler */
#define CONDITION_VARIABLE  pthread_cond_t
#define CRITICAL_SECTION    pthread_mutex_t

#define DeleteConditionVariable(cond_p)             pthread_cond_destroy(cond_p)
#define DeleteCriticalSection(mutex_p)              pthread_mutex_destroy(mutex_p)
#define EnterCriticalSection(mutex_p)               pthread_mutex_lock(mutex_p)
#define InitializeConditionVariable(cond_p)         pthread_cond_init(cond_p, NULL)
#define InitializeCriticalSection(mutex_p)          pthread_mutex_init(mutex_p, NULL)
#define JoinThread(handle)                          pthread_join(*(handle), NULL)
#define LeaveCriticalSection(mutex_p)               pthread_mutex_unlock(mutex_p)
#define PortableSleep(seconds)                      usleep(seconds * 1000000)
#define WaitConditionVariableCS(cond_p, mutex_p)    pthread_cond_wait(cond_p, mutex_p)
#define WakeAllConditionVariable(cond_p)            pthread_cond_broadcast(cond_p)
#define WakeConditionVariable(cond_p)               pthread_cond_signal(cond_p)

/* Type Definded */
#define dpb_type        unsigned char
#define image_type      unsigned char
#define prec_synchro    int
#define stream          unsigned char
#define uchar           unsigned char
#define uint            unsigned int
#define ulong           unsigned long
#define ushort          unsigned short

typedef unsigned char       byte;   // Byte is a char
typedef unsigned short int  word16; // 16-bit word is a short int
typedef unsigned int        word32; // 32-bit word is an int

/* Ring Buffer size Defended */
#define StopDownloadSpace   15728640    // Not used, we want to use this to lock request thread.            Test Value: 12582912, 5242880, 15728640
#define StopDecodeSpace     1572864     // when available data size small than this value will be locked.   Test Value: 2097152, 1572864
#define MaxFileSize         20971520    // the size of ringBuffer called StreamBuffer.                      Test Value: 20971520 (Default : 10185706)
#define RepeatSegments      70          // 70 is total segments of test file

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
    int alloc_size;
    unsigned char* RGB;
} EachFrame;

// Frame Store for each Thread
typedef struct _FrameStore {
    int displayCounter;
    int edge;
    int fps;
    int frameAllocated;
    int frameCounter;
    int frameWidth, frameHeight;
    int IDRindex;
    int threadNo;
    int waitForDisplay;
    int displayedFrames;
    int hasStarTime;

    EachFrame frameList[100]; // hard code it, I don't want to realloc it, there is no vector in C...

    // struct timeb start_decode_time;
    // struct timeb end_decode_time;
    struct timeval start_decode_time;
    struct timeval end_decode_time;
    unsigned long long tempDecodingTime;
    unsigned long long decodingTimeMS;

    // do what member function doing in C++...
    void  (*computeAvgDecodingTime) (struct _FrameStore *thz);
    float (*getAvgDecodingTime) (struct _FrameStore *thz);
    void  (*setStartTime) (struct _FrameStore *thz);
    void  (*setDecodingDelay) (struct _FrameStore *thz);

    int   (*fillFrame)(struct _FrameStore* thz, unsigned char* Y, unsigned char* U, unsigned char* V);
    int   (*isEmpty)(struct _FrameStore* thz);
    void  (*checkResolution)(struct _FrameStore* thz);

    void  (*Display)(struct _FrameStore* thz);
    void  (*DisplaySingle)(struct _FrameStore* thz);

    void  (*frameDestory)(struct _FrameStore* thz);
    void  (*frameInit)(struct _FrameStore* thz, EachFrame* newFrame, int i);
} FrameStore;


/* Multi-Threading */
// #define THREAD_NUM 3
// static pthread_t ThreadList[THREAD_NUM];
static int       THREAD_NUM  = 1;
static pthread_t *ThreadList = NULL;
static int       *threadIDs  = NULL;
static pthread_t request_thread;

CRITICAL_SECTION FrameCounterMutex = PTHREAD_MUTEX_INITIALIZER; // protect race condition when set the FramecCounter

static CRITICAL_SECTION DownloadMutex       = PTHREAD_MUTEX_INITIALIZER;    // lock until free size of StreamBuffer can put new data
static CONDITION_VARIABLE CanDownloadCond   = PTHREAD_COND_INITIALIZER;     // related condition

static CRITICAL_SECTION StreamBufferMutex   = PTHREAD_MUTEX_INITIALIZER;    // lock until data enough
static CONDITION_VARIABLE CanPlayCond       = PTHREAD_COND_INITIALIZER;     // related condition

//static CRITICAL_SECTION FirstDownloadMutex = PTHREAD_MUTEX_INITIALIZER;
//static CONDITION_VARIABLE FirstDownloadCond = PTHREAD_COND_INITIALIZER;

CRITICAL_SECTION BufferReallocMutex = PTHREAD_MUTEX_INITIALIZER;            // protect race condition when copy downloaded data to StreamBuffer
CRITICAL_SECTION DataSizeMutex      = PTHREAD_MUTEX_INITIALIZER;            // protect race condition when set the available data size
CRITICAL_SECTION EnhIDRMutex        = PTHREAD_MUTEX_INITIALIZER;            // protect when set enh_IDRindex

static CRITICAL_SECTION     DirtyMutex          = PTHREAD_MUTEX_INITIALIZER;
static CRITICAL_SECTION     CurrentDisplayMutex = PTHREAD_MUTEX_INITIALIZER;
static CRITICAL_SECTION     FrameBufferMutex    = PTHREAD_MUTEX_INITIALIZER;
static CONDITION_VARIABLE   CurrentDisplayCond  = PTHREAD_COND_INITIALIZER;
static CONDITION_VARIABLE   FrameBufferCond     = PTHREAD_COND_INITIALIZER;

//static char* FILE_PATH;   // Get file segment from HTTP Get request
/* Used for requesting segment */
int     data_size       = 0;
int     displayTimes    = 0;
char    *seqBuf         = NULL;
char    *prefix         = NULL;
char    *base_url       = NULL;

/* For Switch Event */
static int      LAYER_ID;
static int      TEMPORAL_ID;
static int      *Temporal_changed = NULL;      // if is True then change temporal ID
static int      *Layer_changed = NULL;         // if it's True then change layer ID
static int      download_enhance;       // set number means to start downloading which enhancement layer.
static int      pre_download_enhance;   // set the number previous enhancement layer.
static int      CurrentDisplay = 0;
/*
 * indicate when will switch to enhancement layer
 * if enh_IDRindex[0] is True, then can switch to enhancement layer one.
 * if enh_IDRindex[1] is True, then can switch to enhancement layer two.
 */
static int enh_IDRindex[2];

/* For Measurement */
/* Decode */
// static long time_start;
// static struct timeval    *start_decode_time  = NULL;
// static struct timeval    *end_decode_time    = NULL;
// static int               *setStartDecodeTime = NULL;
/* Network */
static struct timeval   time_start; // Used for getting network delay
long                    network_delay   = 0;
static float            totalDataSize   = 0;
static struct timeval   time_start_download;

static int i;
static int init = 0;
static void* pixels[64];
static AndroidBitmapInfo info;
static int getMinBytes = 1622016;   // default 1622016, each run will get this to do some operations.
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

void toRGB565(byte yuvs[], int width, int height, byte rgbs[], int sw, int sh) {
    //the end of the luminance data
    const int lumEnd = width * height;
    //points to the next luminance value pair
    int lumPtr = 0;
    //points to the next chromiance value pair
    int chrPtr = lumEnd;
    //points to the next byte output pair of RGB565 value
    int outPtr = 0;
    //the end of the current luminance scanline
    int lineEnd = width;

    while (1) {
        //skip back to the start of the chromiance values when necessary
        if (lumPtr == lineEnd) {
            if (lumPtr == lumEnd)
                break; //we've reached the end
            //division here is a bit expensive, but's only done once per scanline
            chrPtr = lumEnd + ((lumPtr / width) >> 1) * (width >> 1);
            lineEnd += width;
            //to erase green bar
            outPtr -= 64;            //32*2
        }

        //read the luminance and chromiance values
        const int Y1 = yuvs[lumPtr++];
        const int Y2 = yuvs[lumPtr++];
        const int Cb = (yuvs[chrPtr]) - 128;
        const int Cr = (yuvs[chrPtr + width * height / 4]) - 128;
        chrPtr++;
        int R, G, B;
        const int deltaB = ((454 * Cb) >> 8);
        const int deltaG = ((88 * Cb + 183 * Cr) >> 8);
        const int deltaR = ((359 * Cr) >> 8);

        //generate first RGB components
        B = Y1 + deltaB;
        if (B < 0)
            B = 0;
        else if (B > 255)
            B = 255;

        G = Y1 - deltaG;
        if (G < 0)
            G = 0;
        else if (G > 255)
            G = 255;

        R = Y1 + deltaR;
        if (R < 0)
            R = 0;
        else if (R > 255)
            R = 255;
        //NOTE: this assume little-endian encoding
        rgbs[outPtr++] = (byte) (((G & 0x3c) << 3) | (B >> 3));
        rgbs[outPtr++] = (byte) ((R & 0xf8) | (G >> 5));

        //generate second RGB components
        B = Y2 + deltaB;
        if (B < 0)
            B = 0;
        else if (B > 255)
            B = 255;

        G = Y2 - deltaG;
        if (G < 0)
            G = 0;
        else if (G > 255)
            G = 255;

        R = Y2 + deltaR;
        if (R < 0)
            R = 0;
        else if (R > 255)
            R = 255;
        //NOTE: this assume little-endian encoding
        rgbs[outPtr++] = (byte) (((G & 0x3c) << 3) | (B >> 3));
        rgbs[outPtr++] = (byte) ((R & 0xf8) | (G >> 5));
    }
    //LOGE("rgb size %d, w %d, h %d",outPtr,width, height);
}

void FrameStore_computeAvgDecodingTime(struct _FrameStore *this) {
    struct timeval now;
    gettimeofday(&now, NULL);

    unsigned long long timestampe = (now.tv_sec-this->start_decode_time.tv_sec)*1000000 + (now.tv_usec-this->start_decode_time.tv_usec);
    this->tempDecodingTime += timestampe/1000LL;
}

float FrameStore_getAvgDecodingTime(struct _FrameStore *this) {
    return this->tempDecodingTime/(float)(this->displayedFrames);
}

void FrameStore_setStartTime (struct _FrameStore *this) {
    gettimeofday(&(this->start_decode_time), NULL);
}

void FrameStore_setDecodingDelay (struct _FrameStore *this) {
    this->displayedFrames += bitmapFilledCounter;
    this->hasStarTime = 0;

    LOGI("[SetDecodingDelay]:%d - avg decoding Time=%lld, frames=%d", __LINE__, this->tempDecodingTime, this->displayedFrames);

    if (!this->decodingTimeMS) {
        LOGI("[SetDecodingDelay] - Time=%lld, Frame=%d", this->tempDecodingTime, bitmapFilledCounter);
        this->decodingTimeMS = this->tempDecodingTime*16/bitmapFilledCounter;
    }
}

//---- Alloc YUV ----//
void FrameStore_frameInit(struct _FrameStore* this, EachFrame* newFrame, int i) {
    LOGI("[FrameInit] - allocated Frame[%d] by thread_%d", i, this->threadNo);

    if(!newFrame->RGB) newFrame->RGB = (unsigned char*) malloc(this->frameWidth * this->frameHeight * 2 + 64);
    newFrame->alloc_size = this->frameWidth*this->frameHeight*2+64;

    if (!newFrame->RGB ) {
        // LOGE("FC : w %d, h %d in FrameStore_frameInit()", this->frameWidth, this->frameHeight);
        LOGE("[FrameInit] - malloc failed");
        exit(-1);
    }
}

//---- Display all GOP ----//
void FrameStore_Display(struct _FrameStore* this) {
    int i;
    // start at the displayCounter
    // LOGE("TESTDISPLAY : displayCounter = %d, FrameConter = %d", this->displayCounter, this->frameCounter);

    if(this->frameCounter > 16) this->displayCounter = this->frameCounter - 16;

    LOGI("[Display] - From %d to %d", this->displayCounter, this->frameCounter-1);
    for (i = this->displayCounter; i < this->frameCounter; ++i) {
        // LOGE("[Display] - memcpy(pixels[%d], this->frameList[%d].RGB, %d) by Thread_%d", bitmapFilledCounter, i, this->frameWidth * this->frameHeight * 2, this->threadNo);
        memcpy(pixels[bitmapFilledCounter++], this->frameList[i].RGB, this->frameWidth * this->frameHeight * 2);
        // free(this->frameList[i].RGB);
    }

    // reset all counter
    // this->frameAllocated = 0;
    this->frameCounter   = 0;
    this->displayCounter = 0;
}

/*
 * This function is used to display only one frame
 * It avoid to wait a long time until whole GOP is already decoded
 * Thus, it can be played at the same time while decoding at low speed
 */
void FrameStore_DisplaySingle(struct _FrameStore* this) {
    // depreciated
    /*
    //check exist frame to display
    if(this->displayCounter==init_nal_structthis->frameCounter)
    return;
    //send to display

    //free memory after display
    free(this->frameList[this->displayCounter].Y);
    //free(this->frameList[this->displayCounter].U);
    //free(this->frameList[this->displayCounter].V);
    ++this->displayCounter;
    */
}

//---- Insert YUV data to Frame Store ----//
int FrameStore_fillFrame(struct _FrameStore* this, unsigned char* Y, unsigned char* U, unsigned char* V) {
    /*
     * alloc if all frame element are used
     * no use because now I always free frames after displayed
     * it will be used to avoid much alloc in the future
     */

     // if (this->frameCounter == 16) {
     //    return this->frameCounter;
     // }

    // LOGE("FC : frameCounter = %d, FrameAllocated = %d in FrameStore_fillFrame()", this->frameCounter, this->frameAllocated);
    if (this->frameCounter == this->frameAllocated) {
        // LOGE("FC : w %d, h %d", this->frameWidth, this->frameHeight);
        this->frameInit(this, &(this->frameList[this->frameAllocated]), this->frameAllocated);
        ++this->frameAllocated;
    }

    // LOGI("[FillFrame] - convert to RGB565 from Frame[%d] by thread_%d", this->frameCounter, this->threadNo);
    toRGB565(Y, this->frameWidth + 32, this->frameHeight, (byte*) this->frameList[this->frameCounter].RGB, this->frameWidth + 32, this->frameHeight);

    ++this->frameCounter;
    return 0;
    // LOGE("FC : frameCounter = %d", this->frameCounter);
}

//---- Flush all frames ----//
void FrameStore_frameDestory(struct _FrameStore* this) {
    /*
     * alloc if all frame element are used
     * no use because now I always free frames after displayed
     * it will be used to avoid much alloc in the future
     */
    int i;
    for (i = 0; i < this->frameAllocated; ++i) {
        free((byte*) this->frameList[i].RGB);
        this->frameList[i].RGB = NULL;
    }
    this->frameCounter   = 0;
    this->frameAllocated = 0;
    // LOGE("finish destory");
}

//---- Check there is no frame in Frame Store ----//
int FrameStore_isEmpty(struct _FrameStore* this) {
    if (this->frameCounter > 0)
        return 0;
    return 1;
}

void FrameStore_checkResolution(struct _FrameStore* this) {
    // LOGE("TESTDISPLAY : this w=%d, h=%d | info w=%d, h=%d in FrameStore_checkResolution()", this->frameWidth, this->frameHeight, info.width, info.height);
    if (this->frameWidth != info.width || this->frameHeight != info.height)
        flagError = 1;
}

// It's constructor like C++
void FrameStoreConstructor(struct _FrameStore* this) {
    this->decodingTimeMS   = 0;
    this->displayCounter   = 0;
    this->edge             = 16;
    this->fps              = 20;
    this->frameAllocated   = 0;
    this->frameCounter     = 0;
    this->frameHeight      = 0;
    this->frameWidth       = 0;
    this->IDRindex         = 0;
    this->tempDecodingTime = 0;
    this->waitForDisplay   = 0;
    this->displayedFrames  = 0;
    this->hasStarTime      = 0;
    
    this->checkResolution        = FrameStore_checkResolution;
    this->Display                = FrameStore_Display;
    this->DisplaySingle          = FrameStore_DisplaySingle;
    this->fillFrame              = FrameStore_fillFrame;
    this->frameDestory           = FrameStore_frameDestory;
    this->frameInit              = FrameStore_frameInit;
    this->isEmpty                = FrameStore_isEmpty;

    this->computeAvgDecodingTime = FrameStore_computeAvgDecodingTime;
    this->getAvgDecodingTime     = FrameStore_getAvgDecodingTime;
    this->setStartTime           = FrameStore_setStartTime;
    this->setDecodingDelay       = FrameStore_setDecodingDelay;

    int i;
    for (i = 0; i < 100; i++) {
        this->frameList[i].RGB        = NULL;
        this->frameList[i].alloc_size = 0;
    }
}

void FrameStoreDeConstructor(struct _FrameStore* this) {
    for (; --this->frameAllocated >= 0;) {
        free(this->frameList[this->frameAllocated].RGB);
    }
}

int retrieveFrame(int threadID) {
    if (SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0] == 1 || SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0] == 2) {

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

        FrameStoreList[threadID].frameWidth = *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o;
        FrameStoreList[threadID].frameHeight = *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o;
        // LOGE("[retrieveFrame] - fillFrame in retrieveFrame with thread[%d]", threadID);
        return FrameStoreList[threadID].fillFrame(&FrameStoreList[threadID], Y, U, V);
    }
}


/*
 * Get segments from HTTP server
 * each segment has eight GOPs
 */
void streamFromHttp (int displayTimes) {
    extern char *StreamBuffer;
    extern int  file_size;
    extern int  StreamBufferPos;
    extern int  currentBufferPos;

    char url[120];
    char file_name[30]; //---- The file_name format is : videoName_segmentNumber_layerNumber ----//

    int seqSize = 0;

    // LOGE("FILENAME : file name = " + *prefix);

    if (seqBuf == NULL) {
        seqBuf = (char*) calloc(20000000, sizeof(char));
        // LOGE("[streamFromHttp] - seqBuf Allocated");
    }

    /* hard code temporary, dump all 264 streams */

    //---- This is first time to download segment ----//
    if (displayTimes == 0) {
        file_size = MaxFileSize; // default = 10185706
        StreamBufferPos = 0;
        StreamBuffer = (char*) calloc(file_size, sizeof(char));

        //---- initial segment, only can request once ----//
        sprintf(file_name, "%s_h_%d.264", prefix, download_enhance);
    }
    else {
        //---- The 6th segment of talking_head video has problem, repeat 5th segment ----//
        // if ((displayTimes % RepeatSegments) == 6 && !strcmp(prefix, "talking_head")) {
        //  sprintf(file_name, "%s_5_%d.264", prefix, download_enhance);
        // }
        // else {
        //  sprintf(file_name, "%s_%d_%d.264", prefix, (displayTimes % RepeatSegments), download_enhance);
        // }

        sprintf(file_name, "%s_%d_%d.264", prefix, (displayTimes % RepeatSegments), download_enhance);

        if(download_enhance && !enh_IDRindex[download_enhance - 1]) {
            enh_IDRindex[download_enhance - 1] = displayTimes * 8;
        }
    }
    sprintf(url, "%s/%s/%s", base_url, prefix, file_name);
    GetHttpStream(url, seqBuf, &seqSize);
    LOGE("[streamFromHttp] - Request %s %dth segment, size is %d", file_name, displayTimes, seqSize);

    // LOGE("SYNC : free space = %d, file size = %d", file_size - data_size, seqSize);

    //---- If free space not enough to store downloaded segment ----//
    EnterCriticalSection(&StreamBufferMutex);
    if (file_size - data_size < seqSize) {
        // EnterCriticalSection(&StreamBufferMutex);
            WakeAllConditionVariable(&CanPlayCond); // wake up all decoding thread
            // LOGE("[streamFromHttp] - Wake up DecoderThreads due to free space is not enough");
        // LeaveCriticalSection(&StreamBufferMutex);

        // EnterCriticalSection(&DownloadMutex);
            // LOGE("[streamFromHttp] - RequestThread is Locked");
            WaitConditionVariableCS(&CanDownloadCond, &StreamBufferMutex); // wait for waking up
            // LOGE("[streamFromHttp] - RequestThread keeps going");
        // LeaveCriticalSection(&DownloadMutex);
    }
    LeaveCriticalSection(&StreamBufferMutex);

    //---- rewrite to begin ----//
    EnterCriticalSection(&StreamBufferMutex);
    if (StreamBufferPos + seqSize > file_size) {
        int first_part      = file_size - StreamBufferPos;
        int remainder_part  = seqSize - first_part;

        // LOGE("test_buffer : memcpy1(StreamBuffer[%d], seqBuf[0], %d) in streamFromHttp", StreamBufferPos, first_part);
        memcpy(&StreamBuffer[StreamBufferPos], seqBuf, first_part * sizeof(char));
        // LOGE("test_buffer : memcpy2(StreamBuffer[0], seqBuf[%d], %d) in streamFromHttp", first_part, remainder_part);
        memcpy(StreamBuffer, &seqBuf[first_part], remainder_part * sizeof(char));

        StreamBufferPos = (StreamBufferPos + seqSize) % file_size;
    }
    //---- write into buffer with normal case ----//
    else {
        // LOGE("test_buffer : memcpy(StreamBuffer[%d], seqBuf[0], %d) in streamFromHttp", StreamBufferPos, seqSize);
        memcpy(&StreamBuffer[StreamBufferPos], seqBuf, seqSize * sizeof(char));
        StreamBufferPos = (StreamBufferPos + seqSize) % file_size;
    }
    totalDataSize += seqSize;
    LeaveCriticalSection(&StreamBufferMutex);
    // LOGE("[streamFromHttp] - Copy segment data into StreamBuffer");

    //---- set available data size ----//
    // EnterCriticalSection(&DataSizeMutex);
    // if (StreamBufferPos >= currentBufferPos) {
    //     data_size = StreamBufferPos - currentBufferPos;
    // }
    // else {
    //     data_size = (file_size - 1) - (currentBufferPos - StreamBufferPos);
    // }
    // LeaveCriticalSection(&DataSizeMutex);
    // LOGE("[streamFromHttp] - Current available data size is %d", data_size);

    /* If available data size is too large (larger than StopDecodeSpace) */
    EnterCriticalSection(&StreamBufferMutex);
    data_size += seqSize;
    if (data_size >= StopDecodeSpace) {
        WakeAllConditionVariable(&CanPlayCond);
        // LOGE("[streamFromHttp] - Wake up DecoderTrequest due to available data too large");
    }
    LeaveCriticalSection(&StreamBufferMutex);

    if (!network_delay) {
        struct timeval tp;
        gettimeofday(&tp, NULL);
        network_delay = ((tp.tv_sec - time_start.tv_sec)*1000000) + ((tp.tv_usec - time_start.tv_usec));
        // LOGE("TIME : network delay: %ld", network_delay);
    }

    // LOGE("test_buffer : StreamBufferPos = %d, currentBufferPos = %d\n", StreamBufferPos, currentBufferPos);
}

void* getStreams() {
    gettimeofday(&time_start_download, NULL);
    for (displayTimes = 0; 1; displayTimes++) {
        streamFromHttp(displayTimes);
    }
}

void fileStreamInit() {
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

    // file_size = 20971520;    // 20971520 = 20MB , 1048576 = 1MB, default = 10185706
    // StreamBufferPos = 0;
    // StreamBuffer = (char*) malloc(file_size * sizeof(char));

    /** segment 0, only can get once **/
    // GetHttpStream("http://140.114.79.80/test_0.264", seqBuf, &seqSize);
    // memcpy(&StreamBuffer[StreamBufferPos], seqBuf, seqSize * sizeof(char));
    // StreamBufferPos += seqSize;
    // LOGE("TESTSEG : got test_0.264 with 0 segment");

    pthread_create(&request_thread, NULL, (void*)getStreams, NULL);

    // streamFromHttp();
}

void initialize(int threadID) {
    __android_log_print(ANDROID_LOG_ERROR, "test", "init: %lld", (long long) time(NULL));

    // FILE* yfile = fopen("/sdcard/f1.yuv","w+");

    int argc = 7;
    char **argv = (char **) malloc(sizeof(char *) * 7);
    argv[1] = "-h264";
    // argv[2] = FILE_PATH;
    argv[2] = "";
    // argv[2] = "/sdcard/doc-reality.264";
    // argv[2] = "/sdcard/video_3.264";
    // argv[2] = "/sdcard/doc-reality-short.264";
    argv[3] = "-layer";
    argv[4] = (char *) malloc(sizeof(char) * 5);
    sprintf(argv[4], "%d", LAYER_ID);
    argv[5] = "-tempId";
    argv[6] = (char *) malloc(sizeof(char) * 5);
    sprintf(argv[6], "%d", TEMPORAL_ID);

    init_svc_vectors(SVCinfoList[threadID].decoder_svc_Svc_Vectors_PC_Svc_Vectors);
    decode_init_vlc(SVCinfoList[threadID].decoder_svc_VlcTab_PC_o);
    vector_main_init(SVCinfoList[threadID].decoder_svc_slice_main_vector_PC_Main_vector_o);
    *SVCinfoList[threadID].ReadAU_ReadBytes = getMinBytes;
    readh264_init(argc, argv);    //comment out with stream buffer
    get_layer(argc, argv, SVCinfoList[threadID].GetNalBytesAu_StreamType);



    EnterCriticalSection(&StreamBufferMutex);
    if ((threadID == 0) && (data_size < getMinBytes)) {
    // if ((threadID == 0) && (displayTimes < THREAD_NUM)) {
        // LOGE("SYNC : pThread[%d] lock StreamBufferMutex in initialize", threadID);
        // EnterCriticalSection(&StreamBufferMutex);
        // {
            // LOGE("SYNC : pThread[%d] IN CR of StreamBufferMutex in initialize",threadID);
            // LOGE("SYNCSTOP : ----- pThread[%d] waiting to ParseFirstAU -----", threadID);
            // LOGE("SYNC : thread[%d] waiting ////// CanPlayCond at INIT", threadID);
            WaitConditionVariableCS(&CanPlayCond, &StreamBufferMutex);
            // LOGE("SYNC : thread[%d] keep go ****** CanPlayCond at INIT", threadID);
            // LOGE("SYNC : ***** pThread[%d] continue initialize *****", threadID);

            // ParseFirstAU(getMinBytes, SVCinfoList[threadID].ReadAU_DataFile_o, SVCinfoList[threadID].GetNalBytesAu_StreamType);
        // }
        // LeaveCriticalSection(&StreamBufferMutex);
        // LOGE("SYNC : pThread[%d] unlock StreamBufferMutex in initialize", threadID);
    }
    LeaveCriticalSection(&StreamBufferMutex);

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
    extern int file_size;
    extern int currentBufferPos;
    extern int StreamBufferPos;

    //---- Get my thread ID ----//
    int threadID = *(int*) argu;

    //---- Bear, play with stack size ----//
    // size_t mystacksize;
    // pthread_attr_getstacksize(&attr, &mystacksize);
    // LOGE("Thread %d: stack size = %li bytes\n", threadID, mystacksize);

    int isFinished = 0;

    //---- exit when already reach EOF and played ----//
    // if(SVCinfoList[threadID].EndOfStream && FrameStoreList[threadID].isEmpty(&FrameStoreList[threadID])==0)
    //  return NULL;

    for (; !isFinished; ) {

        // LOGE("SYNC :");
        // LOGE("SYNC : thread[%d] start decoding", threadID);

        if (SVCinfoList[threadID].EndOfStream)
            return NULL;

        //---- data size is not enough for decoding ----//
        EnterCriticalSection(&StreamBufferMutex);
        if (data_size < StopDecodeSpace) {
        // if(displayTimes < THREAD_NUM) {
            WakeConditionVariable(&CanDownloadCond); // wake up donwload thread
            LOGI("[Thread_Decoding]:%d - RequestThread wake up by DecoderThread[%d]", __LINE__, threadID);


            // LOGE("SYNC : ----- pThread[%d] waiting to download -----", threadID);
            // EnterCriticalSection(&StreamBufferMutex);
                // LOGE("SYNC : thread[%d] waiting ////// CanPlayCond at DECODE", threadID);
                LOGI("[Thread_Decoding]:%d - DecoderThread[%d] is locked", __LINE__, threadID);
                WaitConditionVariableCS(&CanPlayCond, &StreamBufferMutex); // waiting for download
                LOGI("[Thread_Decoding]:%d - DecoderThread[%d] keep going", __LINE__, threadID);
            // LeaveCriticalSection(&StreamBufferMutex);

            // choose_layer(SVCinfoList[threadID].GetNalBytesAu_StreamType);
            // SVCinfoList[threadID].GetNalBytesAu_StreamType -> SetLayer = 2;
            // SVCinfoList[threadID].GetNalBytesAu_StreamType -> TemporalCom = 0;
        }
        LeaveCriticalSection(&StreamBufferMutex);
        // LOGE("SYNC : ***** pThread[%d] continue decoding *****", threadID);

        EnterCriticalSection(&StreamBufferMutex);
            // LOGE("test_buffer : ReadByte[%d] = %d ", threadID, SVCinfoList[threadID].ReadAU_ReadBytes[0]);
            ReadAuH264(getMinBytes, SVCinfoList[threadID].ReadAU_DataFile_o,
                                    SVCinfoList[threadID].ReadAU_ReadBytes[0],
                                    SVCinfoList[threadID].ReadAU_pos_o);
            // LOGE("DATA : data size: %d in decoding", data_size);
            SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size[0] = GetNalBytesAuSVC(
                    SVCinfoList[threadID].ReadAU_DataFile_o,
                    SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                    SVCinfoList[threadID].ReadAU_ReadBytes,
                    SVCinfoList[threadID].GetNalBytesAu_StreamType,
                    SVCinfoList[threadID].GetNalBytesAu_NalStreamer,
                    SVCinfoList[threadID].GetNalBytesAu_SPS,
                    SVCinfoList[threadID].GetNalBytesAu_PPS);
        LeaveCriticalSection(&StreamBufferMutex);

        EnterCriticalSection(&DirtyMutex); // Bear
            NextNalDqId(SVCinfoList[threadID].GetNalBytesAu_StreamType, SVCinfoList[threadID].DqIdNextNal_Nal_o);

            NalUnitSVC(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                    SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io,
                    SVCinfoList[threadID].decoder_svc_NalUnit_NalRefIdc_io,
                    SVCinfoList[threadID].DqIdNextNal_Nal_o);

            init_int(SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay);
        LeaveCriticalSection(&DirtyMutex); // Bear

        if (SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0] == 11) {
            // LOGE("!!!!!!!!!!!!!!!!!!!!!!!!!! %d", threadID);
            SVCinfoList[threadID].EndOfStream = 1;
            // sem_post(&ReturnSem);
            return NULL;
        }

        // if (!setStartDecodeTime[threadID]) {
        //     gettimeofday(&start_decode_time[threadID], 0);
        //     setStartDecodeTime[threadID] = 1;
        //     if (firstDecodeThread < 0) firstDecodeThread = threadID;
        // }

        // FrameStoreList[threadID].setStartTime(&FrameStoreList[threadID]);

        //---- Type IDR-frame or End of stream ----//
        // LOGE("ITD : decoder_svc_NalUnit_NalUnitType_io[0] = %d, EndOfStream = %d", SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0], SVCinfoList[threadID].EndOfStream);
        if (SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0] == 5 || SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0] == 11 || SVCinfoList[threadID].EndOfStream) {

            //---- increase IDR-Num ----//
            SVCinfoList[threadID].iFrame++;

            //---- I have previous frames want to Display ----//
            // LOGE("ITD : isEmpty[%d] = %d", threadID, FrameStoreList[threadID].isEmpty(&FrameStoreList[threadID]));
            if (FrameStoreList[threadID].isEmpty(&FrameStoreList[threadID]) == 0) {
                //---- retrieve all treasure from buffer ----//
                if (!FrameStoreList[threadID].waitForDisplay) {
                    // LOGI("FC : thread %d, nw=%d nh=%d", threadID, *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o, *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o);
                    // EnterCriticalSection(&DirtyMutex);
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
                        //   break;
                        // }
                        retrieveFrame(threadID);
                    }
                    // LeaveCriticalSection(&DirtyMutex);
                    // LOGE("ITD : %d, after retrieveFrame", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0]);
                }

                // FC = frameCounter, CD = CurrentDisplay, IDR = IDRindex, L = LayerID, T = TemporalID
                LOGI("[Thread_Decoding]:%d- DecoderThread[%d] Ready To Display, FrameCounter=%d, CurrentDisplay=%d, IDR=%d, ENIDR[%d]=%d, L=%d and T=%d", __LINE__,
                        threadID,
                        FrameStoreList[threadID].frameCounter,
                        CurrentDisplay,
                        FrameStoreList[threadID].IDRindex,
                        download_enhance - 1,
                        download_enhance ? enh_IDRindex[download_enhance - 1] : 0,
                        SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer,
                        SVCinfoList[threadID].GetNalBytesAu_StreamType->TemporalId);

                //---- Critical region to access CurrentDisplay ----//
                // LOGE("deadlock : Thread[%d], before CR CurrentDisplayMutex", threadID);
                EnterCriticalSection(&CurrentDisplayMutex);
                    // LOGE("deadlock : Thread[%d], in CR CurrentDisplayMutex", threadID);

                    int isFinishDisplay = 0;

                    while(!isFinishDisplay) {

                        //---- if this IDRindex is my job and display speed is too slow, so skip this IDR. ----//
                        if( (CurrentDisplay < FrameStoreList[threadID].IDRindex) && (CurrentDisplay%THREAD_NUM == threadID) ) {

                            // LOGE("[Thread_Decoding] - DecoderThread[%d] skip CurrentDisplay %d, IDR is %d", threadID, CurrentDisplay, FrameStoreList[threadID].IDRindex);
                            // ++CurrentDisplay;
                            // LOGE("deadlock : Thread %d, skip IDR %d", threadID, CurrentDisplay);
                            // LOGE("deadlock : Thread[%d], before CR FrameBufferMutex to skip", threadID);
                            EnterCriticalSection(&FrameBufferMutex);
                                // LOGE("deadlock : Thread[%d], in CR FrameBufferMutex to skip", threadID);
                                // :D
                                // LOGE("deadlock : Thread[%d], Wake up every FrameBufferCond",threadID);
                                WakeAllConditionVariable(&FrameBufferCond);
                                LOGI("[Thread_Decoding]:%d - DecoderThread[%d] wake up JavaThread at skip case", __LINE__, threadID);
                            LeaveCriticalSection(&FrameBufferMutex);
                            // LOGE("deadlock : Thread[%d], EXIT CR FrameBufferMutex to skip", threadID);
                            // sem_post(&ReturnSem);
                        }

                        //---- normal situation ----//
                        else if( CurrentDisplay == FrameStoreList[threadID].IDRindex ) {
                            //check resource
                            FrameStoreList[threadID].checkResolution(&FrameStoreList[threadID]);

                            // LOGE("deadlock : Thread[%d], before CR FrameBufferMutex to normal", threadID);
                            EnterCriticalSection(&FrameBufferMutex);
                                // LOGE("[Thread_Decoding] - DecoderThread[%d] Enter CR with FrameBufferMutex in normal case", threadID);

                                if(!flagError){
                                    //display~~~
                                    LOGI("[Thread_Decoding]:%d - DecoderThread[%d] Display", __LINE__, threadID);
                                    FrameStoreList[threadID].Display(&FrameStoreList[threadID]);
                                    FrameStoreList[threadID].setDecodingDelay(&FrameStoreList[threadID]);

                                    // if (!setDecodeTime[threadID]) {
                                    //     gettimeofday(&end_decode_time[threadID], 0);
                                    //     setDecodeTime[threadID] = ((end_decode_time[threadID].tv_sec - start_decode_time[threadID].tv_sec)*1000) + ((end_decode_time[threadID].tv_usec - start_decode_time[threadID].tv_usec)/1000);
                                    //     // LOGE("TIME : decode first frame with thread[%d]: %d", threadID, setDecodeTime[threadID]);
                                    //     LOGI("[Thread_Decoding] - DecoderThread[%d] Set Decoding Time Done", threadID);
                                    // }
                                    // LOGE("Thread %d, display finished, displayCounter %d", threadID, FrameStoreList[threadID].displayCounter);
                                    isFinishDisplay = 1;
                                }else {
                                    // LOGE("TESTDISPLAY : ERROR~~%d", threadID);
                                }

                                // LOGE("deadlock : Thread[%d], Wake up every FrameBufferCond",threadID);

                                /* If frame Copy into Buffer, wake up JavaThread*/
                                WakeAllConditionVariable(&FrameBufferCond);
                                LOGI("[Thread_Decoding]:%d - DecoderThread[%d] wake up JavaThread at normal case", __LINE__, threadID);
                            LeaveCriticalSection(&FrameBufferMutex);
                            // LOGE("deadlock : Thread[%d], EXIT CR FrameBufferMutex to normal", threadID);

                            // LOGE("Thread %d, post", threadID);
                            //sem_post(&ReturnSem);
                        }

                        //---- block until next signal while I'm fast!! ----//
                        if(!isFinishDisplay) {
                            // LOGE("deadlock : Thread[%d], Wait CurrentDisplayCond", threadID);
                            LOGI("[Thread_Decoding]:%d - DecoderThread[%d] is locked due to Decoder too fast", __LINE__, threadID);
                            WaitConditionVariableCS(&CurrentDisplayCond, &CurrentDisplayMutex);
                            // LOGE("[Thread_Decoding] - DecoderThread[%d] is waking up", threadID);
                            // LOGE("SYNC : thread[%d] keep go ****** CurrentDisplayCond", threadID);
                            // LOGE("deadlock : Thread[%d], continue CurrentDisplayCond", threadID);
                        }

                        if(SVCinfoList[threadID].EndOfStream)
                            return NULL;
                    }
                LeaveCriticalSection(&CurrentDisplayMutex);
                // LOGE("deadlock : Thread[%d], EXIT CR CurrentDisplayMutex", threadID);
                // LOGE("deadlock : Thread[%d], leave access display, CurDisplay %d", threadID, CurrentDisplay);
            }

            //---- set IDRindex ----//
            if (SVCinfoList[threadID].iFrame != -1 && SVCinfoList[threadID].iFrame % THREAD_NUM == threadID) {
                FrameStoreList[threadID].IDRindex = SVCinfoList[threadID].iFrame;
                // LOGE("Thread %d, reset iFrame data to %d", threadID, FrameStoreList[threadID].IDRindex);
            }

            // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // test
            // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
            //  FrameStoreList[threadID].frameDestory(&FrameStoreList[threadID]);
            //  SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;
            // }
            // if (SVCinfoList[threadID].iFrame == 9) {
            //  LAYER_ID = 32;
            //  FrameStoreList[threadID].frameDestory(&FrameStoreList[threadID]);
            //  SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;
            // }
            // else if (SVCinfoList[threadID].iFrame == 12) {
            //  LAYER_ID = 16;
            //  FrameStoreList[threadID].frameDestory(&FrameStoreList[threadID]);
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
            //      FrameStoreList[threadID].frameDestory(&FrameStoreList[threadID]);
            //      SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;

            //      Layer_changed[threadID] = 0;
            //      enh_IDRindex[download_enhance - 1] = 0;
            //      LOGE("[Thread_Decoding] - DecoderThread[%d] changes layer to %d", threadID, LAYER_ID);
            //  }
            //  // Swithc to higher layer
            //  else if (download_enhance && enh_IDRindex[download_enhance - 1]) {
            //      if ( (SVCinfoList[threadID].iFrame >= enh_IDRindex[download_enhance - 1]) ) {
            //          FrameStoreList[threadID].frameDestory(&FrameStoreList[threadID]);
            //          SVCinfoList[threadID].GetNalBytesAu_StreamType->SetLayer = LAYER_ID;

            //          Layer_changed[threadID] = 0;
            //          LOGE("[Thread_Decoding] - DecoderThread[%d] changes layer to %d", threadID, LAYER_ID);
            //      }
            //  }
            // }

            // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // end test
            // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            //LOGE("Thread %d, leave IDR proc", threadID);
        } // End check decoder_svc_NalUnit_NalUnitType_io or EndOfStream

        // gettimeofday(&FrameStoreList[threadID].start_decode_time, 0);
        // ftime(&FrameStoreList[threadID].start_decode_time);

        FrameStoreList[threadID].setStartTime(&FrameStoreList[threadID]);

        //------------------------------------------------Nal Unit Type---------------------------------------------------------------
        switch (SVCinfoList[threadID].decoder_svc_NalUnit_NalUnitType_io[0]) {
        case 1: {   //non-IDR
            if (SVCinfoList[threadID].iFrame % THREAD_NUM != threadID)
                break;  // not my job
            // LOGE("Thread %d, case 1", threadID);
            *SVCinfoList[threadID].decoder_svc_Nal_Compute_NalDecodingProcess_Set_Pos_Pos = 8;
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
                case 0: {   //Exp-init_nal_structGolomb coded
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
                case 1: {   //CABAC
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
                case 0: {   //P slice
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

                case 1: {   //B slice
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

                case 2: {   //I slice
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
        case 5: {  // Indicates a IDR-frame!
            // LOGE("Thread %d, case 5", threadID);
            if (SVCinfoList[threadID].iFrame % THREAD_NUM != threadID)
                break;  // not my job

            svc_calculate_dpb((2186240 * (10 + 10 + 4 - 1)),
                    (1920 * 1088 / 8 * 4 * (10 + 1)), 4,
                    SVCinfoList[threadID].decoder_svc_Residu_Mmo,
                    SVCinfoList[threadID].decoder_svc_Residu_SPS,
                    SVCinfoList[threadID].DqIdNextNal_Nal_o);

            *SVCinfoList[threadID].decoder_svc_Nal_Compute_SetPos_Pos = 8;

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
            //I slice
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

        case 6: {   //Supplemental enhancement information
            // LOGE("Thread %d, case 6", threadID);
            EnterCriticalSection(&DirtyMutex); // Bear
                sei_rbsp(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                        SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size[0],
                        SVCinfoList[threadID].decoder_svc_Residu_SPS,
                        SVCinfoList[threadID].decoder_svc_Nal_Compute_sei_rbsp_Sei);
            LeaveCriticalSection(&DirtyMutex); // Bear
            break;
        }

        case 7: {   //Sequence parameter set
            // LOGE("Thread %d, case 7", threadID);
            EnterCriticalSection(&DirtyMutex); // Bear
                seq_parameter_set(SVCinfoList[threadID].GetNalBytesAu_rbsp_o, SVCinfoList[threadID].decoder_svc_Residu_SPS);
            LeaveCriticalSection(&DirtyMutex); // Bear
            break;
        }

        case 8: {   //Picture parameter set
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

        case 11: {  // Indicates the end of video..
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
        case 14: {  // Prefix NAL unit in scalable extension
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
        case 15: {  //Subset sequence parameter set
            // LOGE("Thread %d, case 15", threadID);
            EnterCriticalSection(&DirtyMutex); // Bear
                subset_sps(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                        SVCinfoList[threadID].GetNalBytesAu_rbsp_o_size,
                        SVCinfoList[threadID].decoder_svc_Residu_SPS,
                        SVCinfoList[threadID].DqIdNextNal_Nal_o);
            LeaveCriticalSection(&DirtyMutex); // Bear
            break;
        }
        case 20: {  //Coded slice in scalable extension
            // LOGE("Thread %d, case 20", threadID);
            if (SVCinfoList[threadID].iFrame % THREAD_NUM != threadID)
                break;  // not my job
            NalUnitHeader(SVCinfoList[threadID].GetNalBytesAu_rbsp_o,
                    SVCinfoList[threadID].decoder_svc_Nal_Compute_nal_unit_header_svc_ext_20_pos_o,
                    SVCinfoList[threadID].DqIdNextNal_Nal_o,
                    SVCinfoList[threadID].decoder_svc_VideoParameter_EndOfSlice);

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
                // LOGE("Thread %d, case 20.0", threadID);
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
                // LOGE("Thread %d, case 20.1", threadID);
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
                // LOGE("Thread %d, case 20.2", threadID);
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
                // LOGE("Thread %d, case 20.3", threadID);
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
                // LOGE("Thread %d, case 20.4", threadID);
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
            // EnterCriticalSection(&DirtyMutex);   // Bear
            // LOGE("FC : ImgToDisplay = %d, before switch with thread %d", SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0], threadID);
            switch (SVCinfoList[threadID].decoder_svc_VideoParameter_ImgToDisplay[0]) {
            case 1: {
                EnterCriticalSection(&DirtyMutex); // Bear
                {
                    // LOGE("Thread %d, case 20.6", threadID);
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

                    // LOGE("[Thread_Decoding] - fillFrame in case 20.6 with thread[%d]", threadID);
                    FrameStoreList[threadID].fillFrame(&FrameStoreList[threadID], Y, U, V);
                    // LOGE("end fill");
                }
                LeaveCriticalSection(&DirtyMutex);  // Bear
                break;
            }
            case 2: {
                EnterCriticalSection(&DirtyMutex);  // Bear
                {
                    // LOGE("Thread %d, case 20.7", threadID);
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
                    FrameStoreList[threadID].frameWidth = *SVCinfoList[threadID].decoder_svc_VideoParameter_xsize_o;
                    FrameStoreList[threadID].frameHeight = *SVCinfoList[threadID].decoder_svc_VideoParameter_ysize_o;
                    // LOGE("[Thread_Decoding] - fillFrame in case 20.7 with thread[%d]", threadID);
                    FrameStoreList[threadID].fillFrame(&FrameStoreList[threadID], Y, U, V);
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

        FrameStoreList[threadID].computeAvgDecodingTime(&FrameStoreList[threadID]);
        // gettimeofday(&FrameStoreList[threadID].end_decode_time, 0);
        // ftime(&FrameStoreList[threadID].end_decode_time);
        // FrameStoreList[threadID].tempDecodingTime += (FrameStoreList[threadID].end_decode_time.tv_sec-FrameStoreList[threadID].start_decode_time.tv_sec)*1000LL + (FrameStoreList[threadID].end_decode_time.tv_usec-FrameStoreList[threadID].start_decode_time.tv_sec)/1000;
        // FrameStoreList[threadID].tempDecodingTime += (FrameStoreList[threadID].end_decode_time.time - FrameStoreList[threadID].start_decode_time.time)*1000LL + (FrameStoreList[threadID].end_decode_time.millitm - FrameStoreList[threadID].start_decode_time.millitm);
        // LOGI("[Thread_Decoding]:%d - tempDecodingTime=%lld", __LINE__, FrameStoreList[threadID].tempDecodingTime);
        // gettimeofday(&FrameStoreList[threadID].start_decode_time, 0);
        // ftime(&FrameStoreList[threadID].start_decode_time);

        // LOGE("Thread %d, Finished one packet", threadID);
    } // End main for loopl

    // LOGE("Thread %d, Finish ALL", threadID);
    return NULL;
}

JNIEXPORT jint JNICALL
Java_test_svc_Main_nativeGetWidth(JNIEnv * env, jobject obj) {
    if (init)
        return FrameStoreList[CurrentDisplay % THREAD_NUM].frameWidth;
    return 0;
}

JNIEXPORT jint JNICALL
Java_test_svc_Main_nativeGetHeight(JNIEnv * env, jobject obj) {
    if (init)
        return FrameStoreList[CurrentDisplay % THREAD_NUM].frameHeight;
    return 0;
}

JNIEXPORT jint JNICALL
Java_test_svc_Main_nativeGetLayerID(JNIEnv * env, jobject obj) {
    return LAYER_ID;
}

JNIEXPORT jint JNICALL
Java_test_svc_Main_nativeGetTemporalID(JNIEnv * env, jobject obj) {
    return TEMPORAL_ID;
}

JNIEXPORT jint JNICALL
Java_test_svc_Main_nativeGetNetworkDelay(JNIEnv * env, jobject obj) {
    return (network_delay/1000);
}

JNIEXPORT jint JNICALL
Java_test_svc_Main_nativeGetSwitchGOP(JNIEnv * env, jobject obj) {
    return (download_enhance ? enh_IDRindex[download_enhance - 1] : -1);
}

JNIEXPORT jlong JNICALL
Java_test_svc_Main_nativeGetDecodeDelay(JNIEnv * env, jobject obj) {
    int i, counter = 0;
    long long totalTime = 0;
    for (i = 0; i < THREAD_NUM; i++) {
        if (FrameStoreList[i].decodingTimeMS) {
            counter++;
            totalTime += FrameStoreList[i].decodingTimeMS;
        }
    }

    return totalTime/counter;
    // return (setDecodeTime[firstDecodeThread]);
}

JNIEXPORT jfloat JNICALL
Java_test_svc_Main_nativeGetThroughput(JNIEnv * env, jobject obj) {
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    float result = 0.0;

    if (totalDataSize) {
        float now_in_ms = end_time.tv_sec*1000 + end_time.tv_usec/1000.0;
        float start_in_ms = time_start_download.tv_sec*1000 + time_start_download.tv_usec/1000.0;
        float diff_in_ms = now_in_ms - start_in_ms;
        float dataMb = totalDataSize*8/1048567.0;
        result = dataMb*1000 / diff_in_ms;
    }

    totalDataSize = 0.0;
    time_start_download = end_time;

    return result;
}

JNIEXPORT jfloat JNICALL
Java_test_svc_Main_nativeGetAvgDecodingTime(JNIEnv * env, jobject obj) {
    int i;
    float totalTime = 0.0;
    for (i = 0; i < THREAD_NUM; i++) {
        totalTime += FrameStoreList[i].getAvgDecodingTime(&FrameStoreList[i]);
    }

    return totalTime/THREAD_NUM;
}

JNIEXPORT void JNICALL
Java_test_svc_Main_nativeSetFilePath(JNIEnv * env, jobject obj, jstring BaseUrl, jstring Prefix) {
    base_url    = (char*)(*env)->GetStringUTFChars(env, BaseUrl, NULL);
    prefix      = (char*)(*env)->GetStringUTFChars(env, Prefix, NULL);
    fileStreamInit();
}

JNIEXPORT void JNICALL
Java_test_svc_Main_nativeSetLayerID(JNIEnv * env, jobject obj, jint layerID) {
    int i;
    LAYER_ID = layerID;

    pre_download_enhance = download_enhance;
    switch (LAYER_ID) {
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
            download_enhance = 0;
            break;
    }
    // if (LAYER_ID == 16) {
    //  download_enhance = 0;
    //  //      StopDownloadSpace =  3145728; // 3MB
    //  //      MaxFileSize = 5242880; // 5MB
    // }
    // else if (LAYER_ID == 32) {
    //  download_enhance = 1;
    //  //      StopDownloadSpace =  10485760; // 10MB
    //  //      MaxFileSize = 15728640; // 15MB
    // }
    // else if (LAYER_ID == 33) {
    //  download_enhance = 2;
    //  //      StopDownloadSpace =  31457280; // 30MB
    //  //      MaxFileSize = 47185920; // 40MB
    // }

    for (i = 0; i < THREAD_NUM; i++) {
        Layer_changed[i] = 1;
    }

    enh_IDRindex[0] = enh_IDRindex[1] = 0;
}

JNIEXPORT void JNICALL
Java_test_svc_Main_nativeSetTemporalID(JNIEnv * env, jobject obj, jint temporalID) {
    int i;

    TEMPORAL_ID = temporalID;
    for (i = 0; i < THREAD_NUM; i++) {
        Temporal_changed[i] = 1;
    }
}

JNIEXPORT void JNICALL
Java_test_svc_Main_nativeSetThreadNum(JNIEnv * env, jobject obj, jint threadNum) {
    THREAD_NUM = threadNum;
    LOGI("set %d thread(s)", THREAD_NUM);

    Temporal_changed        = (int*) calloc(THREAD_NUM, sizeof(int));
    if (!Temporal_changed) LOGE("[nativeSetThreadNum]:%d - allocate 'Temporal_changed' memory failed.", __LINE__);

    Layer_changed           = (int*) calloc(THREAD_NUM, sizeof(int));
    if (!Layer_changed) LOGE("[nativeSetThreadNum]:%d - allocate 'Layer_changed' memory failed.", __LINE__);

    pre_download_enhance    = download_enhance  = 0;
    enh_IDRindex[0]         = enh_IDRindex[1]   = 0;
}

JNIEXPORT void JNICALL
Java_test_svc_Main_nativeSetStartTime(JNIEnv * env, jobject obj) {
    gettimeofday(&time_start, NULL);
}

JNIEXPORT void JNICALL
Java_test_svc_Main_nativeClear(JNIEnv * env, jobject obj) {
    LOGE("clear ~");
    EnterCriticalSection(&DirtyMutex);
        if(init == 0) {
            LeaveCriticalSection(&DirtyMutex);
            return;
        }

        free(FrameStoreList);
        free(SVCinfoList);
        free(ThreadList);
        free(Temporal_changed);
        init = 0;
        LOGE("clear done");
    LeaveCriticalSection(&DirtyMutex);
}

JNIEXPORT jint JNICALL
Java_test_svc_Main_nativeDecodeSVC(JNIEnv * env, jobject obj, jobjectArray bitmapArray, jint bitmapNum) {
    // LOGE("***Start fill");
    // AndroidBitmapInfo info;
    // void* pixels;
    int ret;
    int i, res_and;

    jobject bitmap[64];
    bitmapFilledMax = bitmapNum;

    for (i = 0; i < bitmapNum; ++i) {
        bitmap[i] = (jobject) (*env)->GetObjectArrayElement(env, bitmapArray, i);
        if ((ret = AndroidBitmap_getInfo(env, bitmap[i], &info)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return 0;
        }

        if (info.format != ANDROID_BITMAP_FORMAT_RGB_565) {
            LOGE("Bitmap format is not RGB_565 !");
            return 0;
        }

        EnterCriticalSection(&DirtyMutex); // Bear
        if ((ret = AndroidBitmap_lockPixels(env, bitmap[i], &pixels[i])) < 0) {
            LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        }
        LeaveCriticalSection(&DirtyMutex); // Bear
    }

    // __android_log_print(ANDROID_LOG_ERROR,"In","? %d: %d", i, clock());
    if (init == 0) {
        // monstartup("libsvc.so");
        if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM && (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0) {
            /* neon */
            // __android_log_print(ANDROID_LOG_ERROR, "Cpu Info", "with NEON");
        } else {
            /* without neon */
            // __android_log_print(ANDROID_LOG_ERROR, "Cpu Info", "without NEON");
        }

        CurrentDisplay = -1;
        init = 1;

        /* init thread data */
        ThreadList      = (pthread_t*)  calloc(THREAD_NUM, sizeof(pthread_t));
        if (!ThreadList) LOGE("[nativeDecodeSVC]:%d - allocated 'ThreadList' memory failed.", __LINE__);

        threadIDs       = (int *) calloc(THREAD_NUM, sizeof(int));
        if (!threadIDs) LOGE("[nativeDecodeSVC]:%d - allocated 'threadIDs' memory failed.", __LINE__);

        SVCinfoList     = (SVCinfo*)    calloc(THREAD_NUM, sizeof(SVCinfo));
        if (!SVCinfoList) LOGE("[nativeDecodeSVC]:%d - allocated 'SVCinfoList' memory failed.", __LINE__);

        FrameStoreList  = (FrameStore*) calloc(THREAD_NUM, sizeof(FrameStore));
        if (!FrameStoreList) LOGE("[nativeDecodeSVC]:%d - allocated 'FrameStoreList' memory failed.", __LINE__);

        // time_start = time(NULL);
        framecounter = 0;
        flagError = 0;
        // Bear, increase stack size
        size_t stacksize;
        pthread_attr_init(&attr);
        pthread_attr_getstacksize(&attr, &stacksize);
        // __android_log_print(ANDROID_LOG_ERROR, "svc", "Default stack size = %li\n", stacksize);
        stacksize = 1048576;  // 8MB?
        // __android_log_print(ANDROID_LOG_ERROR, "svc", "Amount of stack needed per thread = %li\n", stacksize);
        pthread_attr_setstacksize(&attr, stacksize);

        // sem_init(&ReturnSem, 0, 0);

        // start_decode_time   = (struct timeval*) calloc(THREAD_NUM, sizeof(struct timeval));
        // end_decode_time     = (struct timeval*) calloc(THREAD_NUM, sizeof(struct timeval));
        // setDecodeTime       = (int*)            calloc(THREAD_NUM, sizeof(int));
        // setStartDecodeTime  = (int*)            calloc(THREAD_NUM, sizeof(int));

        int rc = 0;
        for (i = 0; i < THREAD_NUM; ++i) {
            FrameStoreConstructor(&FrameStoreList[i]);
            threadIDs[i] = i;
            FrameStoreList[i].threadNo = i;
            // setDecodeTime[i] = 0;
            // setStartDecodeTime[i] = 0;
            initialize(i);

            rc = pthread_create(&ThreadList[i], &attr, Thread_Decoding, (void*) &threadIDs[i]);
            // if (rc) LOGE("[Java_test_svc_Main_nativeDecodeSVC] - pthread_create() error code = %d", rc);
            // LOGE("SYNC : @@@@ pThread[%d] created @@@@", i);
        }
        // LOGE("Finish log");
    }

    // if(CurrentDisplay == 300)
    // moncleanup();

    bitmapFilledCounter = 0;


    // sem_wait(&ReturnSem);
    while(1) {
        EnterCriticalSection(&FrameBufferMutex);
        if(bitmapFilledCounter > 0) {
            LeaveCriticalSection(&FrameBufferMutex);
            break;
        }
        LeaveCriticalSection(&FrameBufferMutex);

        EnterCriticalSection(&CurrentDisplayMutex);
        if(flagError == 0) {
            // EnterCriticalSection(&CurrentDisplayMutex);
            CurrentDisplay++;
            // LeaveCriticalSection(&CurrentDisplayMutex);
        }
        else {
            // LOGE("Recovery");
            flagError = 0;
        }
        LeaveCriticalSection(&CurrentDisplayMutex);

        EnterCriticalSection(&FrameBufferMutex);
            //---- Wake up DecoderThreads to copy frames into Buffer ----//
            EnterCriticalSection(&CurrentDisplayMutex);
                WakeAllConditionVariable(&CurrentDisplayCond);
            LeaveCriticalSection(&CurrentDisplayMutex);

            // ---- If Decoded Frame wrote into Buffer, keep going ----//
            WaitConditionVariableCS(&FrameBufferCond, &FrameBufferMutex);
        LeaveCriticalSection(&FrameBufferMutex);
        
        if(flagError) break;
    }

    // LOGE("***End fill, bitmapFilledCounter %d", bitmapFilledCounter);

    if(flagError) {
        // error recovery
        return -1;
    }

    return bitmapFilledCounter;
}