#ifndef __LED_STRIP_DISPLAY_CTRL_HPP__
#define __LED_STRIP_DISPLAY_CTRL_HPP__
#include <stdint.h>

// #define STRIPS_NUMBER 7
// #define STRIP_LEDS_NUMBER 36

/**
 * @brief Initialization of hues and led strips
 * 
 */
void led_strips_init(void);

/**
 * @brief The main control algorythm of led strip matrix display
 * 
 */
void led_strip_matrix_disp_ctrl(void);

#endif /* __LED_STRIP_DISPLAY_CTRL_HPP__ */