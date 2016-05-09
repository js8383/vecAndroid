package com.example.bench;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
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

        TextView tv0 = new TextView(this);
        TextView tv1 = new TextView(this);

        LinearLayout ll = (LinearLayout)findViewById(R.id.kernelll);

        tv0.setText(KernelsBenchWrapper.volumenBenchInfo("100000"));
        tv1.setText(KernelsBenchWrapper.saxpxyBenchInfo("100000"));
        ll.addView(tv0);
        ll.addView(tv1);

    }
}
