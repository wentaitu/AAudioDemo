package com.chris.twain;

public enum EchoEngine {

    INSTANCE;

    static {
        System.loadLibrary("echo");
    }

    static native boolean create();

    static native void delete();

    static native void setEchoOn(boolean isEchoOn);

    static native void setRecordingDeviceId(int deviceId);

    static native void setPlaybackDeviceId(int deviceId);

    static native void startRecord();

    static native void stopRecord();

    static native void replay();
}
