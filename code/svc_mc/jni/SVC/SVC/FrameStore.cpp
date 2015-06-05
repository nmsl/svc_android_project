#include <stdlib.h>
#include <stdio.h>

#include "FrameStore.h"

#include <android/log.h>
#define  LOG_TAG    __FILE__
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,   LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,    LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,    LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,   LOG_TAG, __VA_ARGS__)

int MAX_FRAME_SIZE = 1280 * 720 * 2 + 64;
// int MAX_FRAME_SIZE = 1280 * 720 * 3 / 2;

//---- C ----//
void FrameStoreConstructor(FrameStore *thiz) {
    thiz->frameAllocated      = 0;
    thiz->frameCounter        = 0;
    thiz->displayCounter      = 0;
    thiz->edge                = 16;
    thiz->frameWidth          = 0;
    thiz->frameHeight         = 0;
    thiz->fps                 = 20;
    thiz->IDRindex            = 0;
    thiz->waitForDisplay      = 0;
    thiz->cur_sdl_frame_index = 0;

    // Information for SDL display
    thiz->start_frame_index = 0;
    thiz->frames2display    = 0;
    // thiz->GOP_done          = 0;
    thiz->displayWidth  = 0;
    thiz->displayHeight = 0;
    thiz->ThreadID      = 0;
    
    // Measurement of decoding delay
    thiz->start_decoding_time = 0;
    thiz->decode_time_in_ms   = 0;
    thiz->final_decoding_time = 0;

    thiz->FrameInit            = FrameStore_frameInit;
    thiz->FrameDestory         = FrameStore_frameDestory;
    thiz->Display              = FrameStore_Display;
    thiz->DisplaySingle        = FrameStore_DisplaySingle;
    thiz->FillFrame            = FrameStore_fillFrame;
    thiz->IsEmpty              = FrameStore_isEmpty;
    thiz->CheckResolution      = FrameStore_checkResolution;

    // Measurement functions
    thiz->SetStartDecodingTime   = FrameStore_setStartDecodingTime;
    thiz->ComputeDecodeTime      = FrameStore_computeDecodeTime;
    thiz->GetDecodingDelay       = FrameStore_getDecodingDelay;

    int i;
    for (i = 0; i < 50; i++) {
        thiz->frameList[i].RGB        =  NULL;
        thiz->frameList[i].alloc_size = 0;

        thiz->sdlList[i].frame.RGB        = NULL;
        thiz->sdlList[i].frame.alloc_size = 0;
    }

    // for(i = 0; i < 20; i++) {
    //  thiz->sdlList[i].frame.RGB        = NULL;
    //  thiz->sdlList[i].frame.alloc_size = 0;  
    // }
}
void FrameStoreDeConstructor(FrameStore *thiz) {
    for (; --thiz->frameAllocated >= 0;) {
        if (thiz->frameList[thiz->frameAllocated].RGB) {
         free(thiz->frameList[thiz->frameAllocated].RGB);
         thiz->frameList[thiz->frameAllocated].RGB = NULL;
         thiz->frameList[thiz->frameAllocated].alloc_size = 0;
        }
    }
}

//---- Alloc YUV ----//
void FrameStore_frameInit(FrameStore *thiz, EachFrame *newFrame, int i) {
    newFrame->alloc_size = sizeof(Uint8) * thiz->frameWidth * thiz->frameHeight * 2 + 64;
    if (!newFrame->RGB) {
        newFrame->RGB = (Uint8*) calloc(1, MAX_FRAME_SIZE);
    }
}

//---- Display all frames from GOP ----//
void FrameStore_Display(FrameStore *thiz, int gop_no) {
    // LOGE("[FrameStore_Display] : displayCounter = %d, FrameConter = %d", thiz->displayCounter, thiz->frameCounter);

    if(thiz->frameCounter > 16) thiz->displayCounter = thiz->frameCounter - 16;

    // start at the displayCounter
    for (int i = thiz->displayCounter; i < thiz->frameCounter; ++i) {
        SDLFrame *sdl_frame = &(thiz->sdlList[i]);

        sdl_frame->gop_no      = gop_no;
        sdl_frame->width       = thiz->displayWidth;
        sdl_frame->height      = thiz->displayHeight;
        sdl_frame->gop_frames  = thiz->frameCounter - thiz->displayCounter;

        if (!sdl_frame->frame.RGB) sdl_frame->frame.RGB = (Uint8*) calloc(1, MAX_FRAME_SIZE);
        memcpy(sdl_frame->frame.RGB, thiz->frameList[i].RGB, sdl_frame->width * sdl_frame->height * 2);

        // sdl_frame->frame.RGB = thiz->frameList[i].RGB;

        SDL_Event event;
        SDL_zero(event);
        event.type       = DISPLAY_EVENT;
        event.user.data1 = sdl_frame;
        // event.user.data1 = info;
        // event.user.code  = i;
        SDL_PushEvent(&event);

        // thiz->cur_sdl_frame_index = (++thiz->cur_sdl_frame_index)%50;
    }

    thiz->frameCounter   = 0;
    thiz->displayCounter = 0;
    // thiz->waitForDisplay = 0;
}

/*
 * This function is used to display only one frame
 * It avoid to wait a long time until whole GOP is already decoded
 * Thus, it can be played at the same time while decoding at low speed
 */
void FrameStore_DisplaySingle(FrameStore *thiz) {
    // depreciated
    /*
    check exist frame to display
    if(thiz->displayCounter == init_nal_structthis->frameCounter)
    return;
    // send to display

    // free memory after display
    free(thiz->frameList[thiz->displayCounter].Y);
    // free(thiz->frameList[thiz->displayCounter].U);
    // free(thiz->frameList[thiz->displayCounter].V);
    ++thiz->displayCounter;
    */
}

//---- Insert YUV data to Frame Store ----//
int FrameStore_fillFrame(FrameStore *thiz, Uint8 *Y, Uint8 *U, Uint8 *V) {
// void FrameStore_fillFrame(FrameStore *thiz, Uint8 *Y, Uint8 *U, Uint8 *V) {
    // alloc if all frame element are used                      //
    // no use because now I always free frames after displayed  //
    // it will be used to avoid much alloc in the future        //

    // if (thiz->frameCounter == 16) {
    //     LOGW("[FrameStore_fillFrame] - Break");
    //     return thiz->frameCounter;
    // }

    if (thiz->frameCounter == thiz->frameAllocated) {
     thiz->FrameInit(thiz, &(thiz->frameList[thiz->frameCounter]), thiz->frameCounter);
     ++thiz->frameAllocated;
    }

    int correctFrameSize = thiz->frameWidth * thiz->frameHeight * 2 + 64;
    if (thiz->frameList[thiz->frameCounter].alloc_size != correctFrameSize) {
     memset((Uint8*) thiz->frameList[thiz->frameCounter].RGB, 0, correctFrameSize);
     thiz->frameList[thiz->frameCounter].alloc_size = correctFrameSize;
    }
    toRGB565(Y, thiz->frameWidth + 32, thiz->frameHeight, (Uint8*) thiz->frameList[thiz->frameCounter].RGB, correctFrameSize);

    ++thiz->frameCounter;
}

//---- Flush all frames ----//
void FrameStore_frameDestory(FrameStore *thiz) {
    // alloc if all frame element are used                      //
    // no use because now I always free frames after displayed  //
    // it will be used to avoid much alloc in the future        //
    // LOGI("[FrameStore_frameDestory] - Free RGB Buffer");

    for (int i = 0; i < thiz->frameAllocated; ++i) {
        if (thiz->frameList[i].RGB) free((Uint8*) thiz->frameList[i].RGB);        
        thiz->frameList[i].RGB        = NULL;
        thiz->frameList[i].alloc_size = 0;
    }

    thiz->frameCounter   = 0;
    thiz->frameAllocated = 0;
}

//---- Check there is no frame in Frame Store ----//
int FrameStore_isEmpty(FrameStore *thiz) {
    return thiz->frameCounter == 0; 
}

void FrameStore_checkResolution(FrameStore *thiz) {
    // LOGE("TESTDISPLAY : thiz w=%d, h=%d | info w=%d, h=%d in FrameStore_checkResolution()", thiz->frameWidth, thiz->frameHeight, info.width, info.height);

    int frameSize = thiz->frameWidth * thiz->frameHeight * 2 + 64;

    int width  = thiz->frameWidth;
    int height = thiz->frameHeight;

    if (width != thiz->displayWidth || height != thiz->displayHeight) {
        LOGI("[CheckResolution] - Resolution change from %dx%d to %dx%d", thiz->displayWidth, thiz->displayHeight, width, height);
        thiz->displayWidth  = width;
        thiz->displayHeight = height;
    }
}

void toRGB565(Uint8 yuvs[], int width, int height, Uint8 rgbs[], int size) {
    // the end of the luminance data
    const int lumEnd = width * height;
    // points to the next luminance value pair
    int lumPtr = 0;
    // points to the next chromiance value pair
    int chrPtr = lumEnd;
    // points to the next byte output pair of RGB565 value
    int outPtr = 0;
    // the end of the current luminance scanline
    int lineEnd = width;

    while (1) {
        // skip back to the start of the chromiance values when necessary
        if (lumPtr == lineEnd) {
            // LOGI("[toRGB565] - lumPtr=%d, chrPtr=%d, outPtr=%d, MaxSize=%d", lumPtr, chrPtr, outPtr, size);
            if (lumPtr == lumEnd) break; // we've reached the end
            // division here is a bit expensive, but's only done once per scanline
            chrPtr = lumEnd + ((lumPtr / width) >> 1) * (width >> 1);
            lineEnd += width;
            // to erase green bar
            outPtr -= 64;
        }

        // read the luminance and chromiance values
        const int Y1 = yuvs[lumPtr++];
        const int Y2 = yuvs[lumPtr++];
        const int Cr = (yuvs[chrPtr]) - 128;
        const int Cb = (yuvs[chrPtr + width * height / 4]) - 128;
        chrPtr++;

        int R, G, B;
        const int deltaB = ((454 * Cb) >> 8);
        const int deltaG = ((88 * Cb + 183 * Cr) >> 8);
        const int deltaR = ((359 * Cr) >> 8);

        // generate first RGB components
        B = Y1 + deltaB;
        if (B < 0) B = 0;
        else if (B > 255) B = 255;

        G = Y1 - deltaG;
        if (G < 0) G = 0;
        else if (G > 255) G = 255;

        R = Y1 + deltaR;
        if (R < 0) R = 0;
        else if (R > 255) R = 255;
        // NOTE: this assume little-endian encoding
        rgbs[outPtr++] = (Uint8) (((G & 0x3c) << 3) | (B >> 3));
        rgbs[outPtr++] = (Uint8) ((R & 0xf8) | (G >> 5));

        // generate second RGB components
        B = Y2 + deltaB;
        if (B < 0) B = 0;
        else if (B > 255) B = 255;

        G = Y2 - deltaG;
        if (G < 0) G = 0;
        else if (G > 255) G = 255;

        R = Y2 + deltaR;
        if (R < 0) R = 0;
        else if (R > 255) R = 255;
        // NOTE: this assume little-endian encoding
        rgbs[outPtr++] = (Uint8) (((G & 0x3c) << 3) | (B >> 3));
        rgbs[outPtr++] = (Uint8) ((R & 0xf8) | (G >> 5));
    }
}

// unsigned char clip (int valid){
//   return valid < 0 ? 0 : valid > 255 ? 255 : valid;
// }

// void toRGB24(unsigned char *yuvin, int width, int height, unsigned char *rgbout) {
//  int x, y;
//  int temp = 0;

//  unsigned long  idx=0;
//  unsigned char *ybase, *ubase, *vbase;
//  unsigned char Y, U, V;

//  //分别得到y、u、v分量的指针；planar format
//  ybase = yuvin;
//  ubase = ybase + width*height;
//  vbase = ubase + (width*height)/4;

//  for( y = 0; y < Height; y++) {
//      idx = (Height-y-1) * Width * 3; //该值保证所生成的rgb数据逆序存放在rgbbuf中
//      //printf("this is %dth x and index is %d!!!!!!\n",x,idx);
//      for( x = 0; x < Width; x++) {
//          Y = ybase[y*Width+x];
//          U = ubase[(y/2)*(Width/2)+x/2];//注意UV分量的取值方法，y/2的含义
//          V = vbase[(y/2)*(Width/2)+x/2];

//          temp = (int) (Y+1.771*(U - 128));
//          rgbout[idx++] = clip(temp);

//          temp = (int)(Y-0.7145*(V - 128)-0.3456*(U - 128));
//          rgbout[idx++] = clip(temp);

//          temp = (int)(Y+ 1.4022*(V - 128));
//          rgbout[idx++] = clip(temp);
//      }
//  }
// }


// Measurement Functions
void FrameStore_setStartDecodingTime (FrameStore *thiz) {
    thiz->start_decoding_time = SDL_GetTicks();
}
void FrameStore_computeDecodeTime (FrameStore *thiz) {
    thiz->decode_time_in_ms += SDL_GetTicks() - thiz->start_decoding_time;
}
uint32_t FrameStore_getDecodingDelay (FrameStore *thiz) {
    thiz->final_decoding_time = thiz->decode_time_in_ms;
    thiz->decode_time_in_ms = 0;
    return thiz->final_decoding_time;
}
