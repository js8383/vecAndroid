/*******************************************************************************
*                      VecPy generated entry point: Java                       *
*******************************************************************************/
//Includes
#include <stdlib.h>
#include <jni.h>

//Wrapper for the core function
extern "C" JNIEXPORT jboolean JNICALL Java_com_vecandroid_KernelOps_volume(JNIEnv* env, jclass cls, jobject vp_radius, jobject vp_volume) {
    //Make sure the buffers are directly allocated
    jclass Buffer = env->FindClass("java/nio/Buffer");
    jmethodID isDirect = env->GetMethodID(Buffer, "isDirect", "()Z");
    if(!env->CallBooleanMethod(vp_radius, isDirect)) {
        printf("Buffer not direct (radius)\n");
        return false;
    }
    if(!env->CallBooleanMethod(vp_volume, isDirect)) {
        printf("Buffer not direct (volume)\n");
        return false;
    }
    //Number of elements to process
    jlong N = env->GetDirectBufferCapacity(vp_radius);
    if(N == -1) {
        printf("JVM doesn't support direct buffers\n");
        return false;
    }
    //Check length for all buffers
    if(env->GetDirectBufferCapacity(vp_radius) != N) { 
        printf("Java buffer sizes don't match (radius)\n");
        return false;
    }
    if(env->GetDirectBufferCapacity(vp_volume) != N) { 
        printf("Java buffer sizes don't match (volume)\n");
        return false;
    }
    //Extract input arrays from buffers
    KernelArgs args;
    args.radius = (float*)env->GetDirectBufferAddress(vp_radius);
    args.volume = (float*)env->GetDirectBufferAddress(vp_volume);
    args.N = N;
    if(args.radius == NULL) {
        printf("Error retrieving Java buffer (radius)\n");
        return false;
    }
    if(args.volume == NULL) {
        printf("Error retrieving Java buffer (volume)\n");
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
    //    printf("Error allocating buffer (%d)\n", result);
    //    return NULL;
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

