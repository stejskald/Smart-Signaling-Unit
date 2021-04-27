#include "esp_err.h"		// for error handling
#include "esp_log.h"		// for logging functionalities

#include "eth_comm.h"		// for implementation of common functions for Ethernet communication

#include "nvs_flash.h"		// for storing key-value pairs in flash

#include "mb_tcp_server.h"	// for Modbus TCP Server implementation
#include "mbcontroller.h"	// for mbcontroller defines and api
#include "modbus_params.h"	// for modbus parameters structures

#define MB_TCP_PORT_NUMBER      (CONFIG_FMB_TCP_PORT_DEFAULT)

// Defines below are used to define register start address for each type of Modbus registers
#define HOLD_OFFSET(field) ((uint16_t)(offsetof(holding_reg_params_t, field) >> 1))
#define INPUT_OFFSET(field) ((uint16_t)(offsetof(input_reg_params_t, field) >> 1))
#define MB_REG_DISCRETE_INPUT_START         (0x0000)
#define MB_REG_COILS_START                  (0x0000)
#define MB_REG_INPUT_START_AREA0            (INPUT_OFFSET(input_data0)) // register offset input area 0
#define MB_REG_INPUT_START_AREA1            (INPUT_OFFSET(input_data4)) // register offset input area 1
#define MB_REG_HOLDING_START_AREA0          (HOLD_OFFSET(holding_data0))
#define MB_REG_HOLDING_START_AREA1          (HOLD_OFFSET(holding_data4))

static const char *TAG = "mb_server";

void mb_server_test(void){
	esp_log_level_set(TAG, ESP_LOG_INFO); // Set UART log level

	void *mbc_slave_handler = NULL;
	ESP_ERROR_CHECK(mbc_slave_init_tcp(&mbc_slave_handler)); // Initialization of Modbus controller

	mb_param_info_t reg_info; // keeps the Modbus registers access information
	mb_register_area_descriptor_t reg_area; // Modbus register area descriptor structure

	mb_communication_info_t comm_info = { 0 };
	comm_info.ip_port = MB_TCP_PORT_NUMBER;
	comm_info.ip_addr_type = MB_IPV4;
	comm_info.ip_mode = MB_MODE_TCP;
    comm_info.ip_addr = NULL;
    comm_info.ip_netif_ptr = (void*)get_netif();
    // Setup communication parameters and start stack
    ESP_ERROR_CHECK(mbc_slave_setup((void*)&comm_info));

	// The code below initializes Modbus register area descriptors for Modbus Holding Registers, Input Registers,
	// Coils and Discrete Inputs. Initialization should be done for each supported Modbus register area according
	// to register map. When external master trying to access the register in the area that is not initialized
    // by mbc_slave_set_descriptor() API call, then Modbus stack will send exception response for this register area.
 	// Initialization of Holding Registers area
	reg_area.type = MB_PARAM_HOLDING; // Set type of register area
    reg_area.start_offset = MB_REG_HOLDING_START_AREA0; // Offset of register area in Modbus protocol
    reg_area.address = (void*)&holding_reg_params.holding_data0; // Set pointer to storage instance
    reg_area.size = sizeof(float) << 2; // Set the size of register storage instance
    ESP_ERROR_CHECK(mbc_slave_set_descriptor(reg_area));
	// Initialization of Holding Registers area
    reg_area.type = MB_PARAM_HOLDING; // Set type of register area
    reg_area.start_offset = MB_REG_HOLDING_START_AREA1; // Offset of register area in Modbus protocol
    reg_area.address = (void*)&holding_reg_params.holding_data4; // Set pointer to storage instance
    reg_area.size = sizeof(float) << 2; // Set the size of register storage instance
    ESP_ERROR_CHECK(mbc_slave_set_descriptor(reg_area));

    // Initialization of Input Registers area
    reg_area.type = MB_PARAM_INPUT;
    reg_area.start_offset = MB_REG_INPUT_START_AREA0;
    reg_area.address = (void*)&input_reg_params.input_data0;
    reg_area.size = sizeof(float) << 2;
    ESP_ERROR_CHECK(mbc_slave_set_descriptor(reg_area));
    // Initialization of Input Registers area
    reg_area.type = MB_PARAM_INPUT;
    reg_area.start_offset = MB_REG_INPUT_START_AREA1;
    reg_area.address = (void*)&input_reg_params.input_data4;
    reg_area.size = sizeof(float) << 2;
    ESP_ERROR_CHECK(mbc_slave_set_descriptor(reg_area));

    // Initialization of Coils register area
    reg_area.type = MB_PARAM_COIL;
    reg_area.start_offset = MB_REG_COILS_START;
    reg_area.address = (void*)&coil_reg_params;
    reg_area.size = sizeof(coil_reg_params);
    ESP_ERROR_CHECK(mbc_slave_set_descriptor(reg_area));

    // Initialization of Discrete Inputs register area
    reg_area.type = MB_PARAM_DISCRETE;
    reg_area.start_offset = MB_REG_DISCRETE_INPUT_START;
    reg_area.address = (void*)&discrete_reg_params;
    reg_area.size = sizeof(discrete_reg_params);
    ESP_ERROR_CHECK(mbc_slave_set_descriptor(reg_area));
}










void nvs_flash_initialization(void){
	esp_err_t result = nvs_flash_init(); // NVS ... Non-volatile Storage
	if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		result = nvs_flash_init();
	}
	ESP_ERROR_CHECK(result);
}