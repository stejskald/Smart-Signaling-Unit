/*=====================================================================================
 * Description:
 *   This is the main C file of the project Smart Signaling Unit.
 *====================================================================================*/
#include "sdkconfig.h"					// for KConfig options
#include "eth_comm.h"					// for establishing ethernet communication
#include "display_driver.hpp"			// for led strip WS2812B display control
#include "mb_tcp_server.hpp"			// for Modbus TCP Server

// JUST FOR DEBUGGING --->
// #include <stdio.h>					// for standard Input/Output operations
#include "esp_log.h"
#include <iostream>
using std::cout;
// <---

/* The app_main entry point must have C linkage. Thus extern "C". */
extern "C" void app_main(void)
{
	// eth_connect();
	// nvs_flash_initialization(); // Not needed?
	
	// TaskHandle_t mb_server_task_handle;
	// xTaskCreate(mb_server_operation, "mb_tcp_server_task", 8192, NULL, 20, &mb_server_task_handle);


	const char symbol = '@';
	rgb888_pixel_t color = {0x00, 0xff, 0x00};

	display_driver display_driver;
	// display_driver.Led_strips_init();
	display_driver.Disp_letter(symbol,color,0);

}