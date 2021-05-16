#include <cstdint>
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
static led_strip_t *led_strip_channels[STRIPS_NUMBER]{nullptr};

/**
 * @brief Pointer to symbol data (array of 7 uint8_t elements).
 * 
 */
p_symbol_data_t display_driver::pSymbol_data{};

/**
 * @brief Frame buffer mask with height equal to count of strips and width of strips' length
 * 
 */
bool display_driver::frame_buffer_mask[STRIPS_NUMBER][STRIP_LEDS_NUMBER] = {};

/**
 * @brief Frame buffer with height equal to count of strips and width of double strips' length.
 * 	Store the data for display driver.
 * 
 */
rgb888_pixel_t display_driver::frame_buffer[STRIPS_NUMBER][2 * STRIP_LEDS_NUMBER] = {};


/* ================================ PUBLIC METHODS  ================================ */

esp_err_t display_driver::Display_symbol(const char &aSymbol, const rgb888_pixel_t &aColor, const uint8_t &aPosition)
{
	if (aPosition > 6)
	{
		ESP_LOGE(TAG, "Parameter aPosition must not be greater than 6!");
		return ESP_ERR_INVALID_ARG;
	}

	/* Get data of desired symbol */
	pSymbol_data = Get_symbol_data(&aSymbol);

	/* Set the frame_buffer_mask for the symbol*/
	Set_frame_buffer_mask(aPosition);

	/* Fill the frame_buffer */
	Fill_frame_buffer(aColor);

	/* Initialization of led strips */
	Led_strips_init();
	
	/* Display the symbol */
	// Loading symbol data into all LED strips "at the same time"
	for (uint8_t j = 0; j < STRIP_LEDS_NUMBER; ++j)
	{
		for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
		{
			// Write RGB values to strips' drivers
			ESP_ERROR_CHECK(led_strip_channels[i]->set_pixel(led_strip_channels[i], j,
							frame_buffer[i][j].red, frame_buffer[i][j].green, frame_buffer[i][j].blue));

			// Flush RGB values to LEDs
			ESP_ERROR_CHECK(led_strip_channels[i]->refresh(led_strip_channels[i], 100));
		}
	}
	ESP_LOGI(TAG, "The symbol displayed.");
	return ESP_OK;	
}

esp_err_t display_driver::Display_text(const uint16_t (&aText)[3], const rgb888_pixel_t &aColor)
{
	// if (aText == 0) // BUG How to check that value of reference equals 0?
	// {
	// 	ESP_LOGE(TAG, "The parameter aText does not contain any symbols!");
	// 	return ESP_ERR_INVALID_ARG;
	// }

	/* Extraction of symbols from text stored in holding registers */
	char symbols[6]{}; 
	Extract_symbols_from_holding_regs(aText, symbols);

	/* Set the frame_buffer_mask for all symbols*/
	for (uint8_t pos = 1; pos <= 6; ++pos)
	{
		// Get data of desired symbol
		pSymbol_data = Get_symbol_data(&symbols[pos-1]); // TODO iterate through symbols array

		Set_frame_buffer_mask(pos);
	}
	
	/* Fill the frame_buffer */
	Fill_frame_buffer(aColor);

	/* Initialization of led strips */
	Led_strips_init();

	/* Display the text */
	Update_display_from_frame_buffer();

	return ESP_OK;
}

/* ================================ PRIVATE METHODS  ================================ */

p_symbol_data_t display_driver::Get_symbol_data(const char *aSymbol) const
{
	return (display_driver::ascii_tab_5x7_ + *aSymbol);
}

void display_driver::Set_frame_buffer_mask(const uint8_t &aPosition)
{
	for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
	{
		for (uint8_t j = (6 * (aPosition - 1)), mask = BIT4; j < (6 + 6 * (aPosition - 1)); ++j, mask >>= 1)
		{
			frame_buffer_mask[i][j] = (*pSymbol_data)[i] & mask;
			
			if ((j % 6) == 0)
			{
				mask = BIT4;
			}
		}
	}
	ESP_LOGI(TAG, "Set frame buffer mask for symbol at position %d.", aPosition);
}

void display_driver::Fill_frame_buffer(const rgb888_pixel_t &aColor)
{
	// TODO Make conversion from rgb565 to rgb888 later

	for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
	{
		for (uint8_t j = 0, k = 36; j < STRIP_LEDS_NUMBER; ++j, ++k) // Filling the first half of frame_buffer
		{
			uint8_t symbol_red = aColor.green + aColor.blue;
			uint8_t symbol_green = aColor.red + aColor.blue;
			uint8_t symbol_blue = aColor.red + aColor.green;
			// printf("%2x\t%2x\t%2x\n", aColor.red, aColor.green, aColor.blue);
			// printf("%2x\t%2x\t%2x\n", symbol_red, symbol_green, symbol_blue);
			frame_buffer[i][j] = frame_buffer_mask[i][j] ? (rgb888_pixel_t){symbol_red, symbol_green, symbol_blue} : aColor;
			frame_buffer[i][k] = frame_buffer[i][j]; // Filling the second half of frame_buffer
		}
	}
	ESP_LOGI(TAG, "Frame buffer filled.");
}

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
	ESP_LOGI(TAG, "Led strips initialized.");
}

void display_driver::Update_display_from_frame_buffer(void)
{
	// Loading text data into all LED strips "at the same time"
	for (uint8_t j = 0; j < STRIP_LEDS_NUMBER; ++j)
	{
		for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
		{
			// Write RGB values to strips' drivers
			ESP_ERROR_CHECK(led_strip_channels[i]->set_pixel(led_strip_channels[i], j,
							frame_buffer[i][j].red, frame_buffer[i][j].green, frame_buffer[i][j].blue));

			// Flush RGB values to LEDs
			ESP_ERROR_CHECK(led_strip_channels[i]->refresh(led_strip_channels[i], 100));
		}
	}
	ESP_LOGI(TAG, "Display updated and text displayed.");
}

void display_driver::Extract_symbols_from_holding_regs(const uint16_t (&aText)[3], char (&aSymbols)[6])
	{
		// Extract symbols stored in (aText)
		for (uint8_t i = 0, j = 0; i < sizeof(aText)/2; ++i, j += 2)
		{
			aSymbols[j]   = aText[i] >> 8;
			aSymbols[j+1] = aText[i] & 0x00ff;
		}
		ESP_LOGI(TAG, "Symbols extracted from holding registers.");
	}