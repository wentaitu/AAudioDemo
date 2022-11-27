#include "AudioEffect.h"

void AudioEffect::process(int16_t *inputBuffer, int32_t samplesPerFrame, int32_t numFrames) {

    for (int i = 0; i < (numFrames * samplesPerFrame); i++) {

        // DO SOMETHING MORE EXCITING HERE!
        inputBuffer[i] = inputBuffer[i];
    }
}
