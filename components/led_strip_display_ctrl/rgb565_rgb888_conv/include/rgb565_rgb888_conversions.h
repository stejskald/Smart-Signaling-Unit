/* Algorythms for conversions from rgb565 to rgb888 and from rgb888 to rgb565 */
#ifndef __RGB565_RGB888_CONVERSIONS_H__
#define __RGB565_RGB888_CONVERSIONS_H__
#include <stdint.h>

/**
 * @brief Conversion from rgb565 to rgb888 pixel
 * 
 * @param rgb_565_pixel 
 * @return uint32_t 
 */
uint32_t rgb565_to_rgb888(uint16_t *rgb_565_pixel);

/**
 * @brief Conversion from rgb888 to rgb565 pixel
 * 
 * @param rgb_888_pixel 
 * @return uint16_t 
 */
uint16_t rgb888_to_rgb565(uint8_t *rgb_888_pixel);
#endif /* __RGB565_RGB888_CONVERSIONS_H__ */