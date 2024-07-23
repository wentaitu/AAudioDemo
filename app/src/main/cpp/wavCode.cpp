#include <iostream>
#include <logging_macros.h>
#include "wavCode.h"
#include <cstring>
#include <stdint.h>

using namespace std;

#define WAV_DATA_SIZE 4

typedef struct WAV_HEADER {
    uint8_t chunkid[4];
    uint32_t chunksize;//long 4字节
    uint8_t format[WAV_DATA_SIZE];

} pcmHeader;


typedef struct WAV_FMT {
    uint8_t subformat[4];
    uint32_t sbusize;
    uint16_t audioFormat;//short 两字节
    uint16_t numchannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitPerSample;
} pcmFmt;


typedef struct WAV_DATA {
    uint8_t wavdata[WAV_DATA_SIZE];
    uint32_t dataSize;
} pcmData;


long wavCode::getFileSize(char *filename) {
    FILE *fp = fopen(filename, "r");//只读
    if (!fp) {
        return -1;
    }

    fseek(fp, 0, SEEK_END);//指向最后字节
    long size = ftell(fp);//返回文件字节长度
    fclose(fp);

    return size;
}


//pcm 中没有指定信息 通道数 采样率  需要传入
int wavCode::pcvToWav(const char *pcmpath, int channles, int sample_rate, int fmtsize,
                      const char *wavpath) {
    FILE *fp, *fpout;
    WAV_HEADER pcmHeader{};
    WAV_FMT pcmFmt{};
    WAV_DATA pcmData{};
    int bits = 16;

    //打开pcm 文件  二进制方式读
    fp = fopen(pcmpath, "r");
    if (fp == nullptr) {
        cout << "fopen failed" << endl;
        return -1;
    }
    memcpy(pcmHeader.chunkid, "RIFF", strlen("RIFF"));//字符REFF
    long fileSize = 44 + getFileSize((char *) pcmpath) - 8;
    pcmHeader.chunksize = fileSize;//wav文件大小
    memcpy(pcmHeader.format, "WAVE", strlen("WAVE"));//字符WAVE
    memcpy(pcmFmt.subformat, "fmt ", strlen("fmt "));//字符串fmt
    pcmFmt.sbusize = fmtsize;//存储位宽
    pcmFmt.audioFormat = 1;//pcm数据标识
    pcmFmt.numchannels = channles; //通道数
    pcmFmt.sampleRate = sample_rate;//采样率
    pcmFmt.byteRate = sample_rate * channles * bits / 8;//码率 1s中传输的数据字节
    pcmFmt.blockAlign = channles * bits / 8;
    pcmFmt.bitPerSample = bits;//样本格式，16位
    memcpy(pcmData.wavdata, "data", strlen("data"));//字符串data
    pcmData.dataSize = getFileSize((char *) pcmpath);//pcm数据长度
    fpout = fopen(wavpath, "rb+");
    if (fpout == nullptr) {
        cout << "fopen failed" << endl;
        return -1;
    }
    //写入wav头部信息44个字节
    fwrite(&pcmHeader, sizeof(pcmHeader), 1, fpout);
    fwrite(&pcmFmt, sizeof(pcmFmt), 1, fpout);
    fwrite(&pcmData, sizeof(pcmData), 1, fpout);

    //
    char *buff = (char *) malloc(512);
    int len;

    //读取pcm数据写入wav文件
    while ((len = fread(buff, sizeof(char), 512, fp)) != 0) {
        fwrite(buff, sizeof(char), len, fpout);
    }
    free(buff);
    fclose(fp);
    fclose(fpout);
    return 1;
}

wavCode::wavCode() = default;

wavCode::~wavCode() = default;


