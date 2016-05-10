package com.example.bench;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.vecandroid.KernelOps;
import com.vecandroid.VecAndroidApi;
import com.vecandroid.VecAndroidApiBench;

import java.lang.reflect.Array;
import java.nio.FloatBuffer;
import java.util.Arrays;

public class KernelsBenchActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_kernels_bench);

        Intent intent = getIntent();
        String ssize = intent.getStringExtra("size");

        TextView tv0 = new TextView(this);
        TextView tv1 = new TextView(this);
        TextView tv2 = new TextView(this);
        TextView tv3 = new TextView(this);

        LinearLayout ll = (LinearLayout)findViewById(R.id.kernelll);

        tv0.setText(KernelsBenchWrapper.volumenBenchInfo(ssize));
        tv1.setText(KernelsBenchWrapper.saxpxyBenchInfo(ssize));
        tv2.setText(KernelsBenchWrapper.sqrtBenchInfo(ssize));
        tv3.setText(KernelsBenchWrapper.mandelbrotBenchInfo());
//        tv3.setText(KernelsBenchWrapper.mandelbrotBenchShow());
        ll.addView(tv0);
        ll.addView(tv1);
        ll.addView(tv2);
        ll.addView(tv3);

        // Comment out when testing other kernels, as image rendering is time-consuming and
        // thus slows down the activity loading
        Bitmap image = KernelsBenchWrapper.mandelbrotBenchShow();
        ImageView imageView = new ImageView(this);
        imageView.setImageBitmap(image);
        ll.addView(imageView);


    }
}
