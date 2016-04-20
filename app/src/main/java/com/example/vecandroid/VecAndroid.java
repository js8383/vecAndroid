package com.example.vecandroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TextView;

public class VecAndroid extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);
        /* Create a TextView and set its content.
         * the text is retrieved by calling a native
         * function.
         */
        LinearLayout linearLayout = (LinearLayout)findViewById(R.id.benchmark);
        TextView  tv1 = new TextView(this);
        TextView  tv2 = new TextView(this);
        TextView  tv3 = new TextView(this);
        TextView  tv4 = new TextView(this);
        TextView  tv5 = new TextView(this);
        TextView  tv6 = new TextView(this);
        TextView  tv7 = new TextView(this);
        TextView  tv8 = new TextView(this);
        TextView  tv9 = new TextView(this);
        TextView  tv10 = new TextView(this);
        TextView  tv11 = new TextView(this);
        TextView  tv12 = new TextView(this);
//        tv.setText( stringFromJNI() );
//        tv.setText( myTest(5,3) );
//        tv.setText( testings() );
        tv1.setText( intAbsBench() );
        tv2.setText( floatAbsBench() );
        tv3.setText( intAddBench() );
        tv4.setText( floatAddBench() );
        tv5.setText( intSubBench() );
        tv6.setText( floatSubBench() );
        tv7.setText( intMulBench() );
        tv8.setText( floatMulBench() );
        tv9.setText( intEqBench() );
        tv10.setText( floatEqBench() );
        tv11.setText( intGeBench() );
        tv12.setText( floatGeBench() );

//        setContentView(tv);
        linearLayout.addView(tv1);
        linearLayout.addView(tv2);
        linearLayout.addView(tv3);
        linearLayout.addView(tv4);
        linearLayout.addView(tv5);
        linearLayout.addView(tv6);
        linearLayout.addView(tv7);
        linearLayout.addView(tv8);
        linearLayout.addView(tv9);
        linearLayout.addView(tv10);
        linearLayout.addView(tv11);
        linearLayout.addView(tv12);

    }

    /* A native method that is implemented by the
     * 'vecandroid' native library, which is packaged
     * with this application.
     */
//    public native String stringFromJNI();
//    public native String myTest(int n1, int n2);
//    public native String testings();
    public native String intAbsBench();
    public native String floatAbsBench();
    public native String intAddBench();
    public native String floatAddBench();
    public native String intSubBench();
    public native String floatSubBench();
    public native String intMulBench();
    public native String floatMulBench();
    public native String intEqBench();
    public native String floatEqBench();
    public native String intGeBench();
    public native String floatGeBench();


    /* this is used to load the 'vecandroid' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.neon/lib/libvecandroid.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("vecandroid");
    }
}
