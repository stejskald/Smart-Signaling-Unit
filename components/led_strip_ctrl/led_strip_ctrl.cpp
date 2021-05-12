#include <stdint.h>
#include <string>
#include "esp_log.h"			// for logging functionalities
#include "driver/rmt.h"			// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/rmt.html
#include "led_strip.h"			// for led strip WS2812 control
#include "led_strip_ctrl.hpp"	// for ...
#include "ascii_table_5x7.h"	// for 5x7 ASCII characters table


const char *TAG = "WS2812B RMT"; // RMT - Remote Control
const std::string text = "Hello world! It's the 4th of May today! \\m/";

// defining of RMT TX channels 0-6
#define RMT_TX_CHANNEL0 RMT_CHANNEL_0
#define RMT_TX_CHANNEL1 RMT_CHANNEL_1
#define RMT_TX_CHANNEL2 RMT_CHANNEL_2
#define RMT_TX_CHANNEL3 RMT_CHANNEL_3
#define RMT_TX_CHANNEL4 RMT_CHANNEL_4
#define RMT_TX_CHANNEL5 RMT_CHANNEL_5
#define RMT_TX_CHANNEL6 RMT_CHANNEL_6

// defining of GPIO pin numbers for RMT TX channels 0-6
#define RMT_CH0_TX_GPIO 4  // 9,  pin 4  is for Olimex ESP32-PoE Rev.B
#define RMT_CH1_TX_GPIO 16 // 10, pin 16 is for Olimex ESP32-PoE Rev.B
#define RMT_CH2_TX_GPIO 15 // 11, pin 15 is for Olimex ESP32-PoE Rev.B
#define RMT_CH3_TX_GPIO 14 // 6,  pin 14 is for Olimex ESP32-PoE Rev.B
#define RMT_CH4_TX_GPIO 13 // 7,  pin 13 is for Olimex ESP32-PoE Rev.B
#define RMT_CH5_TX_GPIO 2  // 8,  pin 2  is for Olimex ESP32-PoE Rev.B
#define RMT_CH6_TX_GPIO 5  // 15, pin 5  is for Olimex ESP32-PoE Rev.B

#define STRIPS_NUMBER 7
#define STRIP_LEDS_NUMBER 36
// TODO Remove in final implementation
#define CHASE_SPEED_MS	(30) // by changing this value can be set the speed of chasing light

/**
 * @brief Converting HSV color space to RGB color space
 *
 * More info here (Wiki): https://en.wikipedia.org/wiki/HSL_and_HSV
 *
 */
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
	h %= 360; // h -> [0,360]
	uint32_t rgb_max = v * 2.55f;
	uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

	uint32_t i = h / 60;
	uint32_t diff = h % 60;

	// RGB adjustment amount by hue
	uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

	switch (i) {
	case 0:
		*r = rgb_max;
		*g = rgb_min + rgb_adj;
		*b = rgb_min;
		break;
	case 1:
		*r = rgb_max - rgb_adj;
		*g = rgb_max;
		*b = rgb_min;
		break;
	case 2:
		*r = rgb_min;
		*g = rgb_max;
		*b = rgb_min + rgb_adj;
		break;
	case 3:
		*r = rgb_min;
		*g = rgb_max - rgb_adj;
		*b = rgb_max;
		break;
	case 4:
		*r = rgb_min + rgb_adj;
		*g = rgb_min;
		*b = rgb_max;
		break;
	default:
		*r = rgb_max;
		*g = rgb_min;
		*b = rgb_max - rgb_adj;
		break;
	}
}

led_strip_t *led_strip_ch0 = nullptr;
// led_strip_t *led_strip_ch1 = nullptr;
// led_strip_t *led_strip_ch2 = nullptr;
// led_strip_t *led_strip_ch3 = nullptr;
// led_strip_t *led_strip_ch4 = nullptr;
// led_strip_t *led_strip_ch5 = nullptr;
// led_strip_t *led_strip_ch6 = nullptr;

void led_strips_init(void)
{
	/* Init the RMT peripheral and LED strip configuration */
	led_strip_t *led_strip_ch0 = led_strip_init(RMT_CH0_TX_GPIO, RMT_TX_CHANNEL0, STRIP_LEDS_NUMBER);
	// led_strip_t *led_strip_ch1 = led_strip_init(RMT_CH1_TX_GPIO, RMT_TX_CHANNEL1, STRIP_LEDS_NUMBER);
	// led_strip_t *led_strip_ch2 = led_strip_init(RMT_CH2_TX_GPIO, RMT_TX_CHANNEL2, STRIP_LEDS_NUMBER);
	// led_strip_t *led_strip_ch3 = led_strip_init(RMT_CH3_TX_GPIO, RMT_TX_CHANNEL3, STRIP_LEDS_NUMBER);
	// led_strip_t *led_strip_ch4 = led_strip_init(RMT_CH4_TX_GPIO, RMT_TX_CHANNEL4, STRIP_LEDS_NUMBER);
	// led_strip_t *led_strip_ch5 = led_strip_init(RMT_CH5_TX_GPIO, RMT_TX_CHANNEL5, STRIP_LEDS_NUMBER);
	// led_strip_t *led_strip_ch6 = led_strip_init(RMT_CH6_TX_GPIO, RMT_TX_CHANNEL6, STRIP_LEDS_NUMBER);
}

// Obtain the symbol data from the font lookup table - ascii_tab_5x7[96][7].
// The table contains fonts for char codes 0x20 (space) to 0x7f (DEL).
void load_symbol(const uint8_t aSymbol_hex_code)
{
	// // Normalise the ASCII code
	// if(aSymbol_hex_code >= 0x20 && aSymbol_hex_code < 0x80)
	// 	aSymbol_hex_code -= 0x20;
	// // DEL symbol after ASCII normalization if the aSymbol_hex_code is out of font table range
	// else
	// 	aSymbol_hex_code = 0x5f;

	// TODO Get the symbol data from the table
	
	// return ...;
}

// Display buffer with double strips' lenght
CRgb_pixel_t disp_buffer[2 * STRIP_LEDS_NUMBER][STRIPS_NUMBER];

void fill_disp_buffer(CRgb_pixel_t *aDisp_buffer)
{
	// char symbol = 0x23;
	// load_symbol(symbol);

	// aDisp_buffer = ...
}

void update_strips_from_disp_buffer(void)
{
	;
}

void led_strip_matrix_disp_ctrl(void)
{
	// char c = text.at(1);
}


// // TODO: Control all the channels, now works only led_strip_ch0
void chasing_light_example(void){
	uint32_t red = 0;
	uint32_t green = 0;
	uint32_t blue = 0;
	uint16_t hue = 0;
	uint16_t start_rgb = 0;

	if (!led_strip_ch0) {
		ESP_LOGE(TAG, "install WS2812 driver failed");
	}
	// Clear LED strip (turn off all LEDs)
	ESP_ERROR_CHECK(led_strip_ch0->clear(led_strip_ch0, 100));
	// Show simple rainbow chasing pattern
	ESP_LOGI(TAG, "LED Rainbow Chase Start");
	while (true) {
		for (int i = 0; i < 3; i++) {
			for (int j = i; j < STRIP_LEDS_NUMBER; j += 3) {
				// Build RGB values
				hue = j * 360 / STRIP_LEDS_NUMBER + start_rgb;
				led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
				// Write RGB values to strip driver
				ESP_ERROR_CHECK(led_strip_ch0->set_pixel(led_strip_ch0, j, red, green, blue));
			}
			// Flush RGB values to LEDs
			ESP_ERROR_CHECK(led_strip_ch0->refresh(led_strip_ch0, 100));
			vTaskDelay(pdMS_TO_TICKS(CHASE_SPEED_MS));
			led_strip_ch0->clear(led_strip_ch0, 50);
			vTaskDelay(pdMS_TO_TICKS(CHASE_SPEED_MS));
		}
		start_rgb += 60;
	}
}
