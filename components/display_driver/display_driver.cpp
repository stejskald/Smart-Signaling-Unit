#include <stdint.h>
#include "esp_log.h"			// for logging functionalities
#include "driver/rmt.h"			// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/rmt.html
#include "led_strip.h"			// for led strip WS2812 control
#include "display_driver.hpp"	// for ...
#include "esp_bit_defs.h"

// debug --->
#include <iostream>
#include <cstdio>

using std::cout;
// debug <---

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

// TODO Remove in final implementation
#define CHASE_SPEED_MS	(30) // by changing this value can be set the speed of chasing light

const char *TAG = "WS2812B display driver";

// p_symbol_data_t *pSymbol_data; // Row coded symbol data in 1-D array
// p_symbol_data_t display_driver::Get_symbol_data(const char aSymbol) const
// {
// 	// get the address of symbol data from the table
// 	return (p_symbol_data_t)(display_driver::ascii_tab_5x7_ + aSymbol);
// }

// pointer to array of (STRIPS_NUMBER) pointers to type led_strip_t
static led_strip_t *(*led_strip_channels)[STRIPS_NUMBER] = nullptr;

static led_strip_t *led_strip_ch0 = nullptr;

void display_driver::Led_strips_init(void)
{
	/* Init the RMT peripheral and LED strip configuration */
	led_strip_ch0 = led_strip_init(RMT_CH0_TX_GPIO, RMT_TX_CHANNEL0, STRIP_LEDS_NUMBER);

	// (*led_strip_channels)[0] = led_strip_init(RMT_CH0_TX_GPIO, RMT_TX_CHANNEL0, STRIP_LEDS_NUMBER);
	// (*led_strip_channels)[1] = led_strip_init(RMT_CH1_TX_GPIO, RMT_TX_CHANNEL1, STRIP_LEDS_NUMBER);
	// (*led_strip_channels)[2] = led_strip_init(RMT_CH2_TX_GPIO, RMT_TX_CHANNEL2, STRIP_LEDS_NUMBER);
	// (*led_strip_channels)[3] = led_strip_init(RMT_CH3_TX_GPIO, RMT_TX_CHANNEL3, STRIP_LEDS_NUMBER);
	// (*led_strip_channels)[4] = led_strip_init(RMT_CH4_TX_GPIO, RMT_TX_CHANNEL4, STRIP_LEDS_NUMBER);
	// (*led_strip_channels)[5] = led_strip_init(RMT_CH5_TX_GPIO, RMT_TX_CHANNEL5, STRIP_LEDS_NUMBER);
	// (*led_strip_channels)[6] = led_strip_init(RMT_CH6_TX_GPIO, RMT_TX_CHANNEL6, STRIP_LEDS_NUMBER);
} 

// Frame buffer with height equal to count of strips and width of double strips' length
rgb888_pixel_t frame_buffer[STRIPS_NUMBER][2 * STRIP_LEDS_NUMBER] = {};

// Frame buffer mask with height equal to count of strips and width of strips' length
bool frame_buffer_mask[STRIPS_NUMBER][STRIP_LEDS_NUMBER] = {};

void display_driver::Disp_letter(const char aSymbol, rgb888_pixel_t aColor, uint8_t aPosition)
{
	// TODO Make conversion from rgb565 to rgb888 later
	
	// Get_symbol_data - Get data of desired symbol
	const uint8_t (*pSymbol_data)[7] = (display_driver::ascii_tab_5x7_ + aSymbol);

	// cout << "Base address of ascii_tab_5x7_: " << display_driver::ascii_tab_5x7_ << std::endl;
	// cout << "Address of aSymbol:             " << pSymbol_data << std::endl;
	// cout << "Size of *pSymbol_data: " << sizeof(*pSymbol_data) << " Should be the same." << std::endl;

	// Set the frame_buffer_mask
	for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
	{
		for (uint8_t j = 0, mask = BIT4; j < 6; ++j, mask >>= 1) // j < (STRIP_LEDS_NUMBER)
		{
			frame_buffer_mask[i][j] = (*((*pSymbol_data) + i) & mask);
			
			if ((j % 6) == 0)
			{
				mask = BIT4;
			}
			// printf("%x\n", frame_buffer_mask[i][j]);
		}
		// printf("\n");
	}


	// Fill the frame_buffer
	for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
	{
		for (uint8_t j = 0; j < 6; ++j) // j < (2 * STRIP_LEDS_NUMBER)
		{
			frame_buffer[i][j] = frame_buffer_mask[i][j] ? (rgb888_pixel_t){0x00, 0x00, 0x00} : aColor;
		}
	}
	printf("Frame buffer filled.\n");

	// Display the letter
	display_driver::Led_strips_init();

	// for (size_t i = 0; i < STRIPS_NUMBER; ++i)
	// {
	// 	if (!((*led_strip_channels) + i)) {
	// 		ESP_LOGE(TAG, "install WS2812 drivers failed");
	// 	}
	
	// 	// Clear LED strips (turn off all LEDs)
	// 	ESP_ERROR_CHECK((*((*led_strip_channels) + i))->clear((*((*led_strip_channels) + i)), 100));
	// }


	printf("%p\n", led_strip_ch0);

	// ESP_LOGI(TAG, "Displaying one letter started");
	// // Loading symbol data into all LED strips "at the same time"
	// for (uint8_t j = 0; j < STRIP_LEDS_NUMBER; ++j)
	// {
	// 	for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
	// 	{
	// 		// Write RGB values to strips' drivers
	// 		ESP_ERROR_CHECK((*((*led_strip_channels) + i))->set_pixel((*((*led_strip_channels) + i)), j,
	// 						frame_buffer[i][j].red, frame_buffer[i][j].green, frame_buffer[i][j].blue));

	// 		// Flush RGB values to LEDs
	// 		ESP_ERROR_CHECK((*((*led_strip_channels) + i))->refresh((*((*led_strip_channels) + i)), 100));
	// 	}
	// }
		
		
}

// Converting HSV color space to RGB color space
// static void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
// {
// 	h %= 360; // h -> [0,360]
// 	uint32_t rgb_max = v * 2.55f;
// 	uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

// 	uint32_t i = h / 60;
// 	uint32_t diff = h % 60;

// 	// RGB adjustment amount by hue
// 	uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

// 	switch (i) {
// 	case 0:
// 		*r = rgb_max;
// 		*g = rgb_min + rgb_adj;
// 		*b = rgb_min;
// 		break;
// 	case 1:
// 		*r = rgb_max - rgb_adj;
// 		*g = rgb_max;
// 		*b = rgb_min;
// 		break;
// 	case 2:
// 		*r = rgb_min;
// 		*g = rgb_max;
// 		*b = rgb_min + rgb_adj;
// 		break;
// 	case 3:
// 		*r = rgb_min;
// 		*g = rgb_max - rgb_adj;
// 		*b = rgb_max;
// 		break;
// 	case 4:
// 		*r = rgb_min + rgb_adj;
// 		*g = rgb_min;
// 		*b = rgb_max;
// 		break;
// 	default:
// 		*r = rgb_max;
// 		*g = rgb_min;
// 		*b = rgb_max - rgb_adj;
// 		break;
// 	}
// }

// TODO: Control all the channels, now works only led_strip_ch0
// static void chasing_light_example(void){
// 	uint32_t red = 0;
// 	uint32_t green = 0;
// 	uint32_t blue = 0;
// 	uint16_t hue = 0;
// 	uint16_t start_rgb = 0;

// 	if (!led_strip_ch0) {
// 		ESP_LOGE(TAG, "install WS2812 driver failed");
// 	}
// 	// Clear LED strip (turn off all LEDs)
// 	ESP_ERROR_CHECK(led_strip_ch0->clear(led_strip_ch0, 100));
// 	// Show simple rainbow chasing pattern
// 	ESP_LOGI(TAG, "LED Rainbow Chase Start");
// 	while (true) {
// 		for (int i = 0; i < 3; i++) {
// 			for (int j = i; j < STRIP_LEDS_NUMBER; j += 3) {
// 				// Build RGB values
// 				hue = j * 360 / STRIP_LEDS_NUMBER + start_rgb;
// 				led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
// 				// Write RGB values to strip driver
// 				ESP_ERROR_CHECK(led_strip_ch0->set_pixel(led_strip_ch0, j, red, green, blue));
// 			}
// 			// Flush RGB values to LEDs
// 			ESP_ERROR_CHECK(led_strip_ch0->refresh(led_strip_ch0, 100));
// 			vTaskDelay(pdMS_TO_TICKS(CHASE_SPEED_MS));
// 			led_strip_ch0->clear(led_strip_ch0, 50);
// 			vTaskDelay(pdMS_TO_TICKS(CHASE_SPEED_MS));
// 		}
// 		start_rgb += 60;
// 	}
// }

void display_driver::Update_display_from_frame_buffer(void)
{
	;
}

