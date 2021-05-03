/*=====================================================================================
 * Description:
 *	The Modbus data types (classes) used to define Modbus instances that can be
 *	addressed by Modbus protocol. Define these classes per your needs in your
 *	application.
 *====================================================================================*/
#ifndef __MODBUS_DATA_TYPES_HPP__
#define __MODBUS_DATA_TYPES_HPP__
/**
 * @brief Enumeration of states according to PackML standard
 * 
 */
enum class EState_t:uint8_t	// underlying type of enum members is uint8_t
{
	EClearing,
	EStopped,
	EStarting,
	EIdle,
	ESuspended,
	EExecute,
	EStopping,
	EAborting,
	EAborted,
	EHolding,
	EHeld,
	EUnholding,
	ESuspending,
	EUnsuspending,
	EReseting,
	ECompleting,
	EComplete
};

#pragma pack(push, 1)
class CConfiguration_t
{
	public:
		uint8_t one_section:1;		// 1 section
		uint8_t two_sections:1;		// 2 sections
		uint8_t three_sections:1;	// 3 sections
		uint8_t four_sections:1;	// 4 sections
		uint8_t five_sections:1;	// 5 sections
		uint8_t six_sections:1;		// 6 sections
		uint8_t seven_sections:1;	// 7 sections (text mode)
};
#pragma pack(pop)

class CState_t
{
	public:
		uint8_t state_id;
};

#pragma pack(push, 1)
class CRgb_565_t
{
	public:
		uint16_t blue:5;
		uint16_t green:6;
		uint16_t red:5;
};
#pragma pack(pop)

#pragma pack(push, 1)
class CAnimation_t
{
	public:
		uint8_t anim_off:1;		// off
		uint8_t anim_steady:1;	// steady on
		uint8_t anim_blink:1;	// blinking
		uint8_t anim_rrot:1;	// right rotation
		uint8_t anim_lrot:1;	// left rotation
};
#pragma pack(pop)

#endif /* __MODBUS_DATA_TYPES_HPP__ */