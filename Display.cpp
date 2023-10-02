/*
 * Display.cpp
 *
 *  Created on: Aug 18, 2023
 *      Author: docca
 */

#include "Display.h"

// This default constructor shouldn't be used! Assigning source ptr = 0
// (without memory protection) is very dangerous. The
Display::Display() {
//	pins.port = GPIOB;
//	bitwise = 0;
//    source = new Single_digit_counter;  // EVIL D- so trap
	//port = GPIOA;
    assert(false);  // Set a breakpoint here to see if this is used.
}

Display::~Display() {
	// TODO Auto-generated destructor stub
}

Display::Display(const Display &other) {
	//this->port = other.port;
}

Display::Display(GPIO_TypeDef port1){
	//port = port1;
}


void Display::init(SPI_HandleTypeDef hspi1){

	//Drive CD Pin Low to feed commands
	HAL_GPIO_WritePin(GPIOA, 0x0008, GPIO_PIN_RESET);

	//Set Scroll line to 0 :: 0100_0000
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0x40, 2, HAL_MAX_DELAY);

	//Set SEG direction to reverse :: 1010_0001
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0xA1, 2, HAL_MAX_DELAY);

	//Set COM direction to normal :: 1100_0000
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0xC0, 2, HAL_MAX_DELAY);

	//Set all pixels to ON :: 1010_0010
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0xA4, 2, HAL_MAX_DELAY);

	//Set inverse display to OFF :: 1010_0110
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0xA6, 2, HAL_MAX_DELAY);

	//Set LCD Bias Ratio to 1/9 :: 1010_0010
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0xA2, 2, HAL_MAX_DELAY);

	//Set Power Settings to all ON :: 0010_1111
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0x2F, 2, HAL_MAX_DELAY);

	//Set VLCD Resistor Ratio(For contrast) :: 0010_0111
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0x27, 2, HAL_MAX_DELAY);

	//Set Electronic Volume(Two-part command, for contrast) :: 1000_0001, 0001_0000
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0x81, 2, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0x10, 2, HAL_MAX_DELAY);

	//Set Temperature Compensation(Two-part command) :: 1111_1010, 1001_0000
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0xFA, 2, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0x90, 2, HAL_MAX_DELAY);

	//Set Display Enable to ON :: 1010_1111
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0xAF, 2, HAL_MAX_DELAY);

	//Set Scroll line to 0
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0x40, 2, HAL_MAX_DELAY);

	//Set page to 0
	HAL_SPI_Transmit(&hspi1, (uint8_t*) 0xB0, 2, HAL_MAX_DELAY);


	//Set CD Line high to indicate data
	HAL_GPIO_WritePin(GPIOA, 0x0008, GPIO_PIN_SET);
}

void Display::update(){
	// Get a small integer from the source
//	uint8_t n = source->count();
//	// Encode it into a seven-segment pattern, packed bitwise into a byte
//	seven_segmentize(n);
//
//	// Examine each bit of the bitwise encoding
//	// Assert the seven-segment display to match
//	for (int n = 0; n < 7; n++){
//		uint8_t mask = 1 << (6-n);
//		if (mask & bitwise){
//			HAL_GPIO_WritePin(pins.port, pins.segment_mask[n], GPIO_PIN_SET);
//		}
//		else {
//			HAL_GPIO_WritePin(pins.port, pins.segment_mask[n], GPIO_PIN_RESET);
//		}
//	}
}
