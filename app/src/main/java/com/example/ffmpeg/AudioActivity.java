package com.example.ffmpeg;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;

public class AudioActivity extends Activity {
    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("native-lib");
    }

//    public static final String url = Environment.getExternalStorageDirectory().getPath() + "/test.aac";
    public static final String url = Environment.getExternalStorageDirectory().getPath() + "/test.mp4";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        new Thread(new Runnable() {
            @Override
            public void run() {
                audioPlayer(url);
            }
        }).start();
    }


    public static native void audioPlayer(String url);

//    /**
//     * 音频解码
//     */
//    public void audioPlayer(){
////        /**
////         * 1.播放视频文件中的音频
////         */
////        //   String input = new File(Environment.getExternalStorageDirectory(),"告白气球.avi").getAbsolutePath();
////
////        /**
////         * 2.播放音频文件中的音频
////         */
////        String input = new File(Environment.getExternalStorageDirectory(),"说散就散.mp3").getAbsolutePath();
////        VideoPlayer player = new VideoPlayer();
////        player.audioPlayer(input);
////        Log.d("Main","正在播放");
//    }

}
