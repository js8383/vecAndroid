/*******************************************************************************
*                        Generated entry point for Java                        *
*******************************************************************************/
//Includes
#include <stdlib.h>
#include <jni.h>

//Wrapper for the core function
extern "C" JNIEXPORT jboolean JNICALL Java_com_vecandroid_KernelOps_saxpy(JNIEnv* env, jclass cls, jobject vp_scale, jobject vp_x, jobject vp_y, jobject vp_result) {
    //Make sure the buffers are directly allocated
    jclass Buffer = env->FindClass("java/nio/Buffer");
    jmethodID isDirect = env->GetMethodID(Buffer, "isDirect", "()Z");
    if(!env->CallBooleanMethod(vp_scale, isDirect)) {
        printf("Buffer not direct (scale)\n");
        return false;
    }
    if(!env->CallBooleanMethod(vp_x, isDirect)) {
        printf("Buffer not direct (x)\n");
        return false;
    }
    if(!env->CallBooleanMethod(vp_y, isDirect)) {
        printf("Buffer not direct (y)\n");
        return false;
    }
    if(!env->CallBooleanMethod(vp_result, isDirect)) {
        printf("Buffer not direct (result)\n");
        return false;
    }
    //Number of elements to process
    jlong N = env->GetDirectBufferCapacity(vp_scale);
    if(N == -1) {
        printf("JVM doesn't support direct buffers\n");
        return false;
    }
    //Check length for all buffers
    if(env->GetDirectBufferCapacity(vp_scale) != N) { 
        printf("Java buffer sizes don't match (scale)\n");
        return false;
    }
    if(env->GetDirectBufferCapacity(vp_x) != N) { 
        printf("Java buffer sizes don't match (x)\n");
        return false;
    }
    if(env->GetDirectBufferCapacity(vp_y) != N) { 
        printf("Java buffer sizes don't match (y)\n");
        return false;
    }
    if(env->GetDirectBufferCapacity(vp_result) != N) { 
        printf("Java buffer sizes don't match (result)\n");
        return false;
    }
    //Extract input arrays from buffers
    KernelArgs args;
    args.scale = (float*)env->GetDirectBufferAddress(vp_scale);
    args.x = (float*)env->GetDirectBufferAddress(vp_x);
    args.y = (float*)env->GetDirectBufferAddress(vp_y);
    args.result = (float*)env->GetDirectBufferAddress(vp_result);
    args.N = N;
    if(args.scale == NULL) {
        printf("Error retrieving Java buffer (scale)\n");
        return false;
    }
    if(args.x == NULL) {
        printf("Error retrieving Java buffer (x)\n");
        return false;
    }
    if(args.y == NULL) {
        printf("Error retrieving Java buffer (y)\n");
        return false;
    }
    if(args.result == NULL) {
        printf("Error retrieving Java buffer (result)\n");
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

