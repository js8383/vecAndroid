#ifndef VECANDROID_INTRINSICS_H
#define VECANDROID_INTRINSICS_H

void fir_filter_neon_intrinsics(short *output, const short* input, const short* kernel, int width, int kernelSize);

void intAbsVector(int *output, int *input, int N);

void floatAbsVector(float *output, float *input, int N);

void intAddVector(int *output, int *input1, int *input2, int N);

void floatAddVector(float *output, float *input1, float *input2, int N);

void intSubVector(int *output, int *input1, int *input2, int N);

void floatSubVector(float *output, float *input1, float *input2, int N);

void intMulVector(int *output, int *input1, int *input2, int N);

void floatMulVector(float *output, float *input1, float *input2, int N);

void intEqVector(uint32_t *output, int *input1, int *input2, int N);

void floatEqVector(uint32_t *output, float *input1, float *input2, int N);

void intGeVector(uint32_t *output, int *input1, int *input2, int N);

void floatGeVector(uint32_t *output, float *input1, float *input2, int N);

#endif /* VECANDROID_INTRINSICS_H */
