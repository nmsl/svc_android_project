#ifndef H_FRAME_STORE_H
#define H_FRAME_STORE_H

#include "SDLEventHandler.h"

// static CRITICAL_SECTION memoryMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
	int alloc_size;
	Uint8 *RGB;
} EachFrame;

typedef struct {
	int gop_no;
	int width, height;
	int gop_frames;
	int frame_index;
	EachFrame frame;
} SDLFrame;

// typedef struct _SDLDisplayInfo {
// 	int current_frame_index;
// 	int start_index;
// 	int gop_frames;
// 	int width, height;
// 	// EachFrame FrameData[100];
// } SDLDisplayInfo;

//---- C ----//
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
	int cur_sdl_frame_index;
	EachFrame frameList[50]; // hard code it, I don't want to realloc it, there is no vector in C...
	// SDLDisplayInfo displayInfo;
	SDLFrame sdlList[50];

	// Information for SDL display
	int start_frame_index;
	int frames2display;
	int displayWidth, displayHeight;
	// int GOP_done;
	int ThreadID;

	// Measurement of decoding delay
	uint32_t start_decoding_time;
	uint32_t decode_time_in_ms;
	uint32_t final_decoding_time;
	void  (*SetStartDecodingTime)(struct _FrameStore *thz);
	void  (*ComputeDecodeTime)(struct _FrameStore* thz);
	uint32_t (*GetDecodingDelay)(struct _FrameStore* thz);

	// For switching event
	int cur_TID;
	int cur_SID;

	// do what member function doing in C++...
	void (*FrameInit)(struct _FrameStore* thz, EachFrame* newFrame, int i);
	void (*FrameDestory)(struct _FrameStore* thz);
	void (*Display)(struct _FrameStore* thz, int gop_no);
	void (*DisplaySingle)(struct _FrameStore* thz);
	int  (*FillFrame)(struct _FrameStore* thz, Uint8 *Y, Uint8 *U, Uint8 *V);
	// void (*FillFrame)(struct _FrameStore* thz, Uint8 *Y, Uint8 *U, Uint8 *V);
	int  (*IsEmpty)(struct _FrameStore* thz);
	void (*CheckResolution)(struct _FrameStore* thz);
} FrameStore;

void FrameStoreConstructor(FrameStore *thiz);
void FrameStoreDeConstructor(FrameStore *thiz);
void FrameStore_frameInit(FrameStore *thiz, EachFrame* newFrame, int i);
void FrameStore_Display(FrameStore *thiz, int gop_no);
void FrameStore_DisplaySingle(FrameStore *thiz);
int FrameStore_fillFrame(FrameStore *thiz, Uint8 *Y, Uint8 *U, Uint8 *V);
// void FrameStore_fillFrame(FrameStore *thiz, Uint8 *Y, Uint8 *U, Uint8 *V);
void FrameStore_frameDestory(FrameStore *thiz);
int  FrameStore_isEmpty(FrameStore *thiz);
void FrameStore_checkResolution(FrameStore *thiz);
// Compute Decoding Delay
void FrameStore_setStartDecodingTime(FrameStore *thiz);
void FrameStore_computeDecodeTime(FrameStore *thiz);
uint32_t FrameStore_getDecodingDelay(FrameStore *thiz);

void toRGB565(Uint8 yuvs[], int width, int height, Uint8 rgbs[], int size);
// void toRGB24(Uint8 src[], int width, int height, Uint8 out[]);

#endif /* H_FRAME_STORE_H */
