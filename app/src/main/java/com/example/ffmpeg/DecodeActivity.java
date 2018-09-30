package com.example.ffmpeg;

import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

import java.io.File;

public class DecodeActivity extends AppCompatActivity {

    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("native-lib");
    }

    private EditText mInputView;
    private EditText mOutputView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_decode);

        mInputView = (EditText) findViewById(R.id.input);
        mOutputView = (EditText) findViewById(R.id.output);

        findViewById(R.id.decode).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String sdcardPath = Environment.getExternalStorageDirectory().getPath() + "/";
                String input = sdcardPath + mInputView.getText().toString();
                String output = sdcardPath + mOutputView.getText().toString();
                Log.e("yijun", " input=" + input);
                Log.e("yijun", " output=" + output);

                File file = new File(input);
                Log.e("yijun", "file.exists()" + file.exists());

                decode(input, output);
            }
        });
    }

    public static native int decode(String input, String output);

}
