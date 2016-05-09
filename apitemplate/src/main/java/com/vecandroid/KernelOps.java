/*******************************************************************************
*                           Generated API: KernelOps                           *
*******************************************************************************/
package com.vecandroid;
import java.nio.*;
//import java.util.Arrays;

public class KernelOps implements VecAndroidApi.Kernel {
    //Helper functions to allocate and free aligned direct buffers
    private static native ByteBuffer allocate(long N);
    private static native boolean free(Buffer buffer);
    public static FloatBuffer newBuffer(long N) {
        return allocate(N * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
    }
    public static boolean deleteBuffer(Buffer buffer) {
        return free(buffer);
    }
    //Import kernel function library
    public KernelOps() {
        System.loadLibrary("volume");
        System.loadLibrary("saxpy");
    }
    
    //Kernel functions start
    
    //Kernel saxpy
    public static native boolean saxpy(FloatBuffer scale, FloatBuffer x, FloatBuffer y, FloatBuffer result);
    
    @Override
    public boolean map_saxpy(FloatBuffer scale, FloatBuffer x, FloatBuffer y, FloatBuffer result) {
        return saxpy(scale, x, y, result);
    }

    //Kernel volume
    public static native boolean volume(FloatBuffer radius, FloatBuffer volume);
    
    @Override
    public boolean map_volume(FloatBuffer radius, FloatBuffer volume) {
        return volume(radius, volume);
    }

    //Kernel functions end
}
