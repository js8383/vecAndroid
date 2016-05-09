/*******************************************************************************
*                           Generated kernel: saxpy                            *
*******************************************************************************/
//Integer types
#include <stdint.h>
#include "neoncompile_saxpy_kernel.h"

/*******************************************************************************
*                      Target Architecture: NEON (float)                       *
*******************************************************************************/
//Includes
#include <math.h>
//#include <algorithm>

//Kernel function: saxpy
void saxpy_scalar(KernelArgs* args) {

    //Uniforms
    
    //Fuses
    
    //Literals
    
    //Stack variables (numeric)
    float scale, x, y, result, var060, var061;
    
    //Stack variables (boolean)
    
    //Loop over input
    uint64_t index;
    for(index = 0; index < args->N; ++index) {
    
        //Inputs
        scale = args->scale[index];
        x = args->x[index];
        y = args->y[index];
        
        //Begin kernel logic
        {
        
            //>>> result = scale * x + y
            var061 = scale * x;
            var060 = var061 + y;
            result = var060;
        
        }
        //End kernel logic
        
        //Outputs
        args->result[index] = result;
        
    }
}
//End of kernel function


/*******************************************************************************
*                      Target Architecture: NEON (float)                       *
*******************************************************************************/
//Includes
#include <stdint.h>
#include <math.h>
#include <arm_neon.h>
#include "neon_more.h"

//Kernel function: saxpy
void saxpy_vector(KernelArgs* args) {

    //Setup
    const float32x4_t MASK_FALSE = vdupq_n_f32(0.f);
    const float32x4_t MASK_TRUE = vcvtq_f32_u32(vceqq_f32(MASK_FALSE, MASK_FALSE));
    
    //Uniforms
    
    //Fuses
    
    //Literals
    
    //Stack variables
    float32x4_t scale, x, y, result, var060, var061;
    
    //Loop over input
    uint64_t index;
    for(index = 0; index < args->N; index += 4) {
    
        //Inputs
        scale = vld1q_f32(&args->scale[index]);
        x = vld1q_f32(&args->x[index]);
        y = vld1q_f32(&args->y[index]);
        
        //Begin kernel logic
        {
        
            //>>> result = scale * x + y
            var061 = vmulq_f32(scale, x);
            var060 = vaddq_f32(var061, y);
            result = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), var060, result);
        
        }
        //End kernel logic
        
        //Outputs
        vst1q_f32(&args->result[index], result);
        
    }
}
//End of kernel function


