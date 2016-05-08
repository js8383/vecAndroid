/*******************************************************************************
*                        VecPy generated kernel: volume                        *
*******************************************************************************/
//Integer types
#include <stdint.h>
#include "vecpy_volume_kernel.h"

/*******************************************************************************
*                      Target Architecture: NEON (float)                       *
*******************************************************************************/
//Includes
#include <math.h>
// #include <algorithm>
//Kernel function: volume
void volume_scalar(KernelArgs* args) {

    //Uniforms
    
    //Fuses
    
    //Literals
    const float lit008 = 3;
    const float lit009_PI = 3.141592653589793;
    const float lit007 = 4;
    
    //Stack variables (numeric)
    float radius, volume, var004, var005, var006, var010;
    
    //Stack variables (boolean)
    uint64_t index;
    //Loop over input
    for( index = 0; index < args->N; ++index) {
    
        //Inputs
        radius = args->radius[index];
        
        //Begin kernel logic
        {
        
            //>>> volume = (4/3 * math.pi) * (radius ** 3)
            var006 = lit007 / lit008;
            var005 = var006 * lit009_PI;
            var010 = pow(radius, lit008);
            var004 = var005 * var010;
            volume = var004;
        
        }
        //End kernel logic
        
        //Outputs
        args->volume[index] = volume;
        
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

//Kernel function: volume
void volume_vector(KernelArgs* args) {

    //Setup
    const float32x4_t MASK_FALSE = vdupq_n_f32(0.f);
    const float32x4_t MASK_TRUE = vcvtq_f32_u32(vceqq_f32(MASK_FALSE, MASK_FALSE));
    
    //Uniforms
    
    //Fuses
    
    //Literals
    const float32x4_t lit008 = vdupq_n_f32(3);
    const float32x4_t lit009_PI = vdupq_n_f32(3.141592653589793);
    const float32x4_t lit007 = vdupq_n_f32(4);
    
    //Stack variables
    float32x4_t radius, volume, var004, var005, var006, var010;
    uint64_t index;
    //Loop over input
    for( index = 0; index < args->N; index += 4) {
    
        //Inputs
        radius = vld1q_f32(&args->radius[index]);
        
        //Begin kernel logic
        {
        
            //>>> volume = (4/3 * math.pi) * (radius ** 3)
            var006 = vsetq_lane_f32(vgetq_lane_f32(lit007, 0) / vgetq_lane_f32(lit008, 0), var006, 0);
            var006 = vsetq_lane_f32(vgetq_lane_f32(lit007, 1) / vgetq_lane_f32(lit008, 1), var006, 1);
            var006 = vsetq_lane_f32(vgetq_lane_f32(lit007, 2) / vgetq_lane_f32(lit008, 2), var006, 2);
            var006 = vsetq_lane_f32(vgetq_lane_f32(lit007, 3) / vgetq_lane_f32(lit008, 3), var006, 3);
            var005 = vmulq_f32(var006, lit009_PI);
            var010[0] = pow(radius[0], lit008[0]);
            var010[1] = pow(radius[1], lit008[1]);
            var010[2] = pow(radius[2], lit008[2]);
            var010[3] = pow(radius[3], lit008[3]);
            var004 = vmulq_f32(var005, var010);
            volume = var004;
        
        }
        //End kernel logic
        
        //Outputs
        vst1q_f32(&args->volume[index], volume);
        
    }
}
//End of kernel function


