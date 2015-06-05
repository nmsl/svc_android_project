/*
 * libdash_networkpart_test.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "libdash_networkpart_test.h"

#include <android/log.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/stat.h>

#define  LOG_TAG    "NetworkTest"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace dash;
using namespace dash::network;
using namespace libdashtest;
using namespace std;

void NetworkTest::download(IConnection *connection, IChunk *chunk, ofstream *file)
{
    int     len     = 32768;
    uint8_t *p_data = new uint8_t[32768];

    int ret = 0;
    do
    {
        ret = connection->Read(p_data, len, chunk);
        if(ret > 0)
            file->write((char *)p_data, ret);
    }while(ret > 0);
}

NetworkTest::NetworkTest()
{
    LOGI("<Init> - Create NetworkTest Class");

    struct stat st;
    if (stat("/sdcard/video", &st) != 0) {
        mkdir("/sdcard/video", 0777);
        LOGI("<Init> - /sdcard/video is not exist, and create video folder");
    }

    this->httpconnection = new HTTPConnection();

    TestChunk test1chunk("www-itec.uni-klu.ac.at", 80, "/~cmueller/libdashtest/network/test1.txt", 0, 0, false);
    TestChunk test2chunk("www-itec.uni-klu.ac.at", 80, "/~cmueller/libdashtest/network/sintel_trailer-480p.mp4", 0, 0, false);

    this->httpconnection->Init(&test1chunk);
    this->httpconnection->Schedule(&test1chunk);

    ofstream file;
    LOGI("<Init> - *****************************************");
    LOGI("<Init> - * Download files with external HTTP 1.0 *");
    LOGI("<Init> - *****************************************");

    LOGI("Testing download of text file");
    file.open("sdcard/video/test1_http_1_0.txt", ios::out | ios::binary);
    download(this->httpconnection, &test1chunk, &file);
    file.close();
    LOGI("<Init> - finished!");

    delete(this->httpconnection);

    this->httpconnection = new HTTPConnection();
    this->httpconnection->Init(&test2chunk);
    this->httpconnection->Schedule(&test2chunk);
    LOGI("<Init> - Testing download of video file:");
    file.open("sdcard/video/sintel_trailer-480p_http_1_0.mp4", ios::out | ios::binary);
    download(this->httpconnection, &test2chunk, &file);
    file.close();
    LOGI("<Init> - finished!");

    LOGI("<Init> - *****************************************");
    LOGI("<Init> - * Download files with external HTTP 1.1 *");
    LOGI("<Init> - *****************************************");
    LOGI("<Init> - Testing download of text file:");
    this->peristenthttpconnection = new PersistentHTTPConnection();
    this->peristenthttpconnection->Init(&test1chunk);
    this->peristenthttpconnection->Schedule(&test1chunk);
    file.open("sdcard/video/test1_http_1_1.txt", ios::out | ios::binary);
    download(this->peristenthttpconnection, &test1chunk, &file);
    file.close();
    LOGI("<Init> - finished!");

    LOGI("<Init> - Testing download of video file:");
    this->peristenthttpconnection->Schedule(&test2chunk);
    file.open("sdcard/video/sintel_trailer-480p_http_1_1.mp4", ios::out | ios::binary);
    download(this->peristenthttpconnection, &test2chunk, &file);
    file.close();
    LOGI("<Init> - finished!");

    delete(this->peristenthttpconnection);
}

NetworkTest::~NetworkTest() {
    delete(this->httpconnection);
}
