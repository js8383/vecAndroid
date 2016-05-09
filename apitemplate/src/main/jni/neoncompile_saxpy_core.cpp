/*******************************************************************************
*                                Generated core                                *
*******************************************************************************/
//Includes
#include <pthread.h>
#include <stdio.h>
extern "C" {
    #include "neoncompile_saxpy_kernel.h"
}

//Utility functions
static void* threadStart(void* v) {
    saxpy_vector((KernelArgs*)v);
    return NULL;
}
static bool isAligned(void* data) {
    return reinterpret_cast<uint64_t>(data) % 16UL == 0UL;
}
static bool checkArgs(KernelArgs* args) {
    if(!isAligned(args->scale)) {
        printf("Array not aligned (scale)\n");
        return false;
    }
    if(!isAligned(args->x)) {
        printf("Array not aligned (x)\n");
        return false;
    }
    if(!isAligned(args->y)) {
        printf("Array not aligned (y)\n");
        return false;
    }
    if(!isAligned(args->result)) {
        printf("Array not aligned (result)\n");
        return false;
    }
    return true;
}

//Unified core function
static bool run(KernelArgs* args) {
    if(!checkArgs(args)) {
        printf("Arguments are invalid\n");
        return false;
    }
    //Compile-time constants
    const uint64_t vectorSize = 4;
    const uint64_t numThreads = 8;
    //Division of labor
    const uint64_t vectorsPerThread = args->N / (vectorSize * numThreads);
    const uint64_t elementsPerThread = vectorsPerThread * vectorSize;
    //Execute on multiple threads
    uint64_t offset = 0;
    if(elementsPerThread > 0) {
        pthread_t* threads = new pthread_t[numThreads];
        KernelArgs* threadArgs = new KernelArgs[numThreads];
        for(uint64_t t = 0; t < numThreads; t++) {
            threadArgs[t].scale = &args->scale[offset];
            threadArgs[t].x = &args->x[offset];
            threadArgs[t].y = &args->y[offset];
            threadArgs[t].result = &args->result[offset];
            threadArgs[t].N = elementsPerThread;
            offset += elementsPerThread;
            pthread_create(&threads[t], NULL, threadStart, (void*)&threadArgs[t]);
        }
        for(uint64_t t = 0; t < numThreads; t++) {
             pthread_join(threads[t], NULL); 
        }
        delete [] threads;
        delete [] threadArgs;
    }
    //Handle any remaining elements
    if(offset < args->N) {
        KernelArgs lastArgs;
        lastArgs.scale = &args->scale[offset];
        lastArgs.x = &args->x[offset];
        lastArgs.y = &args->y[offset];
        lastArgs.result = &args->result[offset];
        lastArgs.N = args->N - offset;
        saxpy_scalar(&lastArgs);
    }
    return true;
}

//Additional includes for each programming language
#include "neoncompile_saxpy_java.h"

