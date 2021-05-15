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

// pointer to array of (STRIPS_NUMBER) pointers to type led_strip_t
static led_strip_t *led_strip_channels[STRIPS_NUMBER] = {nullptr};

void display_driver::Led_strips_init(void)
{
	/* Init the RMT peripheral and LED strip configuration */
	led_strip_channels[0] = led_strip_init(RMT_TX_CHANNEL0, RMT_CH0_TX_GPIO, STRIP_LEDS_NUMBER);
	led_strip_channels[1] = led_strip_init(RMT_TX_CHANNEL1, RMT_CH1_TX_GPIO, STRIP_LEDS_NUMBER);
	led_strip_channels[2] = led_strip_init(RMT_TX_CHANNEL2, RMT_CH2_TX_GPIO, STRIP_LEDS_NUMBER);
	led_strip_channels[3] = led_strip_init(RMT_TX_CHANNEL3, RMT_CH3_TX_GPIO, STRIP_LEDS_NUMBER);
	led_strip_channels[4] = led_strip_init(RMT_TX_CHANNEL4, RMT_CH4_TX_GPIO, STRIP_LEDS_NUMBER);
	led_strip_channels[5] = led_strip_init(RMT_TX_CHANNEL5, RMT_CH5_TX_GPIO, STRIP_LEDS_NUMBER);
	led_strip_channels[6] = led_strip_init(RMT_TX_CHANNEL6, RMT_CH6_TX_GPIO, STRIP_LEDS_NUMBER);
} 

// Frame buffer with height equal to count of strips and width of double strips' length
rgb888_pixel_t frame_buffer[STRIPS_NUMBER][2 * STRIP_LEDS_NUMBER] = {};

// Frame buffer mask with height equal to count of strips and width of strips' length
bool frame_buffer_mask[STRIPS_NUMBER][STRIP_LEDS_NUMBER] = {};

// display_driver::Get_symbol_data(const char &aSymbol) const
// {
// 	return;
// }

void display_driver::Disp_letter(const char &aSymbol, const rgb888_pixel_t &aColor, const uint8_t &aPosition)
{
	// TODO Make conversion from rgb565 to rgb888 later
	
	/* Get_symbol_data - Get data of desired symbol */
	const uint8_t (*pSymbol_data)[7] = (display_driver::ascii_tab_5x7_ + aSymbol);

	/* Set the frame_buffer_mask */
	for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
	{
		for (uint8_t j = (0 + 6 * aPosition), mask = BIT4; j < (6 + 6 * aPosition); ++j, mask >>= 1)
		{
			frame_buffer_mask[i][j] = (*((*pSymbol_data) + i) & mask);
			
			if ((j % 6) == 0)
			{
				mask = BIT4;
			}
		}
	}


	/* Fill the frame_buffer */
	for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
	{
		for (uint8_t j = 0, k = 36; j < STRIP_LEDS_NUMBER; ++j, ++k) // Filling the first half of frame_buffer
		{
			frame_buffer[i][j] = frame_buffer_mask[i][j] ? (rgb888_pixel_t){0x00, 0x00, 0x00} : aColor;
			frame_buffer[i][k] = frame_buffer[i][j]; // Filling the second half of frame_buffer
		}
	}
	ESP_LOGI(TAG, "Frame buffer filled");

	/* Display the letter */
	// Initialization of led strips
	display_driver::Led_strips_init();
	ESP_LOGI(TAG, "Led strips initialized");
	
	ESP_LOGI(TAG, "Displaying one letter started");
	// Loading symbol data into all LED strips "at the same time"
	for (uint8_t j = 0; j < STRIP_LEDS_NUMBER; ++j)
	{
		for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
		{
			// Write RGB values to strips' drivers
			ESP_ERROR_CHECK(led_strip_channels[i]->set_pixel(led_strip_channels[i], j,
							frame_buffer[i][j].red, frame_buffer[i][j].green, frame_buffer[i][j].blue));
			// printf("%2x\n", frame_buffer[i][j].red);
			// printf("%2x\n", frame_buffer[i][j].green);
			// printf("%2x\n\n", frame_buffer[i][j].blue);

			// Flush RGB values to LEDs
			ESP_ERROR_CHECK(led_strip_channels[i]->refresh(led_strip_channels[i], 100));
		}
	}
		
		
}

void display_driver::Update_display_from_frame_buffer(void)
{
	;
}

