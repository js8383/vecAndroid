/*******************************************************************************
*                     Generated kernel header: mandelbrot                      *
*******************************************************************************/

#ifndef VECANDROID_VOLUME_KERNEL_H
#define VECANDROID_VOLUME_KERNEL_H

//Kernel arguments
typedef struct {
    float* row;
    float* col;
    float* count;
    float max;
    float w_m1;
    float h_m1;
    float left;
    float right;
    float top;
    float bottom;
    uint64_t N;
} KernelArgs;

void mandelbrot_scalar(KernelArgs* args);

void mandelbrot_vector(KernelArgs* args);

#endif
