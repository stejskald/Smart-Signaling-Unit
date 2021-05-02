/*=====================================================================================
 * Description:
 *   This is the main C file of the project Smart Signaling Unit.
 *====================================================================================*/
//#include <stdio.h>			// for standard Input/Output operations
#include "sdkconfig.h"			// for KConfig options
#include "eth_comm.h"			// for establishing ethernet communication
#include "mb_tcp_server.hpp"	// for Modbus TCP Server
#include "led_strip_ctrl.h"		// for led strip WS2812B contorl

/* The app_main entry point must have C linkage. Thus extern "C". */
extern "C" void app_main(void)
{
	eth_connect();

	nvs_flash_initialization();
	
	xTaskCreate(mb_server_operation, "mb_tcp_server_task", 8192, NULL, 20, NULL);

	// TODO: repair the unhandled exceptions during eth_disconnect()
	// vTaskDelay(500);
	// eth_disconnect();
}