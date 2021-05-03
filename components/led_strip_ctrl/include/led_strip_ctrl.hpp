#ifndef __LED_STRIP_CTRL_HPP__
#define __LED_STRIP_CTRL_HPP__

/**
 * @brief Initialization of hues and led strips
 * 
 */
void led_strips_init(void);

/**
 * @brief Obtain the symbol data from the font lookup table - ascii_tab_5x7[96][7].
 * @details The table contains fonts for char codes from 0x20 (space) to 0x7f (DEL).
 * 
 * @param Symbol_Hex_Code 
 */
void load_symbol(uint16_t symbol_hex_code);

// TODO Define data type rgb_pixel_t

/**
 * @brief The main control algorythm of led strip matrix display
 * 
 */
void led_strip_matrix_disp_ctrl(void);

#endif /* __LED_STRIP_CTRL_HPP__ */