package com.example.bench;

import com.vecandroid.MathOps;
import com.vecandroid.MathOpsBench;
import com.vecandroid.VecAndroidApi;
import com.vecandroid.VecAndroidApiBench;

import java.util.Random;

/**
 * Created by Jason on 5/5/16.
 */

public class MathOpsBenchWrapper {

    private static final int ITERATIONS = 1000;
    private static final VecAndroidApiBench.MathOps mb = new MathOpsBench();

    private static String mergeInfo(String nativeInfo, double java_time, int size) {
        String[] lines = nativeInfo.split(System.getProperty("line.separator"));
        String c_info_foo = lines[1].split(":")[1];
        String neon_info_foo = lines[2].split(":")[1];
        double c_time = Double.parseDouble(c_info_foo.substring(0, c_info_foo.indexOf("ms")));
        double neon_time = Double.parseDouble(neon_info_foo.substring(0, neon_info_foo.indexOf("ms")));
        double c_speedup = java_time / c_time;
        double neon_speedup = java_time / neon_time;
        return lines[0] + "\n" + "Array length:  " + Integer.toString(size) + "\n"
                + "Iterations:  " + ITERATIONS + "\n"
                + "Java time:  " + Double.toString(java_time) + "ms\n"
                + "Native C time:  " + Double.toString(c_time) + "ms" + " (" + String.format("%.2f",c_speedup) + "X Speedup)\n"
                + "NEON intrinsics time:  " + Double.toString(neon_time) + "ms" + " (" + String.format("%.2f",neon_speedup) + "X Speedup)\n";
    }

    public static String intAbsBenchInfo(int size) {
        String nativeInfo = mb.intAbsBench(size);
        String ssize = Integer.toString(size);
        double java_time = intAbsJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatAbsBenchInfo(int size) {
        String nativeInfo = mb.floatAbsBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatAbsJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String intAddBenchInfo(int size) {
        String nativeInfo = mb.intAddBench(size);
        String ssize = Integer.toString(size);
        double java_time = intAddJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatAddBenchInfo(int size) {
        String nativeInfo = mb.floatAddBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatAddJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String intSubBenchInfo(int size) {
        String nativeInfo = mb.intSubBench(size);
        String ssize = Integer.toString(size);
        double java_time = intSubJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatSubBenchInfo(int size) {
        String nativeInfo = mb.floatSubBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatSubJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String intMulBenchInfo(int size) {
        String nativeInfo = mb.intMulBench(size);
        String ssize = Integer.toString(size);
        double java_time = intMulJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatMulBenchInfo(int size) {
        String nativeInfo = mb.floatMulBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatMulJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String intEqBenchInfo(int size) {
        String nativeInfo = mb.intEqBench(size);
        String ssize = Integer.toString(size);
        double java_time = intEqJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatEqBenchInfo(int size) {
        String nativeInfo = mb.floatEqBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatEqJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String intGeBenchInfo(int size) {
        String nativeInfo = mb.intGeBench(size);
        String ssize = Integer.toString(size);
        double java_time = intGeJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatGeBenchInfo(int size) {
        String nativeInfo = mb.floatGeBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatGeJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String intLeBenchInfo(int size) {
        String nativeInfo = mb.intLeBench(size);
        String ssize = Integer.toString(size);
        double java_time = intGeJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatLeBenchInfo(int size) {
        String nativeInfo = mb.floatLeBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatGeJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String intGtBenchInfo(int size) {
        String nativeInfo = mb.intGtBench(size);
        String ssize = Integer.toString(size);
        double java_time = intGtJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatGtBenchInfo(int size) {
        String nativeInfo = mb.floatGtBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatGtJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String intLtBenchInfo(int size) {
        String nativeInfo = mb.intLtBench(size);
        String ssize = Integer.toString(size);
        double java_time = intGtJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }

    public static String floatLtBenchInfo(int size) {
        String nativeInfo = mb.floatLtBench(size);
        String ssize = Integer.toString(size);
        double java_time = floatGtJava(ssize);
        return mergeInfo(nativeInfo, java_time, size);
    }


    // -----------------------------------------------

    private static double intAbsJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number = -50 + random.nextInt(100);
            a[i] = number;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                result[j] = Math.abs(a[j]);
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatAbsJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number;
        }


        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                result[j] = Math.abs(a[j]);
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double intAddJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] b = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number1 = -50 + random.nextInt(100);
            a[i] = number1;
            int number2 = -50 + random.nextInt(100);
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                result[j] = a[j] + b[j];
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatAddJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number1 = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number1;
            float number2 = -0.1f + random.nextFloat() * 4.0f;
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                result[j] = a[j] + b[j];
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double intSubJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] b = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number1 = -50 + random.nextInt(100);
            a[i] = number1;
            int number2 = -50 + random.nextInt(100);
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                result[j] = a[j] - b[j];
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatSubJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number1 = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number1;
            float number2 = -0.1f + random.nextFloat() * 4.0f;
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                result[j] = a[j] - b[j];
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double intMulJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] b = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number1 = -50 + random.nextInt(100);
            a[i] = number1;
            int number2 = -50 + random.nextInt(100);
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                result[j] = a[j] * b[j];
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatMulJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number1 = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number1;
            float number2 = -0.1f + random.nextFloat() * 4.0f;
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                result[j] = a[j] * b[j];
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double intEqJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] b = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number1 = -50 + random.nextInt(100);
            a[i] = number1;
            int number2 = -50 + random.nextInt(100);
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] == b[j]) {
                    result[j] = 1;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatEqJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number1 = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number1;
            float number2 = -0.1f + random.nextFloat() * 4.0f;
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] == b[j]) {
                    result[j] = 1.0f;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double intGeJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] b = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number1 = -50 + random.nextInt(100);
            a[i] = number1;
            int number2 = -50 + random.nextInt(100);
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] >= b[j]) {
                    result[j] = 1;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatGeJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number1 = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number1;
            float number2 = -0.1f + random.nextFloat() * 4.0f;
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] >= b[j]) {
                    result[j] = 1.0f;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double intLeJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] b = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number1 = -50 + random.nextInt(100);
            a[i] = number1;
            int number2 = -50 + random.nextInt(100);
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] <= b[j]) {
                    result[j] = 1;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatLeJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number1 = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number1;
            float number2 = -0.1f + random.nextFloat() * 4.0f;
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] <= b[j]) {
                    result[j] = 1.0f;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }
    private static double intGtJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] b = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number1 = -50 + random.nextInt(100);
            a[i] = number1;
            int number2 = -50 + random.nextInt(100);
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] > b[j]) {
                    result[j] = 1;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatGtJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number1 = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number1;
            float number2 = -0.1f + random.nextFloat() * 4.0f;
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] > b[j]) {
                    result[j] = 1.0f;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }
    private static double intLtJava(String ssize) {
        int size = Integer.parseInt(ssize);
        int[] a = new int[size];
        int[] b = new int[size];
        int[] result = new int[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            int number1 = -50 + random.nextInt(100);
            a[i] = number1;
            int number2 = -50 + random.nextInt(100);
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] < b[j]) {
                    result[j] = 1;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }

    private static double floatLtJava(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] result = new float[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number1 = -0.1f + random.nextFloat() * 4.0f;
            a[i] = number1;
            float number2 = -0.1f + random.nextFloat() * 4.0f;
            b[i] = number2;
        }

        long start_time = System.nanoTime();
        for (int iter = 0; iter < ITERATIONS; iter++){
            for (int j = 0; j < size; j++) {
                if (a[j] < b[j]) {
                    result[j] = 1.0f;
                }
            }
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;
        return difference;
    }
}
