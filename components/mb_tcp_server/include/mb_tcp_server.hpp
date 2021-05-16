/*=====================================================================================
 * Description:
 *   Hpp file to ...
 *====================================================================================*/
#ifndef __MB_TCP_SERVER_HPP__
#define __MB_TCP_SERVER_HPP__
#include "modbus_data_types.hpp"

// TODO Make class mb_tcp_server

/**
 * @brief Initialization of Modbus TCP server
 * 
 */
void mb_server_init(void);

/**
 * @brief Set up the register values into known state
 * 
 */
void setup_reg_data(void);

/**
 * @brief Modbus TCP server operation
 * 
 * @param arg 
 */
void mb_server_operation(void *arg); // TODO Needed arg?

/**
 * @brief Stop the Modbus TCP server
 * 
 */
void mb_server_stop(void);

/**
 * @brief Initialization of NVS Flash
 * 
 */
void nvs_flash_initialization(void);



// /**
// * @brief Modbus event declarations
// *
// */
// enum class EMb_event_t : uint32_t
// {
//     MODBUS_EVENT_START,		/*!< Modbus driver start */
//     MODBUS_EVENT_STOP		/*!< Modbus driver stop */
// };

// /**
//  * @brief Event handler for Modbus events
//  * 
//  * @param arg 
//  * @param event_base 
//  * @param event_id 
//  * @param event_data 
//  */
// void mb_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

// /**
// * @brief Modbus event base declaration
// *
// */
// ESP_EVENT_DECLARE_BASE(MB_EVENT);

#endif /* __MB_TCP_SERVER_HPP__ */