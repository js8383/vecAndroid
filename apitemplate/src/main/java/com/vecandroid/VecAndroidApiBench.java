package com.vecandroid;

/**
 * Created by Jason on 5/8/16.
 */
public interface VecAndroidApiBench {
    public interface MathOps {
        public String intAbsBench(int size);
        public String floatAbsBench(int size);
        public String intAddBench(int size);
        public String floatAddBench(int size);
        public String intSubBench(int size);
        public String floatSubBench(int size);
        public String intMulBench(int size);
        public String floatMulBench(int size);
        public String intEqBench(int size);
        public String floatEqBench(int size);
        public String intGeBench(int size);
        public String floatGeBench(int size);
//        public String intLeBench(int size);
//        public String floatLeBench(int size);
    }

    public interface Kernels {

    }
}
