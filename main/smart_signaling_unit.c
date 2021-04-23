#include "sdkconfig.h"		// for KConfig options
#include "eth_comm.h"		// for establishing ethernet communication
#include "mb_tcp_server.h"	// for Modbus TCP Server
#include "led_strip_ctrl.h"	// for led strip WS2812B contorl

void app_main(void)
{
	nvs_flash_initialization();
	
	eth_connect();
	// TODO: repair the unhandled exceptions during eth_disconnect()
	// vTaskDelay(500);
	// eth_disconnect();


}