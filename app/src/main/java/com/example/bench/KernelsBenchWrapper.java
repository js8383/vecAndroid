package com.example.bench;

import com.vecandroid.KernelOps;
import com.vecandroid.VecAndroidApi;

import java.nio.FloatBuffer;
import java.util.Random;

/**
 * Created by Jason on 5/9/16.
 */
public class KernelsBenchWrapper {

    private static void volumeJava(double[] a, double[] b) {
        int l = a.length;
        for (int i = 0; i < l; i++) {
            b[i] = (4.0 / 3) * Math.PI * Math.pow(a[i], 3);
        }
    }

    private static void saxpyJava(double[] a, double[] b, double[] c, double[] d) {
        int l = a.length;
        for (int i = 0; i < l; i++) {
            d[i] = a[i] * b[i] + c[i];
        }
    }

    public static String volumenBenchInfo(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        double[] c = new double[size];
        double[] d = new double[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
            float number = -100.0f + random.nextFloat() * 200.0f;
            a[i] = number;
            b[i] = 0.0f;
            c[i] = number;
            d[i] = 0.0;
        }

        // Neon version

        long sizel = size;
        VecAndroidApi.Kernel k = new KernelOps();
        FloatBuffer b1 = KernelOps.newBuffer(sizel);
        FloatBuffer b2 = KernelOps.newBuffer(sizel);
        b1.put(a);
        b1.position(0);
        b2.put(b);
        b2.position(0);


        long start_time = System.nanoTime();
        for (int iter = 0; iter < 1; iter++){
            k.map_volume(b1, b2);
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;

        // Java version

        long start_time1 = System.nanoTime();
        for (int iter = 0; iter < 1; iter++){
            volumeJava(c, d);
        }
        long end_time1 = System.nanoTime();
        double difference1 = (end_time1 - start_time1)/1e6;

        return Double.toString(difference) + ", " + Double.toString(difference1);
    }

    public static String saxpxyBenchInfo(String ssize) {
        int size = Integer.parseInt(ssize);
        float[] a = new float[size];
        float[] b = new float[size];
        float[] c = new float[size];
        float[] d = new float[size];
        double[] da = new double[size];
        double[] db = new double[size];
        double[] dc = new double[size];
        double[] dd = new double[size];

        Random random = new Random();
        for (int i = 0; i < size; i++) {
//            float number = -100.0f + random.nextFloat() * 200.0f;
            float number1 = -100.0f + random.nextFloat() * 200.0f;
            float number2 = -100.0f + random.nextFloat() * 200.0f;
            float number3 = -100.0f + random.nextFloat() * 200.0f;

            a[i] = number1;
            b[i] = number2;
            c[i] = number3;
            d[i] = 0.0f;
            da[i] = number1;
            db[i] = number2;
            dc[i] = number3;
            dd[i] = 0.0f;
        }

        // Neon
        long sizel = size;
        VecAndroidApi.Kernel k = new KernelOps();
        FloatBuffer b1 = KernelOps.newBuffer(sizel);
        FloatBuffer b2 = KernelOps.newBuffer(sizel);
        FloatBuffer b3 = KernelOps.newBuffer(sizel);
        FloatBuffer b4 = KernelOps.newBuffer(sizel);
        b1.put(a);
        b1.position(0);
        b2.put(b);
        b2.position(0);
        b3.put(c);
        b3.position(0);
        b4.put(d);
        b4.position(0);

        long start_time = System.nanoTime();
        for (int iter = 0; iter < 1; iter++){
            k.map_saxpy(b1, b2, b3, b4);
        }
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;

        // Java
        long start_time1 = System.nanoTime();
        for (int iter = 0; iter < 1; iter++){
            saxpyJava(da, db, dc, dd);
        }
        long end_time1 = System.nanoTime();
        double difference1 = (end_time1 - start_time1)/1e6;

        return Double.toString(difference) + ", " + Double.toString(difference1);

    }
}
