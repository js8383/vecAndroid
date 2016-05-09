package com.example.bench;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.media.Image;
import android.widget.ImageView;

import com.vecandroid.KernelOps;
import com.vecandroid.VecAndroidApi;

import java.nio.FloatBuffer;
import java.util.Random;

/**
 * Created by Jason on 5/9/16.
 */
public class KernelsBenchWrapper {

    private static final int max = 100;
    private static final int w = 801;
    private static final int h = 601;
    private static final int N = w * h;
    private static final float w_m1 = w - 1, h_m1 = h - 1;
    private static final float dx = 0f, dy = 0, sx = 4, sy = 3;
    private static final float left = dx - sx / 2f;
    private static final float right = left + sx;
    private static final float bottom = dy - sy / 2f;
    private static final float top = bottom + sy;

    private static void volumeJava(double[] a, double[] b) {
        int l = a.length;
        for (int i = 0; i < l; i++) {
            b[i] = (4.0 / 3) * Math.PI * Math.pow(a[i], 3);
        }
    }

    private static void saxpyJava(double[] a, double[] b, double[] c, double[] d) {
        int l = a.length;
        for (int i = 0; i < l; i++) {
            d[i] = 1.0 * a[i] * b[i] + c[i] + 0.0;
        }
    }

    private static boolean mandelbrotJava(FloatBuffer row_, FloatBuffer col_, FloatBuffer count_,
                                      float max, float w_m1, float h_m1,
                                      float left, float right, float top,float bottom) {
        for (int index = 0; index < N; index++) {
            float row = row_.get();
            float col = col_.get();
            float x0 = left + col * (right - left) / w_m1;
            float y0 = bottom + (h_m1 - row) * (top - bottom) / h_m1;
            float x = 0, y = 0;
            float count = 0;
            float xx = x * x;
            float yy = y * y;
            //Standard escape time
            while (count < max && xx + yy < 16) {
                float temp = xx - yy + x0;
                y = 2 * x * y + y0;
                x = temp;
                xx = x * x;
                yy = y * y;
                ++count;
            }
            //Smooth shading
            if (count < max) {
                count += 1 - Math.log(Math.log(xx + yy) / Math.log(2) / 2.0) / Math.log(2);
            }
            count_.put(count);
        }
        count_.rewind();
        return true;
    }

    private static String inforFormatter(String header, double vectorized_time, double normal_time) {
        double vectorized_speedup = normal_time / vectorized_time;
        return header + "\n" +
                "Java kernel time: " + Double.toString(normal_time) + "ms\n" +
                "Vectorized kernel time: " + Double.toString(vectorized_time) + "ms\n" + " (" + String.format("%.2f",vectorized_speedup) + "x Speedup)\n";

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

        return inforFormatter("Volume function benchmarking...", difference, difference1);
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

        return inforFormatter("Saxpy function benchmarking...", difference, difference1);

    }

    public static Bitmap mandelbrotBenchShow() {
        VecAndroidApi.Kernel k = new KernelOps();
        FloatBuffer row = KernelOps.newBuffer(N);
        FloatBuffer col = KernelOps.newBuffer(N);
        FloatBuffer count = KernelOps.newBuffer(N);
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {
                row.put(r);
                col.put(c);
            }
        }
//        row.rewind();
//        col.rewind();
//        count.rewind();
//        long start_time = System.nanoTime();
//        k.map_mandelbrot(row, col, count, max, w_m1, h_m1, left, right, top, bottom);
//        long end_time = System.nanoTime();
//        String flag2 = Float.toString(count.get(0));
//        double difference = (end_time - start_time)/1e6;

        row.rewind();
        col.rewind();
        count.rewind();
        long start_time1 = System.nanoTime();
//        mandelbrotJava(row, col, count, max, w_m1, h_m1, left, right, top, bottom);
        k.map_mandelbrot(row, col, count, max, w_m1, h_m1, left, right, top, bottom);
        long end_time1 = System.nanoTime();
//        String flag2 = Float.toString(count.get(0));
        double difference1 = (end_time1 - start_time1)/1e6;

        count.rewind();
        // Draw image
        Bitmap image = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        int columnidx = 0;
        int rowidx = 0;
        while(rowidx < h)
        {
            while(columnidx < w)
            {
                if (count.get() < max) {
                    image.setPixel(columnidx, rowidx, Color.WHITE);
                }
                else {
                    image.setPixel(columnidx, rowidx, Color.BLACK);
                }
                columnidx++;
            }
            rowidx++;
            columnidx = 0;
        }
        return image;
    }

    public static String mandelbrotBenchInfo() {
        VecAndroidApi.Kernel k = new KernelOps();
        FloatBuffer row = KernelOps.newBuffer(N);
        FloatBuffer col = KernelOps.newBuffer(N);
        FloatBuffer count = KernelOps.newBuffer(N);
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {
                row.put(r);
                col.put(c);
            }
        }
        row.rewind();
        col.rewind();
        count.rewind();
        long start_time = System.nanoTime();
        k.map_mandelbrot(row, col, count, max, w_m1, h_m1, left, right, top, bottom);
        long end_time = System.nanoTime();
        double difference = (end_time - start_time)/1e6;

        row.rewind();
        col.rewind();
        count.rewind();
        long start_time1 = System.nanoTime();
        mandelbrotJava(row, col, count, max, w_m1, h_m1, left, right, top, bottom);
        long end_time1 = System.nanoTime();
        double difference1 = (end_time1 - start_time1)/1e6;

        return inforFormatter("Mandelbrot function benchmarking...", difference, difference1);
    }
}
