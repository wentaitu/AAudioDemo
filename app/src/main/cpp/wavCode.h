//
// Created by 王煜愉 on 2022/11/27.
//

#ifndef AAUDIODEMO_WAVCODE_H
#define AAUDIODEMO_WAVCODE_H

class wavCode {
private:
public:
    wavCode();

    ~wavCode();

    static long getFileSize(char *filename);

    static int
    pcvToWav(const char *pcmpath, int channles, int sample_rate, int fmtsize, const char *wavpath);
};

#endif //AAUDIODEMO_WAVCODE_H
