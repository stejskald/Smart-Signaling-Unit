#include <stdint.h>
#include "display_controller.hpp"	// for ...

#include <iostream>
using std::cout;

// uint8_t *pSymbol_data[STRIPS_NUMBER]; // pointer at the data of desired 5x7 ASCII symbol to display

p_symbol_data_t display_controller::Get_symbol_data(const char aSymbol) const
{
	// get the address of symbol data from the table
	return (p_symbol_data_t)(display_controller::ascii_tab_5x7_ + aSymbol);
}

// p_symbol_data_t *pSymbol_data; // Row coded symbol data in 1-D array

void display_controller::Disp_letter(const char aSymbol, rgb888_pixel_t aColor, uint8_t aPosition)
{
	// TODO Make conversion from rgb565 to rgb888 later
	
	// Get_symbol_data
	// const uint8_t (*pSymbol_data)[7] = (display_controller::ascii_tab_5x7_ + aSymbol); // position of desired symbol

	cout << "Base address of ascii_tab_5x7_: " << display_controller::ascii_tab_5x7_ << std::endl;
	cout << "Address of aSymbol:             " << (display_controller::ascii_tab_5x7_ + aSymbol) << std::endl;
	// cout << "Size of aSymbol data:  " << sizeof(*(display_controller::ascii_tab_5x7_ + aSymbol)) << std::endl;
	// cout << "Size of *pSymbol_data: " << sizeof(*pSymbol_data) << std::endl;

	
	// Fill the frame_buffer
	// display_controller::frame_buffer_[1][0];



// 	// TODO Continue here -- verify the for loops
// 	for (size_t i = 0; i < STRIPS_NUMBER; i++)
// 	{
// 		// Fill the first half of the buffer
// 		for (size_t j = 0; j < (STRIP_LEDS_NUMBER); j++)
// 		{
// 			// aDisp_buffer[i][j] = upSymbol_data[i][j]
// 		}
// 		// Fill the second half of the buffer
// 		for (size_t k = 0; k < (STRIP_LEDS_NUMBER); k++)
// 		{
// 			// aDisp_buffer[i][k + STRIP_LEDS_NUMBER] = upSymbol_data[i][k];
// 		}
// 	}	
}