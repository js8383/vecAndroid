/*******************************************************************************
*                   Generated supplement to NEON intrinsics:                   *
*******************************************************************************/
#include <stdint.h>
#include <arm_neon.h>

uint32_t vmovemask_f32(float32x4_t x) {
    uint32x4_t a = vandq_u32(vreinterpretq_u32_f32(x), (uint32x4_t) {0x1, 0x2, 0x4, 0x8});
    uint32x4_t b = vextq_u32(a, a, 2);
    uint32x4_t c = vorrq_u32(a, b);
    uint32x4_t d = vextq_u32(c, c, 3);
    uint32x4_t e = vorrq_u32(c, d);
    return vgetq_lane_u32(e, 0);
}

uint32_t vmovemask_u32(uint32x4_t x) {
    uint32x4_t a = vandq_u32(x, (uint32x4_t) {0x1, 0x2, 0x4, 0x8});
    uint32x4_t b = vextq_u32(a, a, 2);
    uint32x4_t c = vorrq_u32(a, b);
    uint32x4_t d = vextq_u32(c, c, 3);
    uint32x4_t e = vorrq_u32(c, d);
    return vgetq_lane_u32(e, 0);
}
