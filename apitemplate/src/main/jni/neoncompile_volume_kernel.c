/*******************************************************************************
*                           Generated kernel: volume                           *
*******************************************************************************/
//Integer types
#include <stdint.h>
#include "neoncompile_volume_kernel.h"

/*******************************************************************************
*                      Target Architecture: NEON (float)                       *
*******************************************************************************/
//Includes
#include <math.h>
//#include <algorithm>

//Kernel function: volume
void volume_scalar(KernelArgs* args) {

    //Uniforms
    
    //Fuses
    
    //Literals
    const float lit070 = 3;
    const float lit071_PI = 3.141592653589793;
    const float lit069 = 4;
    
    //Stack variables (numeric)
    float radius, volume, var066, var067, var068, var072;
    
    //Stack variables (boolean)
    
    //Loop over input
    uint64_t index;
    for(index = 0; index < args->N; ++index) {
    
        //Inputs
        radius = args->radius[index];
        
        //Begin kernel logic
        {
        
            //>>> volume = (4/3 * math.pi) * (radius ** 3)
            var068 = lit069 / lit070;
            var067 = var068 * lit071_PI;
            var072 = pow(radius, lit070);
            var066 = var067 * var072;
            volume = var066;
        
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
#include "neon_more.h"

//Kernel function: volume
void volume_vector(KernelArgs* args) {

    //Setup
    const float32x4_t MASK_FALSE = vdupq_n_f32(0.f);
    const float32x4_t MASK_TRUE = vcvtq_f32_u32(vceqq_f32(MASK_FALSE, MASK_FALSE));
    
    //Uniforms
    
    //Fuses
    
    //Literals
    const float32x4_t lit070 = vdupq_n_f32(3);
    const float32x4_t lit071_PI = vdupq_n_f32(3.141592653589793);
    const float32x4_t lit069 = vdupq_n_f32(4);
    
    //Stack variables
    float32x4_t radius, volume, var066, var067, var068, var072;
    
    //Loop over input
    uint64_t index;
    for(index = 0; index < args->N; index += 4) {
    
        //Inputs
        radius = vld1q_f32(&args->radius[index]);
        
        //Begin kernel logic
        {
        
            //>>> volume = (4/3 * math.pi) * (radius ** 3)
            var068 = vsetq_lane_f32(vgetq_lane_f32(lit069, 0) / vgetq_lane_f32(lit070, 0), var068, 0);
            var068 = vsetq_lane_f32(vgetq_lane_f32(lit069, 1) / vgetq_lane_f32(lit070, 1), var068, 1);
            var068 = vsetq_lane_f32(vgetq_lane_f32(lit069, 2) / vgetq_lane_f32(lit070, 2), var068, 2);
            var068 = vsetq_lane_f32(vgetq_lane_f32(lit069, 3) / vgetq_lane_f32(lit070, 3), var068, 3);
            var067 = vmulq_f32(var068, lit071_PI);
            var072[0] = pow(radius[0], lit070[0]);
            var072[1] = pow(radius[1], lit070[1]);
            var072[2] = pow(radius[2], lit070[2]);
            var072[3] = pow(radius[3], lit070[3]);
            var066 = vmulq_f32(var067, var072);
            volume = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), var066, volume);
        
        }
        //End kernel logic
        
        //Outputs
        vst1q_f32(&args->volume[index], volume);
        
    }
}
//End of kernel function


