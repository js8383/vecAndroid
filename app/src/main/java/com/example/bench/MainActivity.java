package com.example.bench;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);
        Button mathops = (Button)findViewById(R.id.button);
        Button algs = (Button)findViewById(R.id.button1);
        Button kernels = (Button)findViewById(R.id.button2);

        mathops.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent newIntent = new Intent(MainActivity.this, MathOpsBench.class);
                startActivity(newIntent);
            }
        });

        algs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent newIntent = new Intent(MainActivity.this, AlgorithmsBench.class);
                startActivity(newIntent);
            }
        });

        kernels.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent newIntent = new Intent(MainActivity.this, KernelsBench.class);
                startActivity(newIntent);
            }
        });
    }
}
