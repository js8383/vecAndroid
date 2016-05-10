/*******************************************************************************
*                       Generated kernel header: volume                        *
*******************************************************************************/

#ifndef VECANDROID_VOLUME_KERNEL_H
#define VECANDROID_VOLUME_KERNEL_H

//Kernel arguments
typedef struct {
    float* radius;
    float* volume;
    uint64_t N;
} KernelArgs;

void volume_scalar(KernelArgs* args);

void volume_vector(KernelArgs* args);

#endif
