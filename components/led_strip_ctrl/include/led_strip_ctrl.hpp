#ifndef __LED_STRIP_CTRL_HPP__
#define __LED_STRIP_CTRL_HPP__
#include <stdint.h>

/**
 * @brief Initialization of hues and led strips
 * 
 */
void led_strips_init(void);

/**
 * @brief Obtain the symbol data from the font lookup table - ascii_tab_5x7[96][7].
 * @details The table contains fonts for char codes from 0x20 (space) to 0x7f (DEL).
 * 
 * @param aSymbol_hex_code 
 */
void load_symbol(const uint8_t aSymbol_hex_code);

/**
 * @brief led strip pixel - RGB 8:8:8
 * 
 */
class CRgb_pixel_t
{
	public:
		uint8_t blue;
		uint8_t green;
		uint8_t red;
};

/**
 * @brief The main control algorythm of led strip matrix display
 * 
 */
void led_strip_matrix_disp_ctrl(void);

#endif /* __LED_STRIP_CTRL_HPP__ */