#ifndef VECANDROID_INTRINSICS_H
#define VECANDROID_INTRINSICS_H

/* Just for testing */
void testingVector(int *output, int *input, int N);

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

void intLeVector(uint32_t *output, int *input1, int *input2, int N);

void floatLeVector(uint32_t *output, float *input1, float *input2, int N);

void intGtVector(uint32_t *output, int *input1, int *input2, int N);

void floatGtVector(uint32_t *output, float *input1, float *input2, int N);

void intLtVector(uint32_t *output, int *input1, int *input2, int N);

void floatLtVector(uint32_t *output, float *input1, float *input2, int N);

#endif /* VECANDROID_INTRINSICS_H */
