/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <jni.h>
#include <logging_macros.h>
#include "EchoAudioEngine.h"

static EchoAudioEngine *engine = nullptr;

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_chris_twain_EchoEngine_create(JNIEnv *env,
                                       jclass) {
    if (engine == nullptr) {
        engine = new EchoAudioEngine();
    }

    return (engine != nullptr);
}

JNIEXPORT void JNICALL
Java_com_chris_twain_EchoEngine_delete(JNIEnv *env,
                                       jclass) {
    delete engine;
    engine = nullptr;
}

JNIEXPORT void JNICALL
Java_com_chris_twain_EchoEngine_setEchoOn(JNIEnv *env,
                                          jclass, jboolean isEchoOn) {
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return;
    }

    engine->setEchoOn(isEchoOn);
}

JNIEXPORT void JNICALL
Java_com_chris_twain_EchoEngine_setRecordingDeviceId(JNIEnv *env,
                                                     jclass, jint deviceId) {
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return;
    }

    engine->setRecordingDeviceId(deviceId);
}

JNIEXPORT void JNICALL
Java_com_chris_twain_EchoEngine_setPlaybackDeviceId(JNIEnv *env,
                                                    jclass, jint deviceId) {
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return;
    }

    engine->setPlaybackDeviceId(deviceId);
}

JNIEXPORT void JNICALL
Java_com_chris_twain_EchoEngine_startRecord(JNIEnv *env, jclass clazz,
                                            jstring path) {
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return;
    }
    const char *str;
    str = env->GetStringUTFChars(path, NULL);
    if (str == NULL) {
        LOGE("str path is null");
    }
    LOGE("str path is %s", str);
    engine->startRecord(str);
}

JNIEXPORT void JNICALL
Java_com_chris_twain_EchoEngine_stopRecord(JNIEnv *env, jclass clazz) {
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return;
    }
    engine->stopRecord();
}

JNIEXPORT void JNICALL
Java_com_chris_twain_EchoEngine_startPlayer(JNIEnv *env, jclass clazz,
                                            jstring path) {
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return;
    }
    const char *str;
    str = env->GetStringUTFChars(path, NULL);
    if (str == NULL) {
        LOGE("str path is null");
    }
    engine->startPlayer(str);
}

JNIEXPORT void JNICALL
Java_com_chris_twain_EchoEngine_stopPlayer(JNIEnv *env, jclass clazz) {
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return;
    }
    engine->stopPlayer();
}
}
