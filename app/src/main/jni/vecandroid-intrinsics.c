#include <stdint.h>
#include "vecandroid-intrinsics.h"
#include <arm_neon.h>

/* this source file should only be compiled by Android.mk when targeting
 * the armeabi-v7a ABI, and should be built in NEON mode
 */
void
fir_filter_neon_intrinsics(short *output, const short* input, const short* kernel, int width, int kernelSize)
{
#if 1
   int nn, offset = -kernelSize/2;

   for (nn = 0; nn < width; nn++)
   {
        int mm, sum = 0;
        int32x4_t sum_vec = vdupq_n_s32(0);
        for(mm = 0; mm < kernelSize/4; mm++)
        {
            int16x4_t  kernel_vec = vld1_s16(kernel + mm*4);
            int16x4_t  input_vec = vld1_s16(input + (nn+offset+mm*4));
            sum_vec = vmlal_s16(sum_vec, kernel_vec, input_vec);
        }

        sum += vgetq_lane_s32(sum_vec, 0);
        sum += vgetq_lane_s32(sum_vec, 1);
        sum += vgetq_lane_s32(sum_vec, 2);
        sum += vgetq_lane_s32(sum_vec, 3);

        if(kernelSize & 3)
        {
            for(mm = kernelSize - (kernelSize & 3); mm < kernelSize; mm++)
                sum += kernel[mm] * input[nn+offset+mm];
        }

        output[nn] = (short)((sum + 0x8000) >> 16);
    }
#else /* for comparison purposes only */
    int nn, offset = -kernelSize/2;
    for (nn = 0; nn < width; nn++) {
        int sum = 0;
        int mm;
        for (mm = 0; mm < kernelSize; mm++) {
            sum += kernel[mm]*input[nn+offset+mm];
        }
        output[n] = (short)((sum + 0x8000) >> 16);
    }
#endif
}

/* Absolute value implementation (integers) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
intAbsVector(int *output, int *input, int N)
{
    int vector_width = 4;
    int i;
    int32x4_t abs_vec;
    for (i = 0; i < N; i += vector_width){
        int32x4_t input_vec = vld1q_s32(input + i);
        abs_vec = vabsq_s32(input_vec);
        vst1q_s32(output + i, abs_vec);
    }
}

/* Absolute value implementation (floating points) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
floatAbsVector(float *output, float *input, int N)
{
    int vector_width = 4;
    int i;
    float32x4_t abs_vec;
    for (i = 0; i < N; i += vector_width){
        float32x4_t input_vec = vld1q_f32(input + i);
        abs_vec = vabsq_f32(input_vec);
        vst1q_f32(output + i, abs_vec);
    }
}

/* Add implementation (integers) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
intAddVector(int *output, int *input1, int *input2, int N)
{
    int vector_width = 4;
    int i;
    int32x4_t add_vec;
    for (i = 0; i < N; i += vector_width){
        int32x4_t input1_vec = vld1q_s32(input1 + i);
        int32x4_t input2_vec = vld1q_s32(input2 + i);
        add_vec = vaddq_s32(input1_vec, input2_vec);
        vst1q_s32(output + i, add_vec);
    }
}

/* Add implementation (floating points) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
floatAddVector(float *output, float *input1, float *input2, int N)
{
    int vector_width = 4;
    int i;
    float32x4_t add_vec;
    for (i = 0; i < N; i += vector_width){
        float32x4_t input1_vec = vld1q_f32(input1 + i);
        float32x4_t input2_vec = vld1q_f32(input2 + i);
        add_vec = vaddq_f32(input1_vec, input2_vec);
        vst1q_f32(output + i, add_vec);
    }
}

/* Subtract implementation (integers) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
intSubVector(int *output, int *input1, int *input2, int N)
{
    int vector_width = 4;
    int i;
    int32x4_t add_vec;
    for (i = 0; i < N; i += vector_width){
        int32x4_t input1_vec = vld1q_s32(input1 + i);
        int32x4_t input2_vec = vld1q_s32(input2 + i);
        add_vec = vsubq_s32(input1_vec, input2_vec);
        vst1q_s32(output + i, add_vec);
    }
}

/* Subtract implementation (floating points) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
floatSubVector(float *output, float *input1, float *input2, int N)
{
    int vector_width = 4;
    int i;
    float32x4_t add_vec;
    for (i = 0; i < N; i += vector_width){
        float32x4_t input1_vec = vld1q_f32(input1 + i);
        float32x4_t input2_vec = vld1q_f32(input2 + i);
        add_vec = vsubq_f32(input1_vec, input2_vec);
        vst1q_f32(output + i, add_vec);
    }
}

/* Multiply implementation (integers) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
intMulVector(int *output, int *input1, int *input2, int N)
{
    int vector_width = 4;
    int i;
    int32x4_t add_vec;
    for (i = 0; i < N; i += vector_width){
        int32x4_t input1_vec = vld1q_s32(input1 + i);
        int32x4_t input2_vec = vld1q_s32(input2 + i);
        add_vec = vmulq_s32(input1_vec, input2_vec);
        vst1q_s32(output + i, add_vec);
    }
}

/* Multiply implementation (floating points) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
floatMulVector(float *output, float *input1, float *input2, int N)
{
    int vector_width = 4;
    int i;
    float32x4_t add_vec;
    for (i = 0; i < N; i += vector_width){
        float32x4_t input1_vec = vld1q_f32(input1 + i);
        float32x4_t input2_vec = vld1q_f32(input2 + i);
        add_vec = vmulq_f32(input1_vec, input2_vec);
        vst1q_f32(output + i, add_vec);
    }
}

/* Comparison (equality) implementation (integers) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
intEqVector(uint32_t *output, int *input1, int *input2, int N)
{
    int vector_width = 4;
    int i;
    uint32x4_t eq_vec;
    for (i = 0; i < N; i += vector_width){
        int32x4_t input1_vec = vld1q_s32(input1 + i);
        int32x4_t input2_vec = vld1q_s32(input2 + i);
        eq_vec = vceqq_s32(input1_vec, input2_vec);
        vst1q_u32(output + i, eq_vec);
    }
}

/* Comparison (equality) implementation (floating points) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
floatEqVector(uint32_t *output, float *input1, float *input2, int N)
{
    int vector_width = 4;
    int i;
    uint32x4_t eq_vec;
    for (i = 0; i < N; i += vector_width){
        float32x4_t input1_vec = vld1q_f32(input1 + i);
        float32x4_t input2_vec = vld1q_f32(input2 + i);
        eq_vec = vceqq_f32(input1_vec, input2_vec);
        vst1q_u32(output + i, eq_vec);
    }
}

/* Comparison (equality) implementation (integers) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
intGeVector(uint32_t *output, int *input1, int *input2, int N)
{
    int vector_width = 4;
    int i;
    uint32x4_t eq_vec;
    for (i = 0; i < N; i += vector_width){
        int32x4_t input1_vec = vld1q_s32(input1 + i);
        int32x4_t input2_vec = vld1q_s32(input2 + i);
        eq_vec = vcgeq_s32(input1_vec, input2_vec);
        vst1q_u32(output + i, eq_vec);
    }
}

/* Comparison (equality) implementation (floating points) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
floatGeVector(uint32_t *output, float *input1, float *input2, int N)
{
    int vector_width = 4;
    int i;
    uint32x4_t eq_vec;
    for (i = 0; i < N; i += vector_width){
        float32x4_t input1_vec = vld1q_f32(input1 + i);
        float32x4_t input2_vec = vld1q_f32(input2 + i);
        eq_vec = vcgeq_f32(input1_vec, input2_vec);
        vst1q_u32(output + i, eq_vec);
    }
}
