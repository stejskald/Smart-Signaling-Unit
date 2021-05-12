/*=====================================================================================
 * Description:
 *   Cpp file to define parameter storage instances
 *====================================================================================*/
#include <stdint.h>
#include "modbus_params.hpp"
#include "modbus_data_types.hpp"

/**
 * @brief 	Here are the user defined instances for device parameters packed by 1 byte.
 * 			These are keep the values that can be accessed from Modbus client.
 * 			16-bit Holding Registers
 * 
 */
Cmb_holding_registers mb_holding_registers = {}; // default initialization
