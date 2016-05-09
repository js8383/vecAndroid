/*******************************************************************************
*                         Generated kernel: mandelbrot                         *
*******************************************************************************/
//Integer types
#include <stdint.h>
#include "neoncompile_mandelbrot_kernel.h"

/*******************************************************************************
*                      Target Architecture: NEON (float)                       *
*******************************************************************************/
//Includes
#include <math.h>
//#include <algorithm>
#include <stdbool.h>


//Kernel function: mandelbrot
void mandelbrot_scalar(KernelArgs* args) {

    //Uniforms
    const float max = args->max;
    const float w_m1 = args->w_m1;
    const float h_m1 = args->h_m1;
    const float left = args->left;
    const float right = args->right;
    const float top = args->top;
    const float bottom = args->bottom;
    
    //Fuses
    
    //Literals
    const float lit023 = 0;
    const float lit045 = 1;
    const float lit041 = 2;
    const float lit032 = 16;
    
    //Stack variables (numeric)
    float row, col, count, var012, var013, var014, var015, x0, var017, var018, var019, var020, var021, y0, x, y, var026, xx, var028, yy, var031, var036, var037, var038, var039, var040, var042, var043, var044, var048, var049, var050, var051, var052, var053;
    
    //Stack variables (boolean)
    bool mask030, mask033, mask034, mask035, mask046, mask047;
    
    //Loop over input
    uint64_t index;
    for(index = 0; index < args->N; ++index) {
    
        //Inputs
        row = args->row[index];
        col = args->col[index];
        count = args->count[index];
        
        //Begin kernel logic
        {
        
            //>>> x0 = left + col * (right - left) / w_m1
            var015 = right - left;
            var014 = col * var015;
            var013 = var014 / w_m1;
            var012 = left + var013;
            x0 = var012;
            //>>> y0 = bottom + (h_m1 - row) * (top - bottom) / h_m1
            var020 = h_m1 - row;
            var021 = top - bottom;
            var019 = var020 * var021;
            var018 = var019 / h_m1;
            var017 = bottom + var018;
            y0 = var017;
            //>>> x = y = count = 0
            x = lit023;
            y = lit023;
            count = lit023;
            //>>> xx, yy = (x * x), (y * y)
            var026 = x * x;
            var028 = y * y;
            xx = var026;
            yy = var028;
            //>>> while (count < max and xx + yy < 16):
            mask030 = count < max;
            var031 = xx + yy;
            mask033 = var031 < lit032;
            mask034 = mask030 && mask033;
            mask035 = mask034;
            while(mask035) {
                //>>> x, y = (xx - yy + x0), (2 * x * y + y0)
                var037 = xx - yy;
                var036 = var037 + x0;
                var040 = lit041 * x;
                var039 = var040 * y;
                var038 = var039 + y0;
                x = var036;
                y = var038;
                //>>> xx, yy = (x * x), (y * y)
                var042 = x * x;
                var043 = y * y;
                xx = var042;
                yy = var043;
                //>>> count += 1
                var044 = count + lit045;
                count = var044;
                //>>> while (count < max and xx + yy < 16):
                mask030 = count < max;
                var031 = xx + yy;
                mask033 = var031 < lit032;
                mask034 = mask030 && mask033;
                mask035 = mask034;
            }
            //>>> if count < max:
            mask046 = count < max;
            mask047 = mask046;
            if(mask047) {
                //>>> count += 1 - math.log2(math.log2(xx + yy) / 2)
                var053 = xx + yy;
                var052 = log2(var053);
                var051 = var052 / lit041;
                var050 = log2(var051);
                var049 = lit045 - var050;
                var048 = count + var049;
                count = var048;
            }
            //>>> return (row, col, count)
        
        }
        //End kernel logic
        
        //Outputs
        args->count[index] = count;
        
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

//Kernel function: mandelbrot
void mandelbrot_vector(KernelArgs* args) {

    //Setup
    const float32x4_t MASK_FALSE = vdupq_n_f32(0.f);
    const float32x4_t MASK_TRUE = vcvtq_f32_u32(vceqq_f32(MASK_FALSE, MASK_FALSE));
    
    //Uniforms
    const float32x4_t max = vdupq_n_f32(args->max);
    const float32x4_t w_m1 = vdupq_n_f32(args->w_m1);
    const float32x4_t h_m1 = vdupq_n_f32(args->h_m1);
    const float32x4_t left = vdupq_n_f32(args->left);
    const float32x4_t right = vdupq_n_f32(args->right);
    const float32x4_t top = vdupq_n_f32(args->top);
    const float32x4_t bottom = vdupq_n_f32(args->bottom);
    
    //Fuses
    
    //Literals
    const float32x4_t lit023 = vdupq_n_f32(0);
    const float32x4_t lit045 = vdupq_n_f32(1);
    const float32x4_t lit041 = vdupq_n_f32(2);
    const float32x4_t lit032 = vdupq_n_f32(16);
    
    //Stack variables
    float32x4_t row, col, count, var012, var013, var014, var015, x0, var017, var018, var019, var020, var021, y0, x, y, var026, xx, var028, yy, mask030, var031, mask033, mask034, mask035, var036, var037, var038, var039, var040, var042, var043, var044, mask046, mask047, var048, var049, var050, var051, var052, var053;
    
    //Loop over input
    uint64_t index;
    for(index = 0; index < args->N; index += 4) {
    
        //Inputs
        row = vld1q_f32(&args->row[index]);
        col = vld1q_f32(&args->col[index]);
        count = vld1q_f32(&args->count[index]);
        
        //Begin kernel logic
        {
        
            //>>> x0 = left + col * (right - left) / w_m1
            var015 = vsubq_f32(right, left);
            var014 = vmulq_f32(col, var015);
            var013 = vsetq_lane_f32(vgetq_lane_f32(var014, 0) / vgetq_lane_f32(w_m1, 0), var013, 0);
            var013 = vsetq_lane_f32(vgetq_lane_f32(var014, 1) / vgetq_lane_f32(w_m1, 1), var013, 1);
            var013 = vsetq_lane_f32(vgetq_lane_f32(var014, 2) / vgetq_lane_f32(w_m1, 2), var013, 2);
            var013 = vsetq_lane_f32(vgetq_lane_f32(var014, 3) / vgetq_lane_f32(w_m1, 3), var013, 3);
            var012 = vaddq_f32(left, var013);
            x0 = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), var012, x0);
            //>>> y0 = bottom + (h_m1 - row) * (top - bottom) / h_m1
            var020 = vsubq_f32(h_m1, row);
            var021 = vsubq_f32(top, bottom);
            var019 = vmulq_f32(var020, var021);
            var018 = vsetq_lane_f32(vgetq_lane_f32(var019, 0) / vgetq_lane_f32(h_m1, 0), var018, 0);
            var018 = vsetq_lane_f32(vgetq_lane_f32(var019, 1) / vgetq_lane_f32(h_m1, 1), var018, 1);
            var018 = vsetq_lane_f32(vgetq_lane_f32(var019, 2) / vgetq_lane_f32(h_m1, 2), var018, 2);
            var018 = vsetq_lane_f32(vgetq_lane_f32(var019, 3) / vgetq_lane_f32(h_m1, 3), var018, 3);
            var017 = vaddq_f32(bottom, var018);
            y0 = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), var017, y0);
            //>>> x = y = count = 0
            x = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), lit023, x);
            y = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), lit023, y);
            count = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), lit023, count);
            //>>> xx, yy = (x * x), (y * y)
            var026 = vmulq_f32(x, x);
            var028 = vmulq_f32(y, y);
            xx = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), var026, xx);
            yy = vbslq_f32(vcvtq_u32_f32(MASK_TRUE), var028, yy);
            //>>> while (count < max and xx + yy < 16):
            mask030 = vcvtq_f32_u32(vcltq_f32(count, max));
            var031 = vaddq_f32(xx, yy);
            mask033 = vcvtq_f32_u32(vcltq_f32(var031, lit032));
            mask034 = vcvtq_f32_u32(vandq_u32(vcvtq_u32_f32(mask030), vcvtq_u32_f32(mask033)));
            mask035 = vcvtq_f32_u32(vandq_u32(vcvtq_u32_f32(mask034), vcvtq_u32_f32(MASK_TRUE)));
            while(vmovemask_f32(mask035)) {
                //>>> x, y = (xx - yy + x0), (2 * x * y + y0)
                var037 = vsubq_f32(xx, yy);
                var036 = vaddq_f32(var037, x0);
                var040 = vmulq_f32(lit041, x);
                var039 = vmulq_f32(var040, y);
                var038 = vaddq_f32(var039, y0);
                x = vbslq_f32(vcvtq_u32_f32(mask035), var036, x);
                y = vbslq_f32(vcvtq_u32_f32(mask035), var038, y);
                //>>> xx, yy = (x * x), (y * y)
                var042 = vmulq_f32(x, x);
                var043 = vmulq_f32(y, y);
                xx = vbslq_f32(vcvtq_u32_f32(mask035), var042, xx);
                yy = vbslq_f32(vcvtq_u32_f32(mask035), var043, yy);
                //>>> count += 1
                var044 = vaddq_f32(count, lit045);
                count = vbslq_f32(vcvtq_u32_f32(mask035), var044, count);
                //>>> while (count < max and xx + yy < 16):
                mask030 = vcvtq_f32_u32(vcltq_f32(count, max));
                var031 = vaddq_f32(xx, yy);
                mask033 = vcvtq_f32_u32(vcltq_f32(var031, lit032));
                mask034 = vcvtq_f32_u32(vandq_u32(vcvtq_u32_f32(mask030), vcvtq_u32_f32(mask033)));
                mask035 = vcvtq_f32_u32(vandq_u32(vcvtq_u32_f32(mask034), vcvtq_u32_f32(MASK_TRUE)));
            }
            //>>> if count < max:
            mask046 = vcvtq_f32_u32(vcltq_f32(count, max));
            mask047 = vcvtq_f32_u32(vandq_u32(vcvtq_u32_f32(mask046), vcvtq_u32_f32(MASK_TRUE)));
            {
                //>>> count += 1 - math.log2(math.log2(xx + yy) / 2)
                var053 = vaddq_f32(xx, yy);
                var052[0] = log2(var053[0]);
                var052[1] = log2(var053[1]);
                var052[2] = log2(var053[2]);
                var052[3] = log2(var053[3]);
                var051 = vsetq_lane_f32(vgetq_lane_f32(var052, 0) / vgetq_lane_f32(lit041, 0), var051, 0);
                var051 = vsetq_lane_f32(vgetq_lane_f32(var052, 1) / vgetq_lane_f32(lit041, 1), var051, 1);
                var051 = vsetq_lane_f32(vgetq_lane_f32(var052, 2) / vgetq_lane_f32(lit041, 2), var051, 2);
                var051 = vsetq_lane_f32(vgetq_lane_f32(var052, 3) / vgetq_lane_f32(lit041, 3), var051, 3);
                var050[0] = log2(var051[0]);
                var050[1] = log2(var051[1]);
                var050[2] = log2(var051[2]);
                var050[3] = log2(var051[3]);
                var049 = vsubq_f32(lit045, var050);
                var048 = vaddq_f32(count, var049);
                count = vbslq_f32(vcvtq_u32_f32(mask047), var048, count);
            }
            //>>> return (row, col, count)
        
        }
        //End kernel logic
        
        //Outputs
        vst1q_f32(&args->count[index], count);
        
    }
}
//End of kernel function


