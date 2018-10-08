package com.example.ffmpeg;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;

public class SLESAudioActivity extends Activity {
    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("native-lib");
    }

//    public static final String url = Environment.getExternalStorageDirectory().getPath() + "/test.aac";
    public static final String url = Environment.getExternalStorageDirectory().getPath() + "/test.mp4";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                play(url);
            }
        },1000);

    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        destroy();
    }

    public static native void play(String url);

    public static native void destroy();

    public static native void stop();

}
