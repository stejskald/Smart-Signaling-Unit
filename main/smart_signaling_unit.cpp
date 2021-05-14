/*=====================================================================================
 * Description:
 *   This is the main C file of the project Smart Signaling Unit.
 *====================================================================================*/
// #include <stdio.h>					// for standard Input/Output operations
#include "sdkconfig.h"					// for KConfig options
#include "eth_comm.h"					// for establishing ethernet communication
#include "mb_tcp_server.hpp"			// for Modbus TCP Server
#include "led_strip_display_ctrl.hpp"	// for led strip WS2812B display control

// JUST FOR DEBUGGING
#include "display_controller.hpp"
#include "esp_log.h"
#include <iostream>
using std::cout;

/* The app_main entry point must have C linkage. Thus extern "C". */
extern "C" void app_main(void)
{
	// eth_connect();
	// nvs_flash_initialization(); // Not needed?
	
	// TaskHandle_t mb_server_task_handle;
	// xTaskCreate(mb_server_operation, "mb_tcp_server_task", 8192, NULL, 20, &mb_server_task_handle);

	// led_strips_init();

	// p_symbol_data_t pSymbol_data; // Row coded symbol data in 1-D array
	// p_symbol_data_t ptr_symbol_data_t;

	const char test_symbol = '#';
	rgb888_pixel_t test_color;
	test_color.red = 0x00;
	test_color.green = 0xff;
	test_color.blue = 0x00;

	display_controller disp_driver;

	// pSymbol_data = disp_driver.Get_symbol_data(test_symbol);
	// cout << "Symbol address:     " << pSymbol_data << std::endl;
	// cout << "Symbol address + 1: " << (pSymbol_data + 1) << std::endl;
	// cout << "Size of p_symbol_data_t: " << sizeof(*ptr_symbol_data_t) << std::endl;
	// cout << "Size of *pSymbol_data:   " << sizeof(*pSymbol_data) << std::endl;	// 28???, should be 7

	disp_driver.Disp_letter(test_symbol,test_color,0);

}