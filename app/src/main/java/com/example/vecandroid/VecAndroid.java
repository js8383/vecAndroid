package com.example.vecandroid;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class VecAndroid extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        /* Create a TextView and set its content.
         * the text is retrieved by calling a native
         * function.
         */
        TextView  tv = new TextView(this);
//        tv.setText( stringFromJNI() );
//        tv.setText( myTest(5,3) );
//        tv.setText( testings() );
//        tv.setText( intAbsBench() );
//        tv.setText( floatAbsBench() );
//        tv.setText( intAddBench() );
//        tv.setText( floatAddBench() );
//        tv.setText( intSubBench() );
//        tv.setText( floatSubBench() );
//        tv.setText( intMulBench() );
//        tv.setText( floatMulBench() );
//        tv.setText( intEqBench() );
//        tv.setText( floatEqBench() );
        tv.setText( intGeBench() );
//        tv.setText( floatGeBench() );


        setContentView(tv);
    }

    /* A native method that is implemented by the
     * 'vecandroid' native library, which is packaged
     * with this application.
     */
//    public native String stringFromJNI();
//    public native String myTest(int n1, int n2);
//    public native String testings();
//    public native String absBench();
//    public native String absfBench();
//    public native String intAddBench();
//    public native String floatAddBench();
//    public native String intSubBench();
//    public native String floatSubBench();
//    public native String intMulBench();
//    public native String floatMulBench();
//    public native String intEqBench();
//    public native String floatEqBench();
    public native String intGeBench();
//    public native String floatGeBench();


    /* this is used to load the 'vecandroid' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.neon/lib/libvecandroid.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("vecandroid");
    }
}
