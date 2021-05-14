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
 * 			correspond modbus address space for each modbus register type (coils, discrete inputs,
 * 			holding registers, input registers)
 * @warning Modify this file only with the intention to change structure of modbus address space
 * @author David Stejskal
 * @version 0.1
 */

// #pragma pack(1) ensure that the compiler does not insert padding into the data, each member follows the previous one
#pragma pack(push, 1)
class Cmb_holding_registers // Modbus Holding Registers
{
	public:
		uint16_t desired_state;			// address 0x0000
		uint16_t desired_module;		// address 0x0001
		CConfiguration_t configuration;	// address 0x0002

		uint16_t reserved0;				// address 0x0003
		uint16_t reserved1;				// address 0x0004
		uint16_t reserved2;				// address 0x0005

		uint16_t module1_state_id;		// address 0x0006
		uint16_t module2_state_id;		// address 0x0007
		uint16_t module3_state_id;		// address 0x0008
		uint16_t module4_state_id;		// address 0x0009
		uint16_t module5_state_id;		// address 0x000a
		uint16_t module6_state_id;		// address 0x000b
		uint16_t module7_state_id;		// address 0x000c

		CRgb_565_t module1_color;		// address 0x000d
		CRgb_565_t module2_color;		// address 0x000e
		CRgb_565_t module3_color;		// address 0x000f
		CRgb_565_t module4_color;		// address 0x0010
		CRgb_565_t module5_color;		// address 0x0011
		CRgb_565_t module6_color;		// address 0x0012
		CRgb_565_t module7_color;		// address 0x0013

		CAnimation_t module1_animation;	// address 0x0014
		CAnimation_t module2_animation;	// address 0x0015
		CAnimation_t module3_animation;	// address 0x0016
		CAnimation_t module4_animation;	// address 0x0017
		CAnimation_t module5_animation;	// address 0x0018
		CAnimation_t module6_animation;	// address 0x0019
		CAnimation_t module7_animation;	// address 0x001a

		// uint16_t help_reg;
}; /* class Cmb_holding_registers */
#pragma pack(pop)

// non-const global variable declaration, defined in modbus_params.cpp
extern Cmb_holding_registers mb_holding_registers;



// #pragma pack(push, 1)
// typedef struct
// {
// 	uint16_t holding_data0;
// 	uint16_t holding_data1;
// 	uint16_t holding_data2;
// 	uint16_t holding_data3;
// 	uint16_t holding_data4;
// 	uint16_t holding_data5;
// 	uint16_t holding_data6;
// 	uint16_t holding_data7;
// } holding_reg_params_t;
// #pragma pack(pop)

// extern holding_reg_params_t holding_reg_params;

#endif // !defined(_DEVICE_PARAMS)
