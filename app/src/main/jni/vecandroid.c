#include <stdint.h>
#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cpu-features.h>
#include "vecandroid-intrinsics.h"

#define DEBUG 0

#if DEBUG
#include <android/log.h>
#  define  D(x...)  __android_log_print(ANDROID_LOG_INFO,"vecandroid",x)
#else
#  define  D(...)  do {} while (0)
#endif


/****************** Timer function ********************/

/* return current time in milliseconds */
static double
now_ms(void)
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

/***************** Logger functions *******************/

/*
 * This is the logger function for unary operations on each element in an array
 * array_type convention:
 * 0: int (default)
 * 1: float
 * 2: short
 * 3: uint32_t
 */
static void
unary_logger ( jstring * result, JNIEnv* env, jobject thiz,
         char *func_name, void (*myFunc_c)(), void (*myFunc_neon)(),
         void *array_o, void *array_i, void *array_e, int length, int array_type)
{
    char*  str;
    AndroidCpuFamily family;
    uint64_t features;
    char buffer[512];
    char tryNeon = 0;
    double  t0, t1, time_c, time_neon;
    int iterations = 1000;
    /* setup input */
    int *output = array_o;
    int *input = array_i;
    int *output_expected = array_e;
    if (array_type == 1) {
        float *output = array_o;
        float *input = array_i;
        float *output_expected = array_e;
    } else if (array_type == 2) {
        short *output = array_o;
        short *input = array_i;
        short *output_expected = array_e;
    }
    (*myFunc_c)(output_expected, input, length);

    /* Benchmark C version */
    t0 = now_ms();
    {
        int  count = iterations;
        for (; count > 0; count--) {
            (*myFunc_c)(output, input, length);
        }
    }
    t1 = now_ms();
    time_c = t1 - t0;

    asprintf(&str, "%s benchmark:\nC version          : %g ms\n", func_name, time_c);
    strlcpy(buffer, str, sizeof buffer);
    free(str);

    strlcat(buffer, "Neon version   : ", sizeof buffer);

    family = android_getCpuFamily();
    if ((family != ANDROID_CPU_FAMILY_ARM) &&
        (family != ANDROID_CPU_FAMILY_X86))
    {
        strlcat(buffer, "Not an ARM and not an X86 CPU !\n", sizeof buffer);
        goto EXIT;
    }

    features = android_getCpuFeatures();
    if (((features & ANDROID_CPU_ARM_FEATURE_ARMv7) == 0) &&
        ((features & ANDROID_CPU_X86_FEATURE_SSSE3) == 0))
    {
        strlcat(buffer, "Not an ARMv7 and not an X86 SSSE3 CPU !\n", sizeof buffer);
        goto EXIT;
    }

    /* HAVE_NEON is defined in Android.mk ! */
#ifdef HAVE_NEON
    if (((features & ANDROID_CPU_ARM_FEATURE_NEON) == 0) &&
        ((features & ANDROID_CPU_X86_FEATURE_SSSE3) == 0))
    {
        strlcat(buffer, "CPU doesn't support NEON !\n", sizeof buffer);
        goto EXIT;
    }

    /* Benchmark Neon version */
    t0 = now_ms();
    {
        int  count = iterations;
        for (; count > 0; count--) {
            (*myFunc_neon)(output, input, length);
        }
    }
    t1 = now_ms();
    time_neon = t1 - t0;
    asprintf(&str, "%g ms (x%g faster)\n", time_neon, time_c / (time_neon < 1e-6 ? 1. : time_neon));
    strlcat(buffer, str, sizeof buffer);
    free(str);

    /* check the result, just in case */
    {
        int  nn, fails = 0;
        for (nn = 0; nn < length; nn++) {
            if (output[nn] != output_expected[nn]) {
                if (++fails < 16)
                    D("neon[%d] = %d expected %d", nn, output[nn], output_expected[nn]);
            }
        }
        D("%d fails\n", fails);

        if (fails != 0) {
            if (length >= 80) {
                asprintf(&str, "Test failed. Error: %d out of %d.\n", fails, length);
                strlcat(buffer, str, sizeof buffer);
                free(str);
                goto EXIT;
            }
            strlcat(buffer, "Test failed.\n Input:   [", sizeof buffer);
            int i;
            for (i = 0; i < length; i++) {
                asprintf(&str, "%d, ", input[i]);
                strlcat(buffer, str, sizeof buffer);
                free(str);
            }
            strlcat(buffer, "] \n Output:   [", sizeof buffer);
            for (i = 0; i < length; i++) {
                asprintf(&str, "%d, ", output[i]);
                strlcat(buffer, str, sizeof buffer);
                free(str);
            }
            strlcat(buffer, "] \n Expected: [", sizeof buffer);
            for (i = 0; i < length; i++) {
                asprintf(&str, "%d, ", output_expected[i]);
                strlcat(buffer, str, sizeof buffer);
                free(str);
            }
            strlcat(buffer, "] \n", sizeof buffer);

        } else {
            strlcat(buffer, "Test passed!\n", sizeof buffer);
        }
    }
#else /* !HAVE_NEON */
    strlcat(buffer, "Program not compiled with ARMv7 support !\n", sizeof buffer);
#endif /* !HAVE_NEON */

    // free(output_expected);

EXIT:
    *result = (*env)->NewStringUTF(env, buffer);
}

/*
 * This is the logger function for binary operations on each element in an array
 * array_type convention:
 * 0: int (default)
 * 1: float
 * 2: short
 * 3: uint32_t
 */
static void
binary_logger ( jstring * result, JNIEnv* env, jobject thiz,
         char *func_name, void (*myFunc_c)(), void (*myFunc_neon)(),
         void *array_o, void *array_i1, void *array_i2, void *array_e, int length,
         int array_o_type, int array_i1_type, int array_i2_type)
{
    char*  str;
    AndroidCpuFamily family;
    uint64_t features;
    char buffer[512];
    char tryNeon = 0;
    double  t0, t1, time_c, time_neon;
    int iterations = 1000;
    /* setup input */
    int *output = array_o;
    int *input1 = array_i1;
    int *input2 = array_i2;
    int *output_expected = array_e;
    if (array_o_type == 1) {
        float *output = array_o;
        float *output_expected = array_e;
    } else if (array_o_type == 2) {
        short *output = array_o;
        short *output_expected = array_e;
    } else if (array_o_type == 3) {
        uint32_t *output = array_o;
        uint32_t *output_expected = array_e;
    }
    if (array_i1_type == 1) {
        float *input1 = array_i1;
    } else if (array_i1_type == 2) {
        short *input1 = array_i1;
    } else if (array_i1_type == 3) {
        uint32_t *input1 = array_i1;
    }
    if (array_i2_type == 1) {
        float *input2 = array_i2;
    } else if (array_i2_type == 2) {
        short *input2 = array_i2;
    } else if (array_i2_type == 3) {
        uint32_t *input2 = array_i2;
    }


    (*myFunc_c)(output_expected, input1, input2, length);

    /* Benchmark C version */
    t0 = now_ms();
    {
        int  count = iterations;
        for (; count > 0; count--) {
            (*myFunc_c)(output, input1, input2, length);
        }
    }
    t1 = now_ms();
    time_c = t1 - t0;

    asprintf(&str, "%s benchmark:\nC version          : %g ms\n", func_name, time_c);
    strlcpy(buffer, str, sizeof buffer);
    free(str);

    strlcat(buffer, "Neon version   : ", sizeof buffer);

    family = android_getCpuFamily();
    if ((family != ANDROID_CPU_FAMILY_ARM) &&
        (family != ANDROID_CPU_FAMILY_X86))
    {
        strlcat(buffer, "Not an ARM and not an X86 CPU !\n", sizeof buffer);
        goto EXIT;
    }

    features = android_getCpuFeatures();
    if (((features & ANDROID_CPU_ARM_FEATURE_ARMv7) == 0) &&
        ((features & ANDROID_CPU_X86_FEATURE_SSSE3) == 0))
    {
        strlcat(buffer, "Not an ARMv7 and not an X86 SSSE3 CPU !\n", sizeof buffer);
        goto EXIT;
    }

    /* HAVE_NEON is defined in Android.mk ! */
#ifdef HAVE_NEON
    if (((features & ANDROID_CPU_ARM_FEATURE_NEON) == 0) &&
        ((features & ANDROID_CPU_X86_FEATURE_SSSE3) == 0))
    {
        strlcat(buffer, "CPU doesn't support NEON !\n", sizeof buffer);
        goto EXIT;
    }

    /* Benchmark Neon version */
    t0 = now_ms();
    {
        int  count = iterations;
        for (; count > 0; count--) {
            (*myFunc_neon)(output, input1, input2, length);
        }
    }
    t1 = now_ms();
    time_neon = t1 - t0;
    asprintf(&str, "%g ms (x%g faster)\n", time_neon, time_c / (time_neon < 1e-6 ? 1. : time_neon));
    strlcat(buffer, str, sizeof buffer);
    free(str);

    /* check the result, just in case */
    {
        int  nn, fails = 0;
        for (nn = 0; nn < length; nn++) {
            if (output[nn] != output_expected[nn]) {
                if (++fails < 16)
                    D("neon[%d] = %d expected %d", nn, output[nn], output_expected[nn]);
            }
        }
        D("%d fails\n", fails);

        if (fails != 0) {
            if (length >= 80) {
                asprintf(&str, "Test failed. Error: %d out of %d.\n", fails, length);
                strlcat(buffer, str, sizeof buffer);
                free(str);
                goto EXIT;
            }
            strlcat(buffer, "Test failed.\n Output:   [", sizeof buffer);
            int i;
            for (i = 0; i < length; i++) {
                asprintf(&str, "%d, ", output[i]);
                strlcat(buffer, str, sizeof buffer);
                free(str);
            }
            strlcat(buffer, "] \n Expected: [", sizeof buffer);
            for (i = 0; i < length; i++) {
                asprintf(&str, "%d, ", output_expected[i]);
                strlcat(buffer, str, sizeof buffer);
                free(str);
            }
            strlcat(buffer, "] \n", sizeof buffer);

        } else {
            strlcat(buffer, "Test passed!\n", sizeof buffer);
        }
    }
#else /* !HAVE_NEON */
    strlcat(buffer, "Program not compiled with ARMv7 support !\n", sizeof buffer);
#endif /* !HAVE_NEON */

    // free(output_expected);

EXIT:
    *result = (*env)->NewStringUTF(env, buffer);
}


/***********************************************/


/**************************
 *  START OF SIMPLE TESTS *
 **************************/

/* This is the logger function */
static void
loggerTest(jstring *result, JNIEnv* env, jobject thiz, jint n1, jint n2 )
{

    char*  str;
    char buffer[512];
    asprintf(&str, "logger?\n Summation: %d + %d = %d", n1, n2, n1+n2);
    strlcpy(buffer, str, sizeof buffer);
    free(str);
    (*result) = (*env)->NewStringUTF(env, buffer);

}

jstring
Java_com_example_vecandroid_VecAndroid_myTest( JNIEnv* env, jobject thiz,
                                               jint n1, jint n2 )
{
    jstring result;
    loggerTest(&result, env, thiz, n1, n2);
    return result;
}


/*
 * This is the logger function for simple tests
 * array_type convention:
 * 0: int (default)
 * 1: float
 * 2: short
 * 3: uint32_t
 */
static void
logger ( jstring * result, JNIEnv* env, jobject thiz,
         char *func_name, void (*myFunc_c)(), void (*myFunc_neon)(),
         void *array_o, void *array_i, void *array_e, int length, int array_type)
{
    char*  str;
    AndroidCpuFamily family;
    uint64_t features;
    char buffer[512];
    char tryNeon = 0;
    double  t0, t1, time_c, time_neon;
    int iterations = 50;
    /* setup input */
    int *output = array_o;
    int *input = array_i;
    int *output_expected = array_e;
    if (array_type == 1) {
        float *output = array_o;
        float *input = array_i;
        float *output_expected = array_e;
    } else if (array_type == 2) {
        short *output = array_o;
        short *input = array_i;
        short *output_expected = array_e;
    }
    (*myFunc_c)(output_expected, input, length);

    /* Benchmark C version */
    t0 = now_ms();
    {
        int  count = iterations;
        for (; count > 0; count--) {
            (*myFunc_c)(output, input, length);
        }
    }
    t1 = now_ms();
    time_c = t1 - t0;

    asprintf(&str, "%s benchmark:\nC version          : %g ms\n", func_name, time_c);
    strlcpy(buffer, str, sizeof buffer);
    free(str);

    strlcat(buffer, "Neon version   : ", sizeof buffer);

    family = android_getCpuFamily();
    if ((family != ANDROID_CPU_FAMILY_ARM) &&
        (family != ANDROID_CPU_FAMILY_X86))
    {
        strlcat(buffer, "Not an ARM and not an X86 CPU !\n", sizeof buffer);
        goto EXIT;
    }

    features = android_getCpuFeatures();
    if (((features & ANDROID_CPU_ARM_FEATURE_ARMv7) == 0) &&
        ((features & ANDROID_CPU_X86_FEATURE_SSSE3) == 0))
    {
        strlcat(buffer, "Not an ARMv7 and not an X86 SSSE3 CPU !\n", sizeof buffer);
        goto EXIT;
    }

    /* HAVE_NEON is defined in Android.mk ! */
#ifdef HAVE_NEON
    if (((features & ANDROID_CPU_ARM_FEATURE_NEON) == 0) &&
        ((features & ANDROID_CPU_X86_FEATURE_SSSE3) == 0))
    {
        strlcat(buffer, "CPU doesn't support NEON !\n", sizeof buffer);
        goto EXIT;
    }

    /* Benchmark Neon version */
    t0 = now_ms();
    {
        int  count = iterations;
        for (; count > 0; count--) {
            (*myFunc_neon)(output, input, length);
        }
    }
    t1 = now_ms();
    time_neon = t1 - t0;
    asprintf(&str, "%g ms (x%g faster)\n", time_neon, time_c / (time_neon < 1e-6 ? 1. : time_neon));
    strlcat(buffer, str, sizeof buffer);
    free(str);

    /* check the result, just in case */
    {
        int  nn, fails = 0;
        for (nn = 0; nn < length; nn++) {
            if (output[nn] != output_expected[nn]) {
                if (++fails < 16)
                    D("neon[%d] = %d expected %d", nn, output[nn], output_expected[nn]);
            }
        }
        D("%d fails\n", fails);

        if (fails != 0) {
            if (length >= 80) {
                asprintf(&str, "Test failed. Error: %d out of %d.\n", fails, length);
                strlcat(buffer, str, sizeof buffer);
                free(str);
                goto EXIT;
            }
            strlcat(buffer, "Test failed.\n Output:   [", sizeof buffer);
            int i;
            for (i = 0; i < length; i++) {
                asprintf(&str, "%d, ", output[i]);
                strlcat(buffer, str, sizeof buffer);
                free(str);
            }
            strlcat(buffer, "] \n Expected: [", sizeof buffer);
            for (i = 0; i < length; i++) {
                asprintf(&str, "%d, ", output_expected[i]);
                strlcat(buffer, str, sizeof buffer);
                free(str);
            }
            strlcat(buffer, "] \n", sizeof buffer);

        } else {
            strlcat(buffer, "Test passed!\n", sizeof buffer);
        }


    }


#else /* !HAVE_NEON */
    strlcat(buffer, "Program not compiled with ARMv7 support !\n", sizeof buffer);
#endif /* !HAVE_NEON */

    // free(output_expected);

EXIT:
    *result = (*env)->NewStringUTF(env, buffer);
}

/***************************************/

/* this is the abs implementation in C for integers */
static void
intAbsSerial(int *output, int *input, int N)
{
    int i;
    int x;
    for (i = 0; i < N; i++) {
        x = input[i];
        if (x < 0) {
            output[i] = -x;
        } else {
            output[i] = x;
        }
    }
}

/* this is the abs implementation in C for floating points */
static void
floatAbsSerial(float *output, float *input, int N)
{
    int i;
    float x;
    for (i = 0; i < N; i++) {
        x = input[i];
        if (x < 0.f) {
            output[i] = -x;
        } else {
            output[i] = x;
        }
    }
}

/* this is the absolute value function for integers */
jstring
Java_com_vecandroid_api_MathOps_intAbsBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Absolute value (integers)");
    int length = 1027;
    int output[length];
    int output_expected[length];
    int input[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input[i] = (int) (-50 + (rand() % 100));
        output[i] = (int) 0;
    }
    int array_type = 0; // int
    unary_logger(&result, env, thiz, str, &intAbsSerial, &intAbsVector, output, input, output_expected, length, array_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}

/* ------------------------------------- New API ----------------------------------------- */

/* this is the absolute value function for integers */
jintArray
Java_com_vecandroid_api_MathOps_intAbsNative( JNIEnv* env,
                                               jobject thiz, jintArray input)
{
    jsize length = (*env)->GetArrayLength(env, input);
    jint *input1 = (*env)->GetIntArrayElements(env, input, 0);
    jintArray result = (*env)->NewIntArray(env, length);
    jint output[length];

    intAbsVector(output, input1, length);

    (*env)->SetIntArrayRegion(env, result, 0, length, output);
    return result;
}


/* this is the absolute value function for floating points */
jfloatArray
Java_com_vecandroid_api_MathOps_floatAbsNative( JNIEnv* env,
                                               jobject thiz, jfloatArray input)
{
    jsize length = (*env)->GetArrayLength(env, input);
    jfloat *input1 = (*env)->GetFloatArrayElements(env, input, 0);
    jfloatArray result = (*env)->NewFloatArray(env, length);
    jfloat output[length];

    floatAbsVector(output, input1, length);

    (*env)->SetFloatArrayRegion(env, result, 0, length, output);
    return result;
}


jintArray
Java_com_vecandroid_api_MathOps_intAddNative( JNIEnv* env,
                                               jobject thiz, jintArray input1, jintArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jint *input3 = (*env)->GetIntArrayElements(env, input1, 0);
    jint *input4 = (*env)->GetIntArrayElements(env, input2, 0);

    jintArray result = (*env)->NewIntArray(env, length);
    jint output[length];

    intAddVector(output, input3, input4, length);

    (*env)->SetIntArrayRegion(env, result, 0, length, output);
    return result;
}

jfloatArray
Java_com_vecandroid_api_MathOps_floatAddNative( JNIEnv* env,
                                               jobject thiz, jfloatArray input1, jfloatArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jfloat *input3 = (*env)->GetFloatArrayElements(env, input1, 0);
    jfloat *input4 = (*env)->GetFloatArrayElements(env, input2, 0);

    jfloatArray result = (*env)->NewFloatArray(env, length);
    jfloat output[length];

    floatAddVector(output, input3, input4, length);

    (*env)->SetFloatArrayRegion(env, result, 0, length, output);
    return result;
}

jintArray
Java_com_vecandroid_api_MathOps_intSubNative( JNIEnv* env,
                                               jobject thiz, jintArray input1, jintArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jint *input3 = (*env)->GetIntArrayElements(env, input1, 0);
    jint *input4 = (*env)->GetIntArrayElements(env, input2, 0);

    jintArray result = (*env)->NewIntArray(env, length);
    jint output[length];

    intSubVector(output, input3, input4, length);

    (*env)->SetIntArrayRegion(env, result, 0, length, output);
    return result;
}

jfloatArray
Java_com_vecandroid_api_MathOps_floatSubNative( JNIEnv* env,
                                               jobject thiz, jfloatArray input1, jfloatArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jfloat *input3 = (*env)->GetFloatArrayElements(env, input1, 0);
    jfloat *input4 = (*env)->GetFloatArrayElements(env, input2, 0);

    jfloatArray result = (*env)->NewFloatArray(env, length);
    jfloat output[length];

    floatSubVector(output, input3, input4, length);

    (*env)->SetFloatArrayRegion(env, result, 0, length, output);
    return result;
}

jintArray
Java_com_vecandroid_api_MathOps_intMulNative( JNIEnv* env,
                                               jobject thiz, jintArray input1, jintArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jint *input3 = (*env)->GetIntArrayElements(env, input1, 0);
    jint *input4 = (*env)->GetIntArrayElements(env, input2, 0);

    jintArray result = (*env)->NewIntArray(env, length);
    jint output[length];

    intMulVector(output, input3, input4, length);

    (*env)->SetIntArrayRegion(env, result, 0, length, output);
    return result;
}

jfloatArray
Java_com_vecandroid_api_MathOps_floatMulNative( JNIEnv* env,
                                               jobject thiz, jfloatArray input1, jfloatArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jfloat *input3 = (*env)->GetFloatArrayElements(env, input1, 0);
    jfloat *input4 = (*env)->GetFloatArrayElements(env, input2, 0);

    jfloatArray result = (*env)->NewFloatArray(env, length);
    jfloat output[length];

    floatMulVector(output, input3, input4, length);

    (*env)->SetFloatArrayRegion(env, result, 0, length, output);
    return result;
}

jintArray
Java_com_vecandroid_api_MathOps_intEqNative( JNIEnv* env,
                                               jobject thiz, jintArray input1, jintArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jint *input3 = (*env)->GetIntArrayElements(env, input1, 0);
    jint *input4 = (*env)->GetIntArrayElements(env, input2, 0);

    jintArray result = (*env)->NewIntArray(env, length);
    jint output[length];

    intEqVector(output, input3, input4, length);

    (*env)->SetIntArrayRegion(env, result, 0, length, output);
    return result;
}

jintArray
Java_com_vecandroid_api_MathOps_floatEqNative( JNIEnv* env,
                                               jobject thiz, jfloatArray input1, jfloatArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jfloat *input3 = (*env)->GetFloatArrayElements(env, input1, 0);
    jfloat *input4 = (*env)->GetFloatArrayElements(env, input2, 0);

    jintArray result = (*env)->NewIntArray(env, length);
    jint output[length];

    floatEqVector(output, input3, input4, length);

    (*env)->SetIntArrayRegion(env, result, 0, length, output);
    return result;
}

jintArray
Java_com_vecandroid_api_MathOps_intGeNative( JNIEnv* env,
                                               jobject thiz, jintArray input1, jintArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jint *input3 = (*env)->GetIntArrayElements(env, input1, 0);
    jint *input4 = (*env)->GetIntArrayElements(env, input2, 0);

    jintArray result = (*env)->NewIntArray(env, length);
    jint output[length];

    intGeVector(output, input3, input4, length);

    (*env)->SetIntArrayRegion(env, result, 0, length, output);
    return result;
}

jintArray
Java_com_vecandroid_api_MathOps_floatGeNative( JNIEnv* env,
                                               jobject thiz, jfloatArray input1, jfloatArray input2)
{
    jsize length = (*env)->GetArrayLength(env, input1);
    jfloat *input3 = (*env)->GetFloatArrayElements(env, input1, 0);
    jfloat *input4 = (*env)->GetFloatArrayElements(env, input2, 0);

    jintArray result = (*env)->NewIntArray(env, length);
    jint output[length];

    floatGeVector(output, input3, input4, length);

    (*env)->SetIntArrayRegion(env, result, 0, length, output);
    return result;
}

/* ------------------------------------- End ----------------------------------------- */

/* this is the absolute value function for floating points */
jstring
Java_com_example_vecandroid_VecAndroid_floatAbsBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Absolute value (floating points)");
    int length = 7;
    float output[length];
    float output_expected[length];
    float input[length];

    int i;
    // srand(time(NULL));
    for (i = 0; i < length; i++) {
        // input[i] = (int) (-5 + (rand() % 10));
        input[i] = -1.f + 4.f * (float)(rand()) / RAND_MAX;
        output[i] = 0.f;
    }
    int array_type = 1; // float
    unary_logger(&result, env, thiz, str, &floatAbsSerial, &floatAbsVector, output, input, output_expected, length, array_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/***********************
 *  END OF SIMPLE TESTS *
 ***********************/

/***********************************************/

/****************************************
 *  START OF API (benchmarking section) *
 ****************************************/

/***************** Arithmetic ******************/

/* this is the entry-wise add implementation in C for integers */
static void
intAddSerial(int *output, int *input1, int *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = input1[i] + input2[i];
    }
}

/* this is the entry-wise add operation for integers */
jstring
Java_com_example_vecandroid_VecAndroid_intAddBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise add (integers)");
    int length = 1027;
    int output[length];
    int output_expected[length];
    int input1[length];
    int input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = (int) (-50 + (rand() % 100));
        input2[i] = (int) (-50 + (rand() % 100));
        // input[i] = (short) 1;
        output[i] = (int) 0;
    }
    int array_o_type = 0; // int
    int array_i1_type = 0; // int
    int array_i2_type = 0; // int
    binary_logger (&result, env, thiz, str, &intAddSerial, &intAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/* this is the entry-wise add implementation in C for floating points */
static void
floatAddSerial(float *output, float *input1, float *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = input1[i] + input2[i];
    }
}

/* this is the entry-wise add operation for floating points */
jstring
Java_com_example_vecandroid_VecAndroid_floatAddBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise add (floating points)");
    int length = 1027;
    float output[length];
    float output_expected[length];
    float input1[length];
    float input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        input2[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        output[i] = 0.f;
    }
    int array_o_type = 1; // float
    int array_i1_type = 1; // float
    int array_i2_type = 1; // float
    binary_logger (&result, env, thiz, str, &floatAddSerial, &floatAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}

/* this is the entry-wise subtract implementation in C for integers */
static void
intSubSerial(int *output, int *input1, int *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = input1[i] - input2[i];
    }
}

/* this is the entry-wise subtract operation for integers */
jstring
Java_com_example_vecandroid_VecAndroid_intSubBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise subtract (integers)");
    int length = 1027;
    int output[length];
    int output_expected[length];
    int input1[length];
    int input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = (int) (-50 + (rand() % 100));
        input2[i] = (int) (-50 + (rand() % 100));
        // input[i] = (short) 1;
        output[i] = (int) 0;
    }
    int array_o_type = 0; // int
    int array_i1_type = 0; // int
    int array_i2_type = 0; // int
    binary_logger (&result, env, thiz, str, &intAddSerial, &intAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/* this is the entry-wise subtract implementation in C for floating points */
static void
floatSubSerial(float *output, float *input1, float *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = input1[i] - input2[i];
    }
}

/* this is the entry-wise subtract operation for floating points */
jstring
Java_com_example_vecandroid_VecAndroid_floatSubBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise subtract (floating points)");
    int length = 1027;
    float output[length];
    float output_expected[length];
    float input1[length];
    float input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        input2[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        output[i] = 0.f;
    }
    int array_o_type = 1; // float
    int array_i1_type = 1; // float
    int array_i2_type = 1; // float
    binary_logger (&result, env, thiz, str, &floatAddSerial, &floatAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}

/* this is the entry-wise multiply implementation in C for integers */
static void
intMulSerial(int *output, int *input1, int *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = input1[i] * input2[i];
    }
}

/* this is the entry-wise multiply operation for integers */
jstring
Java_com_example_vecandroid_VecAndroid_intMulBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise multiply (integers)");
    int length = 1027;
    int output[length];
    int output_expected[length];
    int input1[length];
    int input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = (int) (-50 + (rand() % 100));
        input2[i] = (int) (-50 + (rand() % 100));
        // input[i] = (short) 1;
        output[i] = (int) 0;
    }
    int array_o_type = 0; // int
    int array_i1_type = 0; // int
    int array_i2_type = 0; // int
    binary_logger (&result, env, thiz, str, &intAddSerial, &intAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/* this is the entry-wise multiply implementation in C for floating points */
static void
floatMulSerial(float *output, float *input1, float *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = input1[i] * input2[i];
    }
}

/* this is the entry-wise multiply operation for floating points */
jstring
Java_com_example_vecandroid_VecAndroid_floatMulBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise multiply (floating points)");
    int length = 1027;
    float output[length];
    float output_expected[length];
    float input1[length];
    float input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        input2[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        output[i] = 0.f;
    }
    int array_o_type = 1; // float
    int array_i1_type = 1; // float
    int array_i2_type = 1; // float
    binary_logger (&result, env, thiz, str, &floatAddSerial, &floatAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/***************** Comparison ******************/


/* this is the entry-wise comparison (equal) implementation in C for integers */
static void
intEqSerial(uint32_t *output, int *input1, int *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = (input1[i] == input2[i])? (uint32_t)1 : (uint32_t)0;
    }
}

/* this is the entry-wise comparison (equal) operation for integers */
jstring
Java_com_example_vecandroid_VecAndroid_intEqBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise comparison (equal) on integers");
    int length = 1027;
    uint32_t output[length];
    uint32_t output_expected[length];
    int input1[length];
    int input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = (int) (-50 + (rand() % 100));
        input2[i] = (int) (-50 + (rand() % 100));
        // input[i] = (short) 1;
        output[i] = (uint32_t) 0;
    }
    int array_o_type = 3; // uint32_t
    int array_i1_type = 0; // int
    int array_i2_type = 0; // int
    binary_logger (&result, env, thiz, str, &intAddSerial, &intAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/* this is the entry-wise comparison (equal) implementation in C for floating points */
static void
floatEqSerial(uint32_t *output, float *input1, float *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = (input1[i] == input2[i])? (uint32_t)1 : (uint32_t)0;
    }
}

/* this is the entry-wise comparison (equal) operation for floating points */
jstring
Java_com_example_vecandroid_VecAndroid_floatEqBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise comparison (equal) on floating points");
    int length = 1024; // power of 4
    uint32_t output[length];
    uint32_t output_expected[length];
    float input1[length];
    float input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        input2[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        output[i] = (uint32_t) 0;
    }
    int array_o_type = 3; // uint32_t
    int array_i1_type = 1; // float
    int array_i2_type = 1; // float
    binary_logger (&result, env, thiz, str, &floatAddSerial, &floatAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/* this is the entry-wise comparison (greater than or equal to) implementation in C for integers */
static void
intGeSerial(uint32_t *output, int *input1, int *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = (input1[i] == input2[i])? (uint32_t)1 : (uint32_t)0;
    }
}

/* this is the entry-wise comparison (greater than or equal to) operation for integers */
jstring
Java_com_example_vecandroid_VecAndroid_intGeBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise comparison (greater than or equal to) on integers");
    int length = 1024; // power of 4
    uint32_t output[length];
    uint32_t output_expected[length];
    int input1[length];
    int input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = (int) (-50 + (rand() % 100));
        input2[i] = (int) (-50 + (rand() % 100));
        // input[i] = (short) 1;
        output[i] = (uint32_t) 0;
    }
    int array_o_type = 3; // uint32_t
    int array_i1_type = 0; // int
    int array_i2_type = 0; // int
    binary_logger (&result, env, thiz, str, &intAddSerial, &intAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/* this is the entry-wise comparison (greater than or equal to) implementation in C for floating points */
static void
floatGeSerial(uint32_t *output, float *input1, float *input2, int N)
{
    int i;
    for (i = 0; i < N; i++) {
        output[i] = (input1[i] == input2[i])? (uint32_t)1 : (uint32_t)0;
    }
}

/* this is the entry-wise comparison (greater than or equal to) operation for floating points */
jstring
Java_com_example_vecandroid_VecAndroid_floatGeBench( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Entry-wise comparison (greater than or equal to) on floating points");
    int length = 1024; // power of 4
    uint32_t output[length];
    uint32_t output_expected[length];
    float input1[length];
    float input2[length];

    int i;
    srand(time(NULL));
    for (i = 0; i < length; i++) {
        input1[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        input2[i] = -10.f + 40.f * (float)(rand()) / RAND_MAX;
        output[i] = (uint32_t) 0;
    }
    int array_o_type = 0; // int
    int array_i1_type = 1; // float
    int array_i2_type = 1; // float
    binary_logger (&result, env, thiz, str, &floatAddSerial, &floatAddVector,
                   output, input1, input2, output_expected, length,
                   array_o_type, array_i1_type, array_i2_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}


/**************************************
 *  END OF API (benchmarking section) *
 **************************************/

/***********************************************/

/* this is the serial testings */
static void
testingSerial(int *output, int *input, int N)
{
    int i;
    int x;
    for (i = 0; i < N; i++) {
        x = input[i];
        if (x < 0) {
            output[i] = -x;
        } else {
            output[i] = x;
        }
    }
}

/* this is the testing function */
jstring
Java_com_example_vecandroid_VecAndroid_testings( JNIEnv* env,
                                               jobject thiz )
{
    jstring result;
    char *str;
    asprintf(&str, "Testings: Absolute value (integers)");
    int length = 1001; // power of 4
    int output[length];
    int output_expected[length];
    int input[length];

    int i;
    for (i = 0; i < length; i++) {
        input[i] = (int) (-5 + (rand() % 10));
        // input[i] = (short) 1;
        output[i] = (int) 0;
    }
    int array_type = 0; // int
    logger(&result, env, thiz, str, &testingSerial, &testingVector, output, input, output_expected, length, array_type);
    // loggerTest(&result, env, thiz, 2, 3);
    // free(str);
    return result;
}
