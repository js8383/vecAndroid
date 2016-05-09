/*******************************************************************************
*                                Generated core                                *
*******************************************************************************/
//Includes
#include <pthread.h>
#include <stdio.h>
extern "C" {
    #include "neoncompile_volume_kernel.h"
}

//Utility functions
static void* threadStart(void* v) {
    volume_vector((KernelArgs*)v);
    return NULL;
}
static bool isAligned(void* data) {
    return reinterpret_cast<uint64_t>(data) % 16UL == 0UL;
}
static bool checkArgs(KernelArgs* args) {
    if(!isAligned(args->radius)) {
        printf("Array not aligned (radius)\n");
        return false;
    }
    if(!isAligned(args->volume)) {
        printf("Array not aligned (volume)\n");
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
            threadArgs[t].radius = &args->radius[offset];
            threadArgs[t].volume = &args->volume[offset];
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
        lastArgs.radius = &args->radius[offset];
        lastArgs.volume = &args->volume[offset];
        lastArgs.N = args->N - offset;
        volume_scalar(&lastArgs);
    }
    return true;
}

//Additional includes for each programming language
#include "neoncompile_volume_java.h"

