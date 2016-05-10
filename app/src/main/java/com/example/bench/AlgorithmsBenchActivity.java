package com.example.bench;

import android.app.Activity;
import android.graphics.Bitmap;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

public class AlgorithmsBenchActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.algorithms_bench_activity);
//        Bitmap image = KernelsBenchWrapper.mandelbrotBenchShow();
//        ImageView imageView = (ImageView) findViewById(R.id.imageView);
//        imageView.setImageBitmap(image);
        TextView tv0 = new TextView(this);
        LinearLayout ll = (LinearLayout)findViewById(R.id.algoritmll);
        tv0.setText("---- Future work as it's not the core of our project\n" +
                "---- Now we just put this as a category of interface in vecAndroid API");
        ll.addView(tv0);
    }
}
