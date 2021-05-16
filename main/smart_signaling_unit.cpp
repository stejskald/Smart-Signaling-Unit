/*=====================================================================================
 * Description:
 *   This is the main C file of the project Smart Signaling Unit.
 *====================================================================================*/
#include "sdkconfig.h"			// for KConfig options
#include "eth_comm.h"			// for establishing ethernet communication
#include "display_driver.hpp"	// for led strip WS2812B display control
#include "mb_tcp_server.hpp"	// for Modbus TCP Server
#include <cstddef>				// for standard definitions

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

	TaskHandle_t led_display_driver_handle;
	xTaskCreate(led_display_driver_operation, "led_display_driver_task", 8192, NULL, 20, &led_display_driver_handle);
}