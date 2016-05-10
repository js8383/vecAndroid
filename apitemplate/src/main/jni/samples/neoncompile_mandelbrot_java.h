/*******************************************************************************
*                        Generated entry point for Java                        *
*******************************************************************************/
//Includes
#include <stdlib.h>
#include <jni.h>

//Wrapper for the core function
extern "C" JNIEXPORT jboolean JNICALL Java_com_vecandroid_KernelOps_mandelbrot(JNIEnv* env, jclass cls, jobject vp_row, jobject vp_col, jobject vp_count, jfloat vp_max, jfloat vp_w_m1, jfloat vp_h_m1, jfloat vp_left, jfloat vp_right, jfloat vp_top, jfloat vp_bottom) {
    //Make sure the buffers are directly allocated
    jclass Buffer = env->FindClass("java/nio/Buffer");
    jmethodID isDirect = env->GetMethodID(Buffer, "isDirect", "()Z");
    if(!env->CallBooleanMethod(vp_row, isDirect)) {
        printf("Buffer not direct (row)\n");
        return false;
    }
    if(!env->CallBooleanMethod(vp_col, isDirect)) {
        printf("Buffer not direct (col)\n");
        return false;
    }
    if(!env->CallBooleanMethod(vp_count, isDirect)) {
        printf("Buffer not direct (count)\n");
        return false;
    }
    //Number of elements to process
    jlong N = env->GetDirectBufferCapacity(vp_row);
    if(N == -1) {
        printf("JVM doesn't support direct buffers\n");
        return false;
    }
    //Check length for all buffers
    if(env->GetDirectBufferCapacity(vp_row) != N) { 
        printf("Java buffer sizes don't match (row)\n");
        return false;
    }
    if(env->GetDirectBufferCapacity(vp_col) != N) { 
        printf("Java buffer sizes don't match (col)\n");
        return false;
    }
    if(env->GetDirectBufferCapacity(vp_count) != N) { 
        printf("Java buffer sizes don't match (count)\n");
        return false;
    }
    //Extract input arrays from buffers
    KernelArgs args;
    args.row = (float*)env->GetDirectBufferAddress(vp_row);
    args.col = (float*)env->GetDirectBufferAddress(vp_col);
    args.count = (float*)env->GetDirectBufferAddress(vp_count);
    args.max = vp_max;
    args.w_m1 = vp_w_m1;
    args.h_m1 = vp_h_m1;
    args.left = vp_left;
    args.right = vp_right;
    args.top = vp_top;
    args.bottom = vp_bottom;
    args.N = N;
    if(args.row == NULL) {
        printf("Error retrieving Java buffer (row)\n");
        return false;
    }
    if(args.col == NULL) {
        printf("Error retrieving Java buffer (col)\n");
        return false;
    }
    if(args.count == NULL) {
        printf("Error retrieving Java buffer (count)\n");
        return false;
    }
    //Run the kernel
    return run(&args);
}

//Aligned allocation
extern "C" JNIEXPORT jobject JNICALL Java_com_vecandroid_KernelOps_allocate(JNIEnv* env, jclass cls, jlong N) {
    //Allocate space
    void* buffer = memalign(32, (size_t)N);
    //if(result != 0) {
        //printf("Error allocating buffer (%d)\n", result);
        //return NULL;
    //}
    //Instantiate a java ByteBuffer
    jobject byteBuffer = env->NewDirectByteBuffer(buffer, N);
    if(byteBuffer == NULL) { 
        printf("Error instantiating direct ByteBuffer (not supported by JVM)\n");
        return NULL;
    }
    return byteBuffer;
}

//Free
extern "C" JNIEXPORT jboolean JNICALL Java_com_vecandroid_KernelOps_free(JNIEnv* env, jclass cls, jobject buffer) {
    //Make sure the buffer is directly allocated
    jclass Buffer = env->FindClass("java/nio/Buffer");
    jmethodID isDirect = env->GetMethodID(Buffer, "isDirect", "()Z");
    if(!env->CallBooleanMethod(buffer, isDirect)) {
        printf("Buffer not direct\n");
        return false;
    }
    //Free the memory
    free(env->GetDirectBufferAddress(buffer));
    return true;
}

