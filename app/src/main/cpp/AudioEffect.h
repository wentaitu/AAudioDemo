#ifndef AAUDIO_EFFECT_PROCESSOR_H
#define AAUDIO_EFFECT_PROCESSOR_H


#include <cstdint>

class AudioEffect {
public:
    void process(int16_t *inputBuffer, int32_t samplesPerFrame, int32_t numFrames);
};


#endif //AAUDIO_EFFECT_PROCESSOR_H
