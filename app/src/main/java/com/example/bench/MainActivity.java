package com.example.bench;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);
        final EditText arraysize =(EditText)findViewById(R.id.editText);
        Button mathops = (Button)findViewById(R.id.button);
        Button algs = (Button)findViewById(R.id.button1);
        Button kernels = (Button)findViewById(R.id.button2);

        mathops.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (arraysize.getText().toString().equals("")) {
                    Context context = getApplicationContext();
                    CharSequence text = "You must specify array length!";
                    int duration = Toast.LENGTH_LONG;
                    Toast toast = Toast.makeText(context, text, duration);
                    toast.show();
                } else {
                    Intent newIntent = new Intent(MainActivity.this, MathOpsBenchActivity.class);
                    newIntent.putExtra("size", arraysize.getText().toString());
                    startActivity(newIntent);
                }

            }
        });

        algs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent newIntent = new Intent(MainActivity.this, AlgorithmsBenchActivity.class);
                startActivity(newIntent);
            }
        });

        kernels.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (arraysize.getText().toString().equals("")) {
                    Context context = getApplicationContext();
                    CharSequence text = "You must specify array length!";
                    int duration = Toast.LENGTH_LONG;
                    Toast toast = Toast.makeText(context, text, duration);
                    toast.show();
                } else {
                    Intent newIntent = new Intent(MainActivity.this, KernelsBenchActivity.class);
                    newIntent.putExtra("size", arraysize.getText().toString());
                    startActivity(newIntent);
                }
            }
        });
    }
}
