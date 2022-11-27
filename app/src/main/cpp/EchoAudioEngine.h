#ifndef AAUDIO_ECHOAUDIOENGINE_H
#define AAUDIO_ECHOAUDIOENGINE_H

#include <thread>
#include "audio_common.h"
#include "AudioEffect.h"

class EchoAudioEngine {

public:
    ~EchoAudioEngine();

    void setRecordingDeviceId(int32_t deviceId);

    void setPlaybackDeviceId(int32_t deviceId);

    void setEchoOn(bool isEchoOn);

    aaudio_data_callback_result_t dataCallback(AAudioStream *stream,
                                               void *audioData,
                                               int32_t numFrames);

    void errorCallback(AAudioStream *stream,
                       aaudio_result_t  __unused error);

    void startRecord(const char *path);

    void stopRecord();

    void startPlayer(const char *path);

    void stopPlayer();

    aaudio_data_callback_result_t dataToPlayCallback(AAudioStream *stream,
                                                     void *audioData,
                                                     int32_t numFrames);

    aaudio_data_callback_result_t dataToRecordCallback(AAudioStream *stream,
                                                       void *audioData,
                                                       int32_t numFrames);

    void recordErrorCallback(AAudioStream *stream,
                             aaudio_result_t  __unused error);

    void playErrorCallback(AAudioStream *stream,
                           aaudio_result_t  __unused error);

private:

    bool isEchoOn_ = false;
    bool isFirstDataCallback_ = true;
    bool isRecordIng = false;
    bool isPlaying = false;
    FILE *recordFile;
    FILE *playFile;
    const char *recordPath;

    int32_t recordingDeviceId_ = AAUDIO_UNSPECIFIED;
    int32_t playbackDeviceId_ = AAUDIO_UNSPECIFIED;
    aaudio_format_t format_ = AAUDIO_FORMAT_PCM_I16;
    int32_t sampleRate_;
    int32_t inputChannelCount_ = kMonoChannelCount;
    int32_t outputChannelCount_ = kStereoChannelCount;
    AAudioStream *recordingStream_ = nullptr;
    AAudioStream *playStream_ = nullptr;
    int32_t framesPerBurst_;
    std::mutex restartingLock_;
    AudioEffect audioEffect_;

    void openRecordingStream(bool isOnlyRecording);

    void drainRecordingStream(void *audioData, int32_t numFrames);

    void openPlaybackStream(bool isOnlyPlaying);

    void startStream(AAudioStream *stream);

    void stopStream(AAudioStream *stream);

    void closeStream(AAudioStream *stream);

    void openAllStreams();

    void closeAllStreams();

    void closeRecordStreams();

    void closePlayStreams();

    void restartStreams();

    void resetRecordStreams();

    void resetPlayStreams();

    AAudioStreamBuilder *createStreamBuilder();

    void setupCommonStreamParameters(AAudioStreamBuilder *builder);

    void setupRecordingStreamParameters(AAudioStreamBuilder *builder, bool isOnlyRecording);

    void setupPlaybackStreamParameters(AAudioStreamBuilder *builder, bool isOnlyPlaying);

    void warnIfNotLowLatency(AAudioStream *stream);

};

#endif //AAUDIO_ECHOAUDIOENGINE_H
