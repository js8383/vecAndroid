/*******************************************************************************
*                           Generated API: KernelOps                           *
*******************************************************************************/
package com.vecandroid;
import java.nio.*;
//import java.util.Arrays;

public class KernelOps implements VecAndroidApi.Kernel {
    //JNI wrappers
    public static native boolean volume1(FloatBuffer radius, FloatBuffer volume);
    private static native ByteBuffer allocate(long N);
    private static native boolean free(Buffer buffer);
    //Helper functions to allocate and free aligned direct buffers
    public static FloatBuffer newBuffer(long N) {
        return allocate(N * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
    }
    public static boolean deleteBuffer(Buffer buffer) {
        return free(buffer);
    }
    
    //Import kernel function library
    public KernelOps() { System.loadLibrary("volume1"); }
    
    //Testing section (implementing functions in VecAndroidApi)
    @Override
    public String foo() {
        long b = 3L;
        FloatBuffer b1 = newBuffer(b);
        FloatBuffer b2 = newBuffer(b);
        float[] b11 = new float[] {1.f, 2.f, 3.f};
        float[] b12 = new float[] {1.f, 2.f, 3.f};
        b1.put(b11);
        b2.put(b12);
        b1.position(0);
        b2.position(0);
        volume1(b1, b2);
        return(Float.toString(b2.get(2)));
    }
}

