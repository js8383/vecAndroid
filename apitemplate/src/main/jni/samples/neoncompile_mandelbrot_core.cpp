/*******************************************************************************
*                                Generated core                                *
*******************************************************************************/
//Includes
#include <pthread.h>
#include <stdio.h>
extern "C" {
    #include "neoncompile_mandelbrot_kernel.h"
}

//Utility functions
static void* threadStart(void* v) {
    mandelbrot_vector((KernelArgs*)v);
    return NULL;
}
static bool isAligned(void* data) {
    return reinterpret_cast<uint64_t>(data) % 16UL == 0UL;
}
static bool checkArgs(KernelArgs* args) {
    if(!isAligned(args->row)) {
        printf("Array not aligned (row)\n");
        return false;
    }
    if(!isAligned(args->col)) {
        printf("Array not aligned (col)\n");
        return false;
    }
    if(!isAligned(args->count)) {
        printf("Array not aligned (count)\n");
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
    mandelbrot_scalar(args);
    return true;
}

//Additional includes for each programming language
#include "neoncompile_mandelbrot_java.h"

