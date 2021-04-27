/*=====================================================================================
 * Description:
 *   H file to ...
 *====================================================================================*/
#ifndef __MB_TCP_SERVER_H__
#define __MB_TCP_SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set up the register values into known state
 * 
 */
void setup_reg_data(void);

/**
 * @brief Initialization of NVS Flash
 * 
 */
void nvs_flash_initialization(void);

/**
 * @brief Initialization of Modbus register area descriptors for Modbus Holding Registers, Input Registers,
 * Coils and Discrete Inputs.
 * 
 * Initialization should be done for each supported Modbus register area according
 * to register map. When external master trying to access the register in the area that is not initialized
 * by mbc_slave_set_descriptor() API call, then Modbus stack will send exception response for this register area.
 * 
 */
void mb_reg_init(void);

/**
 * @brief Test functionality of mb_server
 * 
 */
void mb_server_test(void);

#ifdef __cplusplus
}
#endif

#endif /* __MB_TCP_SERVER_H__ */