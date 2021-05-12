/*=====================================================================================
 * Description:
 *   Cpp file to ...
 *====================================================================================*/
#include "esp_err.h"			// for error handling
#include "esp_log.h"			// for logging functionalities

#include "eth_comm.h"			// for implementation of common functions for Ethernet communication

#include "nvs_flash.h"			// for storing key-value pairs in flash

#include "modbus_params.hpp"	// for modbus parameters structures
#include "mb_tcp_server.hpp"	// for Modbus TCP Server implementation
#include "mbcontroller.h"		// for mbcontroller defines and api


#define MB_TCP_PORT_NUMBER      (CONFIG_FMB_TCP_PORT_DEFAULT)

// Defines used to define Modbus holding registers' start address
#define HOLD_OFFSET(field)		((uint16_t)(offsetof(Cmb_holding_registers, field) >> 1))
// #define HOLD_OFFSET(field)		((uint16_t)(offsetof(holding_reg_params_t, field) >> 1))
#define MB_REG_HOLDING_START	(HOLD_OFFSET(desired_state))

/* TODO Example define - after testing remove/replace */
#define MB_PAR_INFO_GET_TOUT	(10) // Timeout for get parameter info

/* Defines for read/write/read&write masks */
#define MB_READ_MASK			(MB_EVENT_INPUT_REG_RD \
									| MB_EVENT_HOLDING_REG_RD \
									| MB_EVENT_DISCRETE_RD \
									| MB_EVENT_COILS_RD)
#define MB_WRITE_MASK			(MB_EVENT_HOLDING_REG_WR \
									| MB_EVENT_COILS_WR)
#define MB_READ_WRITE_MASK		(MB_READ_MASK | MB_WRITE_MASK)

static const char *TAG = "mb_server";
static Cmb_holding_registers mb_holding_registers;

// void mb_event_handler(void *arg, esp_event_base_t event_base, EMb_event_t event_id, void *event_data)
// {
// 	// can get the ethernet driver handle from event data
// 	//esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;

// 	switch (event_id) {
// 	case EMb_event_t::MODBUS_EVENT_START:
// 		ESP_LOGI(TAG, "Modbus Up");
// 		break;
// 	case EMb_event_t::MODBUS_EVENT_STOP:
// 		ESP_LOGI(TAG, "Modbus Down");
// 		break;
// 	default:
// 		break;
// 	}
// }

/* Initialization of Modbus register area descriptor for Modbus Holding Registers */
void mb_server_init(void)
{
	// esp_log_level_set(TAG, ESP_LOG_INFO); // Set logging level to informative

	void *mbc_slave_handler = NULL;
	// Initialization of Modbus controller and stack for TCP port
	ESP_ERROR_CHECK(mbc_slave_init_tcp(&mbc_slave_handler));

	mb_communication_info_t comm_info; // Modbus communication parameters
	comm_info.ip_mode = MB_MODE_TCP;
	comm_info.ip_port = MB_TCP_PORT_NUMBER;
	comm_info.ip_addr_type = MB_IPV4;
	comm_info.ip_addr = NULL;
	comm_info.ip_netif_ptr = (void *)get_netif();
	// Setup communication parameters and start stack
	ESP_ERROR_CHECK(mbc_slave_setup((void *)&comm_info));

	/* Initialization of Modbus register area descriptors for Modbus Holding Registers
	 *
	 * Initialization should be done for each supported Modbus register area according to register map.
	 * When external client trying to access the register in the area that is not initialized by
	 * mbc_slave_set_descriptor() API call, then Modbus stack will send exception response for this register area.
	 */
	mb_register_area_descriptor_t reg_area;	// Modbus registers area descriptor structure
	// Initialization of Holding Registers area
	reg_area.type = MB_PARAM_HOLDING;					// Set type of register area
	reg_area.start_offset = MB_REG_HOLDING_START;		// Offset of register area in Modbus protocol
	reg_area.address = (void *)&mb_holding_registers;	// Set pointer to storage instance
	// TODO Replace the hardcoded number ?-> (size_t)(HOLD_OFFSET(help_reg) - HOLD_OFFSET(desired_state));
	reg_area.size = (size_t)(2*27);
	ESP_ERROR_CHECK(mbc_slave_set_descriptor(reg_area));
}

// TODO Hardcode the names of states for State Mode
/* Set up the register values into known state */
void setup_reg_data(void)
{
	// TODO Init the holding regs
	mb_holding_registers.desired_state	= 0x0001;
	mb_holding_registers.desired_module	= 0x0002;
	
	mb_holding_registers.configuration.four_modules	= 1;
	mb_holding_registers.configuration.modular_mode = 1;
	mb_holding_registers.configuration.state_mode = 1;

	mb_holding_registers.reserved0	= 0;
	mb_holding_registers.reserved1	= 0;
	mb_holding_registers.reserved2	= 0;

	mb_holding_registers.module1_state_id	= 0;
	mb_holding_registers.module2_state_id	= 1;
	mb_holding_registers.module3_state_id	= 2;
	mb_holding_registers.module4_state_id	= 3;
	mb_holding_registers.module5_state_id	= 4;
	mb_holding_registers.module6_state_id	= 5;
	mb_holding_registers.module7_state_id	= 6;

// TODO Other modules' color...
	mb_holding_registers.module1_color.red		= 0b11111;
	mb_holding_registers.module1_color.green	= 0b000000;
	mb_holding_registers.module1_color.blue		= 0b00000;

	mb_holding_registers.module2_color.red		= 0b00000;
	mb_holding_registers.module2_color.green	= 0b111111;
	mb_holding_registers.module2_color.blue		= 0b00000;

	mb_holding_registers.module3_color.red		= 0b00000;
	mb_holding_registers.module3_color.green	= 0b000000;
	mb_holding_registers.module3_color.blue		= 0b11111;
	
	// TODO Other modules' animation...
	mb_holding_registers.module1_animation.steady	= 1;
	mb_holding_registers.module2_animation.steady	= 1;
	mb_holding_registers.module3_animation.steady	= 1;
	mb_holding_registers.module4_animation.blink	= 1;
	

	// uint16_t help_reg = {};
}

void mb_server_operation(void *arg)
{
	mb_param_info_t reg_info;	// keeps the Modbus registers access information

	/* Initialization of Modbus register area descriptor for Modbus Holding Registers */
	mb_server_init();

	/* Set values into known state */
	setup_reg_data();

	// Register Modbus event handler (to deal with user specific stuffs when event like Modbus up/down happened)
	// ESP_ERROR_CHECK(esp_event_handler_register(MB_EVENT, ESP_EVENT_ANY_ID, &mb_event_handler, NULL));

	/* Starts of Modbus controller and stack */
	ESP_ERROR_CHECK(mbc_slave_start());
	ESP_LOGI(TAG, "Modbus server started.");

	//esp_netif_t *my_netif = get_netif();
	// BUG How to run until eth disconnect?
	// while(esp_netif_is_netif_up(my_netif))
	// {
		for (uint8_t iter = 0; iter < 3; iter++)
		{
		// Check for read/write events of Modbus master for certain events
		mb_event_group_t event = mbc_slave_check_event(static_cast<mb_event_group_t>(MB_READ_WRITE_MASK));
		const char *rw_str = (event & MB_READ_MASK) ? "READ" : "WRITE";
		// Filter events and process them accordingly
		if (event & (MB_EVENT_HOLDING_REG_WR | MB_EVENT_HOLDING_REG_RD))
		{
			// Get parameter information from parameter queue
			ESP_ERROR_CHECK(mbc_slave_get_param_info(&reg_info, MB_PAR_INFO_GET_TOUT));
			ESP_LOGI(TAG, "HOLDING %s (%u us), ADDR:%u, TYPE:%u, INST_ADDR:0x%.4x, SIZE:%u",
					 rw_str,
					 (uint32_t)reg_info.time_stamp,
					 (uint32_t)reg_info.mb_offset,
					 (uint32_t)reg_info.type,
					 (uint32_t)reg_info.address,
					 (uint32_t)reg_info.size);
		}
		// if (true) break; // TODO If eth disconnect, break
		}
	// }

	/* Destroy of Modbus controller on alarm */
	// if(!esp_netif_is_netif_up(my_netif))
	// {
		mb_server_stop();
		vTaskDelete(NULL);
	// }
}

void mb_server_stop(void)
{
	ESP_ERROR_CHECK(mbc_slave_destroy());
	ESP_LOGI(TAG,"Modbus controller destroyed.");
	// ESP_ERROR_CHECK(esp_event_handler_unregister(MB_EVENT, ESP_EVENT_ANY_ID, &mb_event_handler));
}

// void nvs_flash_initialization(void){
// 	esp_err_t result = nvs_flash_init(); // NVS ... Non-volatile Storage
// 	if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
// 		ESP_ERROR_CHECK(nvs_flash_erase());
// 		result = nvs_flash_init();
// 	}
// 	ESP_ERROR_CHECK(result);
// }