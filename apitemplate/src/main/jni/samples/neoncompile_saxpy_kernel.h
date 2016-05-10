/*******************************************************************************
*                        Generated kernel header: saxpy                        *
*******************************************************************************/

#ifndef VECANDROID_VOLUME_KERNEL_H
#define VECANDROID_VOLUME_KERNEL_H

//Kernel arguments
typedef struct {
    float* scale;
    float* x;
    float* y;
    float* result;
    uint64_t N;
} KernelArgs;

void saxpy_scalar(KernelArgs* args);

void saxpy_vector(KernelArgs* args);

#endif
