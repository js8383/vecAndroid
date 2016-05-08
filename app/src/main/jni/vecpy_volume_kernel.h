#ifndef VECPY_VOLUME_KERNEL_H
#define VECPY_VOLUME_KERNEL_H

//Kernel arguments
typedef struct {
    float* radius;
    float* volume;
    uint64_t N;
} KernelArgs;

void volume_scalar(KernelArgs* args);

void volume_vector(KernelArgs* args);

#endif

