#pragma once
#ifndef _SZ3_LIB_H_
#define _SZ3_LIB_H_

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "SZ3/api/sz.hpp"

#define SZ_FLOAT 0
#define SZ_DOUBLE 1
#define SZ_UINT8 2
#define SZ_INT8 3
#define SZ_UINT16 4
#define SZ_INT16 5
#define SZ_UINT32 6
#define SZ_INT32 7
#define SZ_UINT64 8
#define SZ_INT64 9

#ifdef CREATEDLL_EXPORTS
#define SZ3_DECLSPEC __declspec(dllexport)
#else
#define SZ3_DECLSPEC __declspec(dllimport)
#endif

namespace sz3
{
void usage();
void usage_sz2();

template<class T>
void compress(const char *inPath, const char *cmpPath, SZ::Config conf) {
    T *data = new T[conf.num];
    SZ::readfile<T>(inPath, conf.num, data);

    size_t outSize;
    SZ::Timer timer(true);
    char *bytes = SZ_compress<T>(conf, data, outSize);
    double compress_time = timer.stop();

    char outputFilePath[1024];
    if (cmpPath == nullptr) {
        sprintf(outputFilePath, "%s.sz", inPath);
    } else {
        strcpy(outputFilePath, cmpPath);
    }
    SZ::writefile(outputFilePath, bytes, outSize);

    printf("compression ratio = %.2f \n", conf.num * 1.0 * sizeof(T) / outSize);
    printf("compression time = %f\n", compress_time);
    printf("compressed data file = %s\n", outputFilePath);

    delete[]data;
    delete[]bytes;
}

template <class T>
size_t compress(char* in, char** out, const size_t bytes, SZ::Config conf)
{
    conf.num = bytes / sizeof(T);

    size_t outSize;
    SZ::Timer timer(true);
    *out = SZ_compress<T>(conf, (T*)in, outSize);
    double compress_time = timer.stop();

    //printf("compression ratio = %.2f \n", conf.num * 1.0 * sizeof(T) / outSize);
    //printf("compression time = %f\n", compress_time);

    return outSize;
}

template<class T>
void decompress(const char *inPath, const char *cmpPath, const char *decPath,
                SZ::Config conf,
                int binaryOutput, int printCmpResults) {

    size_t cmpSize;
    auto cmpData = SZ::readfile<char>(cmpPath, cmpSize);

    SZ::Timer timer(true);
    T *decData = SZ_decompress<T>(conf, cmpData.get(), cmpSize);
    double compress_time = timer.stop();

    char outputFilePath[1024];
    if (decPath == nullptr) {
        sprintf(outputFilePath, "%s.out", cmpPath);
    } else {
        strcpy(outputFilePath, decPath);
    }
    if (binaryOutput == 1) {
        SZ::writefile<T>(outputFilePath, decData, conf.num);
    } else {
        SZ::writeTextFile<T>(outputFilePath, decData, conf.num);
    }
    if (printCmpResults) {
        //compute the distortion / compression errors...
        size_t totalNbEle;
        auto ori_data = SZ::readfile<T>(inPath, totalNbEle);
        assert(totalNbEle == conf.num);
        SZ::verify<T>(ori_data.get(), decData, conf.num);
    }
    delete[]decData;

    printf("compression ratio = %f\n", conf.num * sizeof(T) * 1.0 / cmpSize);
    printf("decompression time = %f seconds.\n", compress_time);
    printf("decompressed file = %s\n", outputFilePath);
}

template <class T>
void decompress(char* in, char** out, size_t bytes, SZ::Config conf)
{
    SZ::Timer timer(true);
    *out = (char*)(SZ_decompress<T>(conf, in, bytes));
    double compress_time = timer.stop();
 
    //printf("compression ratio = %f\n", conf.num * sizeof(T) * 1.0 / bytes);
    //printf("decompression time = %f seconds.\n", compress_time);
}

extern "C" int szmain(int argc, const char *argv[]) ;
extern "C" int sz3lib(int argc, const char *argv[], char* inData, char** outData, size_t inDataBytes);
// extern "C" SZ3_DECLSPEC int szmain(int argc, const char *argv[]) ;
// extern "C" SZ3_DECLSPEC int sz3lib(int argc, const char *argv[], char* inData, char** outData, size_t inDataBytes);
}

#endif  //_SZ3_LIB_H_