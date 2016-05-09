package com.example.bench;

import android.app.Activity;
import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.TextView;

//import com.vecandroid.api.KernelOps;
//import com.vecandroid.api.MathOps;
//import com.vecandroid.api.VecAndroidApi;

import java.util.Arrays;

import com.vecandroid.KernelOps;
import com.vecandroid.MathOps;
import com.vecandroid.VecAndroidApi;
import com.example.bench.R;

public class MathOpsBench extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mathops_activity);
        /* Create a TextView and set its content.
         * the text is retrieved by calling a native
         * function.
         */
        getActionBar().setDisplayHomeAsUpEnabled(true);
        LinearLayout linearLayout = (LinearLayout) findViewById(R.id.mathopsbenchmark);
        TextView tv0 = new TextView(this);
        TextView tv1 = new TextView(this);
        TextView tv2 = new TextView(this);
        TextView tv3 = new TextView(this);
        TextView tv4 = new TextView(this);
        TextView tv5 = new TextView(this);
        TextView tv6 = new TextView(this);
        TextView tv7 = new TextView(this);
        TextView tv8 = new TextView(this);
        TextView tv9 = new TextView(this);
        TextView tv10 = new TextView(this);
        TextView tv11 = new TextView(this);
        TextView tv12 = new TextView(this);

//        tv.setText( stringFromJNI() );
//        tv.setText( myTest(5,3) );
//        tv0.setText( testings() );
//        tv1.setText( intAbsBench() );
//        tv2.setText( floatAbsBench() );
//        tv3.setText( intAddBench() );
//        tv4.setText( floatAddBench() );
//        tv5.setText( intSubBench() );
//        tv6.setText( floatSubBench() );
//        tv7.setText( intMulBench() );
//        tv8.setText( floatMulBench() );
//        tv9.setText( intEqBench() );
//        tv10.setText( floatEqBench() );
//        tv11.setText( intGeBench() );
//        tv12.setText( floatGeBench() );

        VecAndroidApi.MathOp m = new MathOps();

        int test0[] = new int[]{-1, -2, -3, -4, -5, -6};
        tv0.setText(Arrays.toString(m.intAbs(test0)));

        float test1[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        tv1.setText(Arrays.toString(m.floatAbs(test1)));

        int test21[] = new int[]{1, 2, 3, 4, 5, 6};
        int test22[] = new int[]{-1, -2, -3, -4, -5, -6};
        tv2.setText(Arrays.toString(m.intAdd(test21, test22)));

        float test31[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        float test32[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        tv3.setText(Arrays.toString(m.floatAdd(test31, test32)));

        int test41[] = new int[]{10, 10, 10, 10, 10, 10};
        int test42[] = new int[]{1, 2, 3, 4, 5, 6};
        tv4.setText(Arrays.toString(m.intSub(test41, test42)));

        float test51[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        float test52[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        tv5.setText(Arrays.toString(m.floatSub(test51, test52)));

        int test61[] = new int[]{10, 10, 10, 10, 10, 10};
        int test62[] = new int[]{1, 2, 3, 4, 5, 6};
        tv6.setText(Arrays.toString(m.intMul(test61, test62)));

        float test71[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        float test72[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        tv7.setText(Arrays.toString(m.floatMul(test71, test72)));

        int test81[] = new int[]{10, 10, 10, 10, 10, 10};
        int test82[] = new int[]{10, 2, 10, 4, 5, 6};
        tv8.setText(Arrays.toString(m.intEq(test81, test82)));

        float test91[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        float test92[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        tv9.setText(Arrays.toString(m.floatEq(test91, test92)));

        int test10_1[] = new int[]{10, 10, 10, 10, 10, 10};
        int test10_2[] = new int[]{10, 2, 11, 4, 5, 6};
        tv10.setText(Arrays.toString(m.intGe(test10_1, test10_2)));

        float test11_1[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        float test11_2[] = new float[]{-1f, -5f, 3f, 2f, 9f};
        tv11.setText(Arrays.toString(m.floatGe(test11_1, test11_2)));

        VecAndroidApi.Kernel k = new KernelOps();
        tv12.setText(k.foo());

//        setContentView(tv);
        linearLayout.addView(tv0);
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
//    public native String intAbsBench();
//    public native String floatAbsBench();
//    public native String intAddBench();
//    public native String floatAddBench();
//    public native String intSubBench();
//    public native String floatSubBench();
//    public native String intMulBench();
//    public native String floatMulBench();
//    public native String intEqBench();
//    public native String floatEqBench();
//    public native String intGeBench();
//    public native String floatGeBench();


    /* this is used to load the 'vecandroid' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.neon/lib/libvecandroid.so at
     * installation time by the package manager.
     */
//    static {
//        System.loadLibrary("vecandroid");
//    }
}
