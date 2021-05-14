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
enum class EState_t : uint8_t // underlying type of enum members is uint8_t
{
	EClearing,		// = "Clearn",
	EStopped,		// = "Stoppd",
	EStarting,		// = "Startn",
	EIdle,			// = "Idle",
	ESuspended,		// = "Sspndd",
	EExecute,		// = "Execut",
	EStopping,		// = "Stoppn",
	EAborting,		// = "Abortn",
	EAborted,		// = "Abortd",
	EHolding,		// = "Holdin",
	EHeld,			// = "Held",
	EUnholding,		// = "Unhldn",
	ESuspending,	// = "Sspndn",
	EUnsuspending,	// = "Uspndn",
	EReseting,		// = "Resetn",
	ECompleting,	// = "Cmpltn",
	EComplete		// = "Complt"
};

/* Note:
 *	A block of many registers must be reserved for a long text string.
 *	One character is 1 B, so for 6 chars long string it is 6 B, which
 *	equal to 3 holding registers.
 */

// TODO Row/Text mode

#pragma pack(push, 1)
class CConfiguration_t
{
public:
	// lower byte
	uint8_t no_module : 1;	   // 0 modules = LSB
	uint8_t one_module : 1;	   // 1 module
	uint8_t two_modules : 1;   // 2 modules
	uint8_t three_modules : 1; // 3 modules
	uint8_t four_modules : 1;  // 4 modules
	uint8_t five_modules : 1;  // 5 modules
	uint8_t six_modules : 1;   // 6 modules
	uint8_t seven_modules : 1; // 7 modules = MSB

	// higher byte
	uint8_t modular_mode : 1;
	uint8_t text_mode : 1;
	uint8_t unused0 : 2;
	uint8_t state_mode : 1;
	uint8_t manual_mode : 1;
	uint8_t unused1 : 2;
};
#pragma pack(pop)

#pragma pack(push, 1)
class CRgb_565_t
{
public:
	uint16_t blue : 5;
	uint16_t green : 6;
	uint16_t red : 5;
};
#pragma pack(pop)

#pragma pack(push, 1)
class CAnimation_t
{
public:
	uint8_t off : 1;	// off
	uint8_t steady : 1; // steady on
	uint8_t blink : 1;	// blinking
	uint8_t rrot : 1;	// right rotation
	uint8_t lrot : 1;	// left rotation
};
#pragma pack(pop)

#endif /* __MODBUS_DATA_TYPES_HPP__ */