/* Algorythms for conversions from rgb565 to rgb888 and from rgb888 to rgb565 */
#include "rgb565_rgb888_conversions.h"

uint32_t rgb565_to_rgb888(uint16_t *rgb_565_pixel)
{
    // raw values shifted to the left of the least significant byte
    uint8_t r565_SHL3 = (*rgb_565_pixel & 0xF800) >> 8; // 0b-----000
    uint8_t g565_SHL2 = (*rgb_565_pixel & 0x07E0) >> 3; // 0b------00
    uint8_t b565_SHL3 = (*rgb_565_pixel & 0x001F) << 3; // 0b-----000

    // extract 3 (2) the most significant bits of raw values
    uint8_t r565_SHL3_3MSB_SHR5 = (r565_SHL3 & 0xE0) >> 5; // 0b00000---
    uint8_t g565_SHL2_2MSB_SHR6 = (g565_SHL2 & 0xC0) >> 6; // 0b000000--
    uint8_t b565_SHL3_3MSB_SHR5 = (b565_SHL3 & 0xE0) >> 5; // 0b00000---

    // append previously extracted bits to the raw values and move final color values to the correct positions
    uint32_t r = (r565_SHL3 | r565_SHL3_3MSB_SHR5) << 16; // 0b--------_00000000_00000000
    uint16_t g = (g565_SHL2 | g565_SHL2_2MSB_SHR6) << 8;  // 0b00000000_--------_00000000
    uint8_t b = b565_SHL3 | b565_SHL3_3MSB_SHR5;          // 0b00000000_00000000_--------

    return (uint32_t)(r | g | b);
}

uint16_t rgb888_to_rgb565(uint8_t *rgb_888_pixel)
{
    uint8_t red = rgb_888_pixel[0];
    uint8_t green = rgb_888_pixel[1];
    uint8_t blue = rgb_888_pixel[2];

    uint16_t b = (blue >> 3) & 0x1F;
    uint16_t g = ((green >> 2) & 0x3F) << 5;
    uint16_t r = ((red >> 3) & 0x1F) << 11;

    return (uint16_t)(r | g | b);
}