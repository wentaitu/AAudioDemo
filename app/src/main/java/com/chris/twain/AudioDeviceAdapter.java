package com.chris.twain;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

/**
 * Provides views for a list of audio devices. Usually used as an Adapter for a Spinner or ListView.
 */
public class AudioDeviceAdapter extends ArrayAdapter<AudioDeviceListEntry> {

    public AudioDeviceAdapter(Context context) {
        super(context, R.layout.audio_devices);
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        return getDropDownView(position, convertView, parent);
    }

    @Override
    public View getDropDownView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        View rowView = convertView;
        if (rowView == null) {
            LayoutInflater inflater = LayoutInflater.from(parent.getContext());
            rowView = inflater.inflate(R.layout.audio_devices, parent, false);
        }

        TextView deviceName = rowView.findViewById(R.id.device_name);
        AudioDeviceListEntry deviceInfo = getItem(position);
        deviceName.setText(deviceInfo.getName());

        return rowView;
    }
}
