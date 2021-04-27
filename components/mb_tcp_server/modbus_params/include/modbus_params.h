/*=====================================================================================
 * Description:
 *	The Modbus parameter structures used to define Modbus instances that can be
 *	addressed by Modbus protocol. Define these structures per your needs in your
 *	application. Below is just an example of possible parameters.
 *====================================================================================*/
#ifndef _DEVICE_PARAMS
#define _DEVICE_PARAMS

/*
* This file defines structure of modbus parameters which reflect correspond modbus address space
* for each modbus register type (coils, discreet inputs, holding registers, input registers)
*/
// TODO Implement Data Model of SSU
#pragma pack(push, 1)
typedef struct
{
	uint8_t discrete_input0:1;
	uint8_t discrete_input1:1;
	uint8_t discrete_input2:1;
	uint8_t discrete_input3:1;
	uint8_t discrete_input4:1;
	uint8_t discrete_input5:1;
	uint8_t discrete_input6:1;
	uint8_t discrete_input7:1;
	uint8_t discrete_input_port1:8;
} discrete_reg_params_t;
#pragma pack(pop)
// #pragma pack(1) ensure that the compiler does not insert padding into the data, each member follows the previous one
#pragma pack(push, 1)
typedef struct
{
	uint8_t coils_port0;
	uint8_t coils_port1;
} coil_reg_params_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
	float input_data0; // 0
	float input_data1; // 2
	float input_data2; // 4
	float input_data3; // 6
	uint16_t data[150]; // 8 + 150 = 158
	float input_data4; // 158
	float input_data5;
	float input_data6;
	float input_data7;
} input_reg_params_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
	float holding_data0;
	float holding_data1;
	float holding_data2;
	float holding_data3;
	uint16_t test_regs[150];
	float holding_data4;
	float holding_data5;
	float holding_data6;
	float holding_data7;
} holding_reg_params_t;
#pragma pack(pop)

extern holding_reg_params_t holding_reg_params;
extern input_reg_params_t input_reg_params;
extern coil_reg_params_t coil_reg_params;
extern discrete_reg_params_t discrete_reg_params;

#endif // !defined(_DEVICE_PARAMS)
