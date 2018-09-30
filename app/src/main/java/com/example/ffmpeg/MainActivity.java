package com.example.ffmpeg;

import android.os.Build;
import android.os.Environment;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;
import android.widget.VideoView;

import java.io.File;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback{


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("native-lib");
    }

//    public static final String url = "http://fs.mv.android.kugou.com/201809301131/34c73aea00fad96ad2dede08f5068d85/G141/M06/0A/0D/bZQEAFuvMXKARULbAUZH52ezEBM474.mp4";
//    public static final String url = Environment.getExternalStorageDirectory().getPath() + "/test.mp4";
    public static final String url = Environment.getExternalStorageDirectory().getPath() + "/test1.h264";
//    public static final String url = Environment.getExternalStorageDirectory().getPath() + "/test.yuv";

    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        File file = new File(url);
        Log.e("yijun", "file.exists()" + file.exists());

        final TextView tv = (TextView) findViewById(R.id.tv);
        mSurfaceView = (SurfaceView) findViewById(R.id.surface_view);
        tv.setMovementMethod(ScrollingMovementMethod.getInstance());

        mSurfaceHolder = mSurfaceView.getHolder();
        mSurfaceHolder.addCallback(this);

        findViewById(R.id.configurationinfo).setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
            @Override
            public void onClick(View v) {
                tv.setText(configurationinfo());
            }
        });

        findViewById(R.id.urlprotocolinfo).setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
            @Override
            public void onClick(View v) {
                tv.setText(urlprotocolinfo());
            }
        });

        findViewById(R.id.avformatinfo).setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
            @Override
            public void onClick(View v) {
                tv.setText(avformatinfo());
            }
        });

        findViewById(R.id.avcodecinfo).setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
            @Override
            public void onClick(View v) {
                tv.setText(avcodecinfo());
            }
        });

        findViewById(R.id.avfilterinfo).setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
            @Override
            public void onClick(View v) {
                tv.setText(avfilterinfo());
            }
        });


    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                playVideo(url,mSurfaceHolder.getSurface());
            }
        }).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }


    public static native String configurationinfo();

    public static native String urlprotocolinfo();

    public static native String avformatinfo();

    public static native String avcodecinfo();

    public static native String avfilterinfo();



    public static native int playVideo(String url, Object surface);
}
