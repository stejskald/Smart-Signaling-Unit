/*=====================================================================================
 * Description:
 *   This is the main C file of the project Smart Signaling Unit.
 *====================================================================================*/
#include "sdkconfig.h"					// for KConfig options
#include "eth_comm.h"					// for establishing ethernet communication
#include "display_driver.hpp"			// for led strip WS2812B display control
#include "mb_tcp_server.hpp"			// for Modbus TCP Server
#include <cstdint>

// JUST FOR DEBUGGING --->
#include "esp_log.h"
#include <cstdio>		// for standard Input/Output operations
// <---

/* The app_main entry point must have C linkage. Thus extern "C". */
extern "C" void app_main(void)
{
	// eth_connect();
	// nvs_flash_initialization(); // Not needed?
	
	// TaskHandle_t mb_server_task_handle;
	// xTaskCreate(mb_server_operation, "mb_tcp_server_task", 8192, NULL, 20, &mb_server_task_handle);

	// Takto získám data z holding registrů
	uint16_t text[3] = {0x5353, 0x555f, 0x7631}; // text: SSU_v1

	// const char symbol = '*';
	rgb888_pixel_t color = {0x00, 0x00, 0x7f};
	// uint8_t position = 1;

	display_driver display_driver;
	// display_driver.Display_symbol(symbol, color, position);
	display_driver.Display_text(text, color);

}