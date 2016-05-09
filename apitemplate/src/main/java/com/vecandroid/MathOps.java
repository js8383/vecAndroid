package com.vecandroid;

import java.util.List;

/**
 * Created by Jason on 5/5/16.
 */
public class MathOps implements VecAndroidApi.MathOp {

//    public native String testings();
//    private native String intAbsBench();
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

    private native int[] intAbsNative(int[] a);
    private native float[] floatAbsNative(float[] a);
    private native int[] intAddNative(int[] a, int[] b);
    private native float[] floatAddNative(float[] a, float[] b);
    private native int[] intSubNative(int[] a, int[] b);
    private native float[] floatSubNative(float[] a, float[] b);
    private native int[] intMulNative(int[] a, int[] b);
    private native float[] floatMulNative(float[] a, float[] b);
    private native int[] intEqNative(int[] a, int[] b);
    private native int[] floatEqNative(float[] a, float[] b);
    private native int[] intGeNative(int[] a, int[] b);
    private native int[] floatGeNative(float[] a, float[] b);

    public MathOps() {
        System.loadLibrary("vecandroid");
    }


    @Override
    public int[] intAbs(int[] a) {
        return intAbsNative(a);
    }

    @Override
    public List<Integer> intAbs(List<Integer> a) {
        return null;
    }

    @Override
    public float[] floatAbs(float[] a) {
        return floatAbsNative(a);
    }

    @Override
    public List<Float> floatAbs(List<Float> a) {
        return null;
    }

    @Override
    public int[] intAdd(int[] a, int[] b) {
        return intAddNative(a, b);
    }

    @Override
    public List<Integer> intAdd(List<Integer> a, List<Integer> b) {
        return null;
    }

    @Override
    public float[] floatAdd(float[] a, float[] b) {
        return floatAddNative(a, b);
    }

    @Override
    public List<Float> floatAdd(List<Float> a, List<Float> b) {
        return null;
    }

    @Override
    public int[] intSub(int[] a, int[] b) {
        return intSubNative(a, b);
    }

    @Override
    public List<Integer> intSub(List<Integer> a, List<Integer> b) {
        return null;
    }

    @Override
    public float[] floatSub(float[] a, float[] b) {
        return floatSubNative(a, b);
    }

    @Override
    public List<Float> floatSub(List<Float> a, List<Float> b) {
        return null;
    }

    @Override
    public int[] intMul(int[] a, int[] b) {
        return intMulNative(a, b);
    }

    @Override
    public List<Integer> intMul(List<Integer> a, List<Integer> b) {
        return null;
    }

    @Override
    public float[] floatMul(float[] a, float[] b) {
        return floatMulNative(a, b);
    }

    @Override
    public List<Float> floatMul(List<Float> a, List<Float> b) {
        return null;
    }

    @Override
    public int[] intEq(int[] a, int[] b) {
        return intEqNative(a, b);
    }

    @Override
    public List<Integer> intEq(List<Integer> a, List<Integer> b) {
        return null;
    }

    @Override
    public int[] floatEq(float[] a, float[] b) {
        return floatEqNative(a, b);
    }

    @Override
    public List<Integer> floatEq(List<Float> a, List<Float> b) {
        return null;
    }

    @Override
    public int[] intGe(int[] a, int[] b) {
        return intGeNative(a, b);
    }

    @Override
    public List<Integer> intGe(List<Integer> a, List<Integer> b) {
        return null;
    }

    @Override
    public int[] floatGe(float[] a, float[] b) {
        return floatGeNative(a, b);
    }

    @Override
    public List<Integer> floatGe(List<Float> a, List<Float> b) {
        return null;
    }

    @Override
    public int[] intLe(int[] a, int[] b) {
        return null;
    }

    @Override
    public List<Integer> intLe(List<Integer> a, List<Integer> b) {
        return null;
    }

    @Override
    public int[] floatLe(float[] a, float[] b) {
        return null;
    }

    @Override
    public List<Integer> floatLe(List<Float> a, List<Float> b) {
        return null;
    }
}
