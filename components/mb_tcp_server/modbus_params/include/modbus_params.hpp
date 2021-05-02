/*=====================================================================================
 * Description:
 *	The class used to define Modbus holding registers that can be addressed by Modbus
 *	protocol. Define these structures per your needs in your application.
 *====================================================================================*/
#ifndef _DEVICE_PARAMS
#define _DEVICE_PARAMS
#include <cstdint>
#include "modbus_data_types.hpp"

/**
 * @file modbus_params.hpp
 * @brief modbus_params class header
 * @details File defines structure of modbus parameters in Smart Signaling Unit which reflect
 * correspond modbus address space for each modbus register type (coils, discrete inputs,
 * holding registers, input registers)
 * @warning Modify this file only with the intention to change structure of modbus address space
 * @author David Stejskal
 * @version 0.1
 */

// namespace CModbus_params
// {

// #pragma pack(1) ensure that the compiler does not insert padding into the data, each member follows the previous one
#pragma pack(push, 1)
class Cmb_holding_registers // Modbus Holding Registers
{
	public:
	// TODO edit following
	CConfiguration_t holding_reg_data0;	// Running mode configuration
	CState_t holding_reg_data1;			// Displaying state
	CRgb_565_t holding_reg_data2;		// Color
	CAnimation_t holding_reg_data3;		// Animation
	}; /* class Cmb_holding_registers */
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
	uint32_t holding_data0;
	uint32_t holding_data1;
	uint32_t holding_data2;
	uint32_t holding_data3;
	float test_regs[150];
	uint32_t holding_data4;
	uint32_t holding_data5;
	uint32_t holding_data6;
	uint32_t holding_data7;
} holding_reg_params_t;
#pragma pack(pop)

// non-const global variable declaration, defined in modbus_params.cpp
extern holding_reg_params_t holding_reg_params;

// } /* namespace CModbus_params */

#endif // !defined(_DEVICE_PARAMS)
