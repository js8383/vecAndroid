package com.vecandroid;

/**
 * Created by Jason on 5/8/16.
 */
public class MathOpsBench implements VecAndroidApiBench.MathOps {

    private native String intAbsBenchNative(int size);
    private native String floatAbsBenchNative(int size);
    private native String intAddBenchNative(int size);
    private native String floatAddBenchNative(int size);
    private native String intSubBenchNative(int size);
    private native String floatSubBenchNative(int size);
    private native String intMulBenchNative(int size);
    private native String floatMulBenchNative(int size);
    private native String intEqBenchNative(int size);
    private native String floatEqBenchNative(int size);
    private native String intGeBenchNative(int size);
    private native String floatGeBenchNative(int size);
    private native String intLeBenchNative(int size);
    private native String floatLeBenchNative(int size);
    private native String intGtBenchNative(int size);
    private native String floatGtBenchNative(int size);
    private native String intLtBenchNative(int size);
    private native String floatLtBenchNative(int size);
    @Override
    public String intAbsBench(int size) {
        return intAbsBenchNative(size);
    }

    @Override
    public String floatAbsBench(int size) {
        return floatAbsBenchNative(size);
    }

    @Override
    public String intAddBench(int size) {
        return intAddBenchNative(size);
    }

    @Override
    public String floatAddBench(int size) {
        return floatAddBenchNative(size);
    }

    @Override
    public String intSubBench(int size) {
        return intSubBenchNative(size);
    }

    @Override
    public String floatSubBench(int size) {
        return floatSubBenchNative(size);
    }

    @Override
    public String intMulBench(int size) {
        return intMulBenchNative(size);
    }

    @Override
    public String floatMulBench(int size) {
        return floatMulBenchNative(size);
    }

    @Override
    public String intEqBench(int size) {
        return intEqBenchNative(size);
    }

    @Override
    public String floatEqBench(int size) {
        return floatEqBenchNative(size);
    }

    @Override
    public String intGeBench(int size) {
        return intGeBenchNative(size);
    }

    @Override
    public String floatGeBench(int size) {
        return floatGeBenchNative(size);
    }

    @Override
    public String intLeBench(int size) {
        return intLeBenchNative(size);
    }

    @Override
    public String floatLeBench(int size) {
        return floatLeBenchNative(size);
    }

    @Override
    public String intGtBench(int size) {
        return intGtBenchNative(size);
    }

    @Override
    public String floatGtBench(int size) {
        return floatGtBenchNative(size);
    }

    @Override
    public String intLtBench(int size) {
        return intLtBenchNative(size);
    }

    @Override
    public String floatLtBench(int size) {
        return floatLtBenchNative(size);
    }

}
