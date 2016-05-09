/*******************************************************************************
*                         Generated API: VecAndroidApi                         *
*******************************************************************************/
package com.vecandroid;
import java.nio.*;
import java.util.List;

public interface VecAndroidApi {
    //Library for math operations
    interface MathOp {
        //Absolute value
        public int[] intAbs(int[] a);
        public List<Integer> intAbs(List<Integer> a);
        public float[] floatAbs(float[] a);
        public List<Float> floatAbs(List<Float> a);
        //Addition
        public int[] intAdd(int[] a, int[] b);
        public List<Integer> intAdd(List<Integer> a, List<Integer> b);
        public float[] floatAdd(float[] a, float[] b);
        public List<Float> floatAdd(List<Float> a, List<Float> b);
        //Subtraction
        public int[] intSub(int[] a, int[] b);
        public List<Integer> intSub(List<Integer> a, List<Integer> b);
        public float[] floatSub(float[] a, float[] b);
        public List<Float> floatSub(List<Float> a, List<Float> b);
        //Mutiplication
        public int[] intMul(int[] a, int[] b);
        public List<Integer> intMul(List<Integer> a, List<Integer> b);
        public float[] floatMul(float[] a, float[] b);
        public List<Float> floatMul(List<Float> a, List<Float> b);
        //Comparison - Equal
        public int[] intEq(int[] a, int[] b);
        public List<Integer> intEq(List<Integer> a, List<Integer> b);
        public int[] floatEq(float[] a, float[] b);
        public List<Integer> floatEq(List<Float> a, List<Float> b);
        //Comparison - Greater than or equal to
        public int[] intGe(int[] a, int[] b);
        public List<Integer> intGe(List<Integer> a, List<Integer> b);
        public int[] floatGe(float[] a, float[] b);
        public List<Integer> floatGe(List<Float> a, List<Float> b);
        //Comparison - Less than or equal to
        public int[] intLe(int[] a, int[] b);
        public List<Integer> intLe(List<Integer> a, List<Integer> b);
        public int[] floatLe(float[] a, float[] b);
        public List<Integer> floatLe(List<Float> a, List<Float> b);
        //Comparison - Greater than
        public int[] intGt(int[] a, int[] b);
        public List<Integer> intGt(List<Integer> a, List<Integer> b);
        public int[] floatGt(float[] a, float[] b);
        public List<Integer> floatGt(List<Float> a, List<Float> b);
        //Comparison - Less than
        public int[] intLt(int[] a, int[] b);
        public List<Integer> intLt(List<Integer> a, List<Integer> b);
        public int[] floatLt(float[] a, float[] b);
        public List<Integer> floatLt(List<Float> a, List<Float> b);
    }
    //Library for Algorithms
    interface Algorithm {
        
    }
    //Library for Kernel
    interface Kernel {
        public boolean map_volume(FloatBuffer radius, FloatBuffer volume);
        public boolean map_saxpy(FloatBuffer scale, FloatBuffer x, FloatBuffer y, FloatBuffer result);
        public boolean map_mandelbrot(FloatBuffer row, FloatBuffer col, FloatBuffer count, float max, float w_m1, float h_m1, float left, float right, float top, float bottom);
        
    }
    //BenchReference
    interface BenchReference {
        public int[] intAbsSerial(int[] a);
    }
}
