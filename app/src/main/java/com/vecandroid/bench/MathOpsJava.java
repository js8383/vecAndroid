package com.vecandroid.bench;

/**
 * Created by Jason on 5/5/16.
 */
public class MathOpsJava {

    public MathOpsJava() {

    }

    public int[] intAbsJava(int[] a) {
        int[] result = new int[a.length];
        for (int i = 0; i < a.length; i++) {
            result[i] = Math.abs(a[i]);
        }
        return result;
    }
}
