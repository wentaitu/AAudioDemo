package com.chris.twain;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.media.AudioManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

import java.io.File;

public class MainActivity extends Activity
        implements ActivityCompat.OnRequestPermissionsResultCallback {

    private static final String TAG = MainActivity.class.getName();
    private static final int AUDIO_ECHO_REQUEST = 0;
    private static final int AUDIO_RECORD_REQUEST = 1;

    private TextView statusText;
    private Button toggleEchoButton;
    private AudioDeviceSpinner recordingDeviceSpinner;
    private AudioDeviceSpinner playbackDeviceSpinner;
    private boolean isEchoPlaying = false;
    private boolean isPlaying = false;
    private boolean isRecording = false;

    private Button btnRecord;
    private Button btnReplay;

    private String recordCacheFilePath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        recordCacheFilePath = getCacheDir().getAbsolutePath() + "/recordAudio.wav";
        Log.i(TAG, "recording path " + recordCacheFilePath);

        statusText = findViewById(R.id.status_view_text);
        toggleEchoButton = findViewById(R.id.button_toggle_echo);
        toggleEchoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                toggleEcho();
            }
        });
        toggleEchoButton.setText(getString(R.string.start_echo));

        recordingDeviceSpinner = findViewById(R.id.recording_devices_spinner);
        recordingDeviceSpinner.setDirectionType(AudioManager.GET_DEVICES_INPUTS);
        recordingDeviceSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                EchoEngine.setRecordingDeviceId(getRecordingDeviceId());
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
                // Do nothing
            }
        });

        playbackDeviceSpinner = findViewById(R.id.playback_devices_spinner);
        playbackDeviceSpinner.setDirectionType(AudioManager.GET_DEVICES_OUTPUTS);
        playbackDeviceSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                EchoEngine.setPlaybackDeviceId(getPlaybackDeviceId());
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
                // Do nothing
            }
        });

        btnRecord = findViewById(R.id.button_recording);
        btnReplay = findViewById(R.id.button_replay);
        btnRecord.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startRecording();
            }
        });

        btnReplay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!new File(recordCacheFilePath).exists()) {
                    Toast.makeText(getApplicationContext(),
                            getString(R.string.status_not_file),
                            Toast.LENGTH_SHORT)
                            .show();
                    return;
                }

                if (!isPlaying) {
                    isPlaying = true;
                    EchoEngine.startPlayer(recordCacheFilePath);
                    setSpinnersEnabled(false);
                    btnRecord.setEnabled(false);
                    toggleEchoButton.setEnabled(false);
                    statusText.setText(R.string.stop_player);
                    btnReplay.setText(R.string.stop_player);
                } else {
                    EchoEngine.stopPlayer();
                    isPlaying = false;
                    setSpinnersEnabled(true);
                    btnRecord.setEnabled(true);
                    toggleEchoButton.setEnabled(true);
                    statusText.setText(R.string.start_player);
                    btnReplay.setText(R.string.start_player);
                }
            }
        });

        EchoEngine.create();
    }

    @Override
    protected void onStart() {
        super.onStart();
        setVolumeControlStream(AudioManager.STREAM_MUSIC);
    }

    @Override
    protected void onDestroy() {
        EchoEngine.delete();
        super.onDestroy();
    }

    private void startRecording() {
        Log.i(TAG, "recording " + isRecording + ", " + recordCacheFilePath);
        if (!isRecording) {
            if (!isRecordPermissionGranted()) {
                requestRecordPermission(AUDIO_RECORD_REQUEST);
                return;
            }

            isRecording = true;
            EchoEngine.startRecord(recordCacheFilePath);
            setSpinnersEnabled(false);
            btnReplay.setEnabled(false);
            toggleEchoButton.setEnabled(false);
            statusText.setText(R.string.stop_record);
            btnRecord.setText(R.string.stop_record);
        } else {
            EchoEngine.stopRecord();
            isRecording = false;
            setSpinnersEnabled(true);
            btnReplay.setEnabled(true);
            toggleEchoButton.setEnabled(true);
            statusText.setText(R.string.start_record);
            btnRecord.setText(R.string.start_record);
        }
    }

    public void toggleEcho() {
        if (isEchoPlaying) {
            stopEchoing();
        } else {
            startEchoing();
        }
    }

    private void startEchoing() {
        Log.d(TAG, "Attempting to start");

        if (!isRecordPermissionGranted()) {
            requestRecordPermission(AUDIO_ECHO_REQUEST);
            return;
        }

        setSpinnersEnabled(false);
        btnRecord.setEnabled(false);
        btnReplay.setEnabled(false);
        EchoEngine.setEchoOn(true);
        statusText.setText(R.string.status_echoing);
        toggleEchoButton.setText(R.string.stop_echo);
        isEchoPlaying = true;
    }

    private void stopEchoing() {
        Log.d(TAG, "Playing, attempting to stop");
        EchoEngine.setEchoOn(false);
        resetStatusView();
        toggleEchoButton.setText(R.string.start_echo);
        isEchoPlaying = false;
        setSpinnersEnabled(true);
        btnRecord.setEnabled(true);
        btnReplay.setEnabled(true);
    }

    private void setSpinnersEnabled(boolean isEnabled) {
        recordingDeviceSpinner.setEnabled(isEnabled);
        playbackDeviceSpinner.setEnabled(isEnabled);
    }

    private int getRecordingDeviceId() {
        return ((AudioDeviceListEntry) recordingDeviceSpinner.getSelectedItem()).getId();
    }

    private int getPlaybackDeviceId() {
        return ((AudioDeviceListEntry) playbackDeviceSpinner.getSelectedItem()).getId();
    }

    private boolean isRecordPermissionGranted() {
        return (ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) ==
                PackageManager.PERMISSION_GRANTED);
    }

    private void requestRecordPermission(int reqCode) {
        ActivityCompat.requestPermissions(
                this,
                new String[]{Manifest.permission.RECORD_AUDIO},
                reqCode);
    }

    private void resetStatusView() {
        statusText.setText(R.string.status_warning);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {

        if (AUDIO_ECHO_REQUEST != requestCode && AUDIO_RECORD_REQUEST != requestCode) {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
            return;
        }

        if (grantResults.length != 1 ||
                grantResults[0] != PackageManager.PERMISSION_GRANTED) {

            // User denied the permission, without this we cannot record audio
            // Show a toast and update the status accordingly
            statusText.setText(R.string.status_record_audio_denied);
            Toast.makeText(getApplicationContext(),
                    getString(R.string.need_record_audio_permission),
                    Toast.LENGTH_SHORT)
                    .show();
        } else if (AUDIO_ECHO_REQUEST == requestCode) {
            // Permission was granted, start echoing
            toggleEcho();
        } else if (AUDIO_RECORD_REQUEST == requestCode) {
            startRecording();
        } else {
            Log.i(TAG, "onRequestPermissionsResult " + requestCode);
        }
    }
}
