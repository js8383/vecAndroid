package com.vecandroid.api;

import java.util.List;

/**
 * Created by Jason on 5/5/16.
 */
public interface VecAndroidApi {

    interface MathOp {

        // Absolute value
        public int[] intAbs(int[] a);
        public List<Integer> intAbs(List<Integer> a);
        public float[] floatAbs(float[] a);
        public List<Float> floatAbs(List<Float> a);

        // Addition
        public int[] intAdd(int[] a, int[] b);
        public List<Integer> intAdd(List<Integer> a, List<Integer> b);
        public float[] floatAdd(float[] a, float[] b);
        public List<Float> floatAdd(List<Float> a, List<Float> b);

        // Subtraction
        public int[] intSub(int[] a, int[] b);
        public List<Integer> intSub(List<Integer> a, List<Integer> b);
        public float[] floatSub(float[] a, float[] b);
        public List<Float> floatSub(List<Float> a, List<Float> b);

        // Mutiplication
        public int[] intMul(int[] a, int[] b);
        public List<Integer> intMul(List<Integer> a, List<Integer> b);
        public float[] floatMul(float[] a, float[] b);
        public List<Float> floatMul(List<Float> a, List<Float> b);

        // Comparison - Equal
        public int[] intEq(int[] a, int[] b);
        public List<Integer> intEq(List<Integer> a, List<Integer> b);
        public int[] floatEq(float[] a, float[] b);
        public List<Integer> floatEq(List<Float> a, List<Float> b);

        // Comparison - Greater or equal to
        public int[] intGe(int[] a, int[] b);
        public List<Integer> intGe(List<Integer> a, List<Integer> b);
        public int[] floatGe(float[] a, float[] b);
        public List<Integer> floatGe(List<Float> a, List<Float> b);

        // Comparison - Less or equal to
        public int[] intLe(int[] a, int[] b);
        public List<Integer> intLe(List<Integer> a, List<Integer> b);
        public int[] floatLe(float[] a, float[] b);
        public List<Integer> floatLe(List<Float> a, List<Float> b);

    }

    interface Algorithm {

    }

    interface Kernel {
        public String foo();
    }

    interface BenchReference{
        public int[] intAbsSerial(int[] a);
    }
}
