cmake_minimum_required(VERSION 3.4.1)

# Debug utilities
set(DEBUG_UTILS_PATH "../../../debug-utils")
set(DEBUG_UTILS_SOURCES ${DEBUG_UTILS_PATH}/trace.cpp)

# Code shared between AAudio samples
set(AAUDIO_COMMON_PATH "../../../common")
set(AAUDIO_COMMON_SOURCES ${AAUDIO_COMMON_PATH}/audio_common.cpp)

add_library(echo SHARED
        EchoAudioEngine.cpp
        jni_bridge.cpp
        AudioEffect.cpp
        wavCode.cpp
        ${DEBUG_UTILS_SOURCES}
        ${AAUDIO_COMMON_SOURCES}
        )

target_include_directories(echo PRIVATE
        ${AAUDIO_COMMON_PATH}
        ${DEBUG_UTILS_PATH})

target_link_libraries(echo android atomic log aaudio)
