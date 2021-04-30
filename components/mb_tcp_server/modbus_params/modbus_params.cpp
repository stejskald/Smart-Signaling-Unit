/*=====================================================================================
 * Description:
 *   Cpp file to define parameter storage instances
 *====================================================================================*/
#include <stdint.h>
#include "modbus_params.hpp"
#include "modbus_data_types.hpp"

// Here are the user defined instances for device parameters packed by 1 byte
// These are keep the values that can be accessed from Modbus master

// Holding Registers -- 16-bit word
holding_reg_params_t holding_reg_params = { 0 };

// Input Registers -- 16-bit word
// input_reg_params_t input_reg_params = { 0 };

// Coils -- single bit
// coil_reg_params_t coil_reg_params = { 0 };

// Discrete Inputs -- single bit
// discrete_reg_params_t discrete_reg_params = { 0 };
