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
#define BLINK_SPEED_MS	(500) // by changing this value can be set the speed of chasing light
// vTaskDelay(pdMS_TO_TICKS(BLINK_SPEED_MS));

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

// TODO Add args for cooperation with Modbus TCP Server
void led_display_driver_operation(void *arg)
{
	display_driver Cdisplay_driver;

	// Color data from holding register
	uint16_t color{0b1111110111100000}; // AMBER(255:191:0) = r(11111), g(101111), b(00000)

	// Text data from holding registers
	uint16_t text[3]{0x4142, 0x4f52, 0x5420}; 	// 	text: 	SSU_v1	= {0x5353, 0x555f, 0x7631}
												//			ABORT	= {0x4142, 0x4f52, 0x5420}

	// const char symbol = 'A';
	// uint8_t position = 3;

	/* Initialization of led strips */
	Cdisplay_driver.Led_strips_init();

	// ESP_ERROR_CHECK(Cdisplay_driver.Display_symbol(symbol, color, position));
	ESP_ERROR_CHECK(Cdisplay_driver.Display_text(text, color));

	vTaskDelete(NULL);
}


/* ================================ PUBLIC METHODS  ================================ */

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

esp_err_t display_driver::Display_symbol(const char &aSymbol, const uint16_t &aColor, const uint8_t &aPosition)
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

	/* Color conversion, fill the frame_buffer, display the symbol */
	Display_driver_routine(aColor);

	return ESP_OK;	
}

esp_err_t display_driver::Display_text(const uint16_t (&aText)[3], const uint16_t &aColor)
{
	/* Extraction of symbols from text stored in holding registers */
	char symbols[6]{}; 
	Extract_symbols_from_holding_regs(aText, symbols);

	/* Set the frame_buffer_mask for all symbols*/
	for (uint8_t pos = 1; pos <= 6; ++pos)
	{
		// Iterate through symbols array to get data
		pSymbol_data = Get_symbol_data(&symbols[pos-1]);

		Set_frame_buffer_mask(pos);
	}
	
	/* Color conversion, fill the frame_buffer, display the text */
	Display_driver_routine(aColor);

	return ESP_OK;
}

// TODO Finish
esp_err_t display_driver::Blink(const size_t &aBlink_freq)
{
	if (aBlink_freq <= 0)
	{
		ESP_LOGE(TAG, "The blink frequency cannot be 0 or less!");
		return ESP_ERR_INVALID_ARG;
	}



	return ESP_OK;
}

// TODO Finish
esp_err_t display_driver::Rotate_text(const uint8_t &aRotation_speed, const bool aRotation_dir)
{
	if ((aRotation_speed <= 0) || (aRotation_speed > 1))
	{
		ESP_LOGE(TAG, "The rotation speed must be in interval ( 0 ; 1 >!");
		return ESP_ERR_INVALID_ARG;
	}



	return ESP_OK;
}


/* ================================ PRIVATE METHODS  ================================ */

p_symbol_data_t display_driver::Get_symbol_data(const char *aSymbol) const
{
	return (display_driver::ascii_tab_5x7_ + *aSymbol);
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

rgb888_pixel_t display_driver::rgb565_to_rgb888(const uint16_t &rgb_565_color)
{
	// raw values shifted to the left of the least significant byte
	uint8_t r565_SHL3 = (rgb_565_color & 0xF800) >> 8; // 0b#####000
	uint8_t g565_SHL2 = (rgb_565_color & 0x07E0) >> 3; // 0b######00
	uint8_t b565_SHL3 = (rgb_565_color & 0x001F) << 3; // 0b#####000

	// extract 3 (2) the most significant bits of raw values
	uint8_t r565_SHL3_3MSB_SHR5 = (r565_SHL3 & 0xE0) >> 5; // 0b00000###
	uint8_t g565_SHL2_2MSB_SHR6 = (g565_SHL2 & 0xC0) >> 6; // 0b000000##
	uint8_t b565_SHL3_3MSB_SHR5 = (b565_SHL3 & 0xE0) >> 5; // 0b00000###

	// append previously extracted bits to the raw values and move final color values to the correct positions
	uint8_t r = r565_SHL3 | r565_SHL3_3MSB_SHR5; // 0b########
	uint8_t g = g565_SHL2 | g565_SHL2_2MSB_SHR6; // 0b########
	uint8_t b = b565_SHL3 | b565_SHL3_3MSB_SHR5; // 0b########

	ESP_LOGI(TAG, "Conversion of color coded 5:6:5 to color coded 8:8:8 was succesful.");
	return (rgb888_pixel_t){r, g, b};
}

void display_driver::Fill_frame_buffer(const rgb888_pixel_t &aColor)
{
	for (uint8_t i = 0; i < STRIPS_NUMBER; ++i)
	{
		for (uint8_t j = 0, k = 36; j < STRIP_LEDS_NUMBER; ++j, ++k) // Filling the first half of frame_buffer
		{
			// Reversed colors - negative
			uint8_t symbol_red = ~aColor.red;
			uint8_t symbol_green = ~aColor.green;
			uint8_t symbol_blue = ~aColor.blue;
			rgb888_pixel_t symbol_color = {symbol_red, symbol_green, symbol_blue};

			frame_buffer[i][j] = frame_buffer_mask[i][j] ? symbol_color : aColor;
			frame_buffer[i][k] = frame_buffer[i][j]; // Filling the second half of frame_buffer
			// printf("%2x\t%2x\t%2x\n", aColor.red, aColor.green, aColor.blue);
			// printf("%2x\t%2x\t%2x\n\n", symbol_color.red, symbol_color.green, symbol_color.blue);
		}
	}
	ESP_LOGI(TAG, "Frame buffer filled.");
}

void display_driver::Update_display_from_frame_buffer(void)
{
	// Loading symbol/text data into all LED strips "at the same time"
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
	ESP_LOGI(TAG, "Display updated and symbol/text displayed.");
}

void display_driver::Display_driver_routine(const uint16_t &aColor)
{
	/* Color conversion from rgb565 to rgb888 */
	rgb888_pixel_t color = rgb565_to_rgb888(aColor);

	/* Fill the frame_buffer */
	Fill_frame_buffer(color);

	/* Display the text */
	Update_display_from_frame_buffer();
}