#include <stdint.h>
#include "vecandroid-intrinsics.h"
#include <arm_neon.h>

/* this source file should only be compiled by Android.mk when targeting
 * the armeabi-v7a ABI, and should be built in NEON mode
 */


/* Absolute value implementation (integers) in NEON intrinsics
 * Requires N to be a power of 4.
 */
void
intAbsVector(int *output, int *input, int N)
{
    int vector_width = 4;
    int i;
    int32x4_t abs_vec;
    for (i = 0; i < N & ~3; i += vector_width){
        int32x4_t input_vec = vld1q_s32(input + i);
        abs_vec = vabsq_s32(input_vec);
        vst1q_s32(output + i, abs_vec);
    }
    if (i & 2) {
        vst1_s32(output + i, vabs_s32(vld1_s32(input + i)));
        i += 2;
    }
    if (i & 1) {
        int x = input[i];
        output[i] = (x >= 0)? x : -x;
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
    for (i = 0; i < N & ~3; i += vector_width){
        float32x4_t input_vec = vld1q_f32(input + i);
        abs_vec = vabsq_f32(input_vec);
        vst1q_f32(output + i, abs_vec);
    }
    if (i & 2) {
        vst1_f32(output + i, vabs_f32(vld1_f32(input + i)));
        i += 2;
    }
    if (i & 1) {
        float x = input[i];
        output[i] = (x >= 0.f)? x : -x;
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
    for (i = 0; i < N & ~3; i += vector_width){
        int32x4_t input1_vec = vld1q_s32(input1 + i);
        int32x4_t input2_vec = vld1q_s32(input2 + i);
        add_vec = vaddq_s32(input1_vec, input2_vec);
        vst1q_s32(output + i, add_vec);
    }
    if (i & 2) {
        vst1_s32(output + i, vadd_s32(vld1_s32(input1 + i), vld1_s32(input2 + i)));
        i += 2;
    }
    if (i & 1) {
        output[i] = input1[i] + input2[i];
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
    for (i = 0; i < N & ~3; i += vector_width){
        float32x4_t input1_vec = vld1q_f32(input1 + i);
        float32x4_t input2_vec = vld1q_f32(input2 + i);
        add_vec = vaddq_f32(input1_vec, input2_vec);
        vst1q_f32(output + i, add_vec);
    }
    if (i & 2) {
        vst1_f32(output + i, vadd_f32(vld1_f32(input1 + i), vld1_f32(input2 + i)));
        i += 2;
    }
    if (i & 1) {
        output[i] = input1[i] + input2[i];
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
    int32x4_t sub_vec;
    for (i = 0; i < N & ~3; i += vector_width){
        int32x4_t input1_vec = vld1q_s32(input1 + i);
        int32x4_t input2_vec = vld1q_s32(input2 + i);
        sub_vec = vsubq_s32(input1_vec, input2_vec);
        vst1q_s32(output + i, sub_vec);
    }
    if (i & 2) {
        vst1_s32(output + i, vsub_s32(vld1_s32(input1 + i), vld1_s32(input2 + i)));
        i += 2;
    }
    if (i & 1) {
        output[i] = input1[i] - input2[i];
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
    float32x4_t sub_vec;
    for (i = 0; i < N & ~3; i += vector_width){
        float32x4_t input1_vec = vld1q_f32(input1 + i);
        float32x4_t input2_vec = vld1q_f32(input2 + i);
        sub_vec = vsubq_f32(input1_vec, input2_vec);
        vst1q_f32(output + i, sub_vec);
    }
    if (i & 2) {
        vst1_f32(output + i, vsub_f32(vld1_f32(input1 + i), vld1_f32(input2 + i)));
        i += 2;
    }
    if (i & 1) {
        output[i] = input1[i] - input2[i];
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
    int32x4_t mul_vec;
    for (i = 0; i < N & ~3; i += vector_width){
        int32x4_t input1_vec = vld1q_s32(input1 + i);
        int32x4_t input2_vec = vld1q_s32(input2 + i);
        mul_vec = vmulq_s32(input1_vec, input2_vec);
        vst1q_s32(output + i, mul_vec);
    }
    if (i & 2) {
        vst1_s32(output + i, vmul_s32(vld1_s32(input1 + i), vld1_s32(input2 + i)));
        i += 2;
    }
    if (i & 1) {
        output[i] = input1[i] * input2[i];
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
    float32x4_t mul_vec;
    for (i = 0; i < N & ~3; i += vector_width){
        float32x4_t input1_vec = vld1q_f32(input1 + i);
        float32x4_t input2_vec = vld1q_f32(input2 + i);
        mul_vec = vmulq_f32(input1_vec, input2_vec);
        vst1q_f32(output + i, mul_vec);
    }
    if (i & 2) {
        vst1_f32(output + i, vmul_f32(vld1_f32(input1 + i), vld1_f32(input2 + i)));
        i += 2;
    }
    if (i & 1) {
        output[i] = input1[i] * input2[i];
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
