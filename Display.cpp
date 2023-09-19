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
	pins.port = GPIOB;
	bitwise = 0;
    source = new Single_digit_counter;  // EVIL D- so trap
    assert(false);  // Set a breakpoint here to see if this is used.
}

Display::~Display() {
	// TODO Auto-generated destructor stub
}

Display::Display(const Display &other) {
	this-> pins = other.pins;
	this->bitwise = other.bitwise;
	this->source = other.source;
}

Display::Display(const Single_digit_counter *source, const Display_config  *config){
	// MEMORIZE WIRING
	pins.port = config->port;
	for (int i = 0; i < 7; i++){
		pins.segment_mask[i] = config->segment_mask[i];
	}
	//  bit pattern: 7-segment as a byte = 0abcdefg
	bitwise = 0x01;  // NULL is a dash.

	// Data from counter:
	this->source = source;
}


void Display::seven_segmentize(uint8_t n){
	//  bit pattern: 7-segment as a byte = 0abcdefg
	switch (n){
	case 0: bitwise = 0x7E; break; //_111 1110
	case 1: bitwise = 0x30; break; //_011 0000
	case 2: bitwise = 0x6D; break; //_110 1101
	case 3: bitwise = 0x79; break; //_111 1001
	case 4: bitwise = 0x33; break; //_011 0011
	case 5: bitwise = 0x5B; break; //_101 1011
	case 6: bitwise = 0x5F; break; //_101 1111
	case 7: bitwise = 0x70; break; //_111 0000
	case 8: bitwise = 0x7F; break;
	case 9: bitwise = 0x73; break; //_111 0011
	}
}


void Display::update(){
	// Get a small integer from the source
	uint8_t n = source->count();
	// Encode it into a seven-segment pattern, packed bitwise into a byte
	seven_segmentize(n);

	// Examine each bit of the bitwise encoding
	// Assert the seven-segment display to match
	for (int n = 0; n < 7; n++){
		uint8_t mask = 1 << (6-n);
		if (mask & bitwise){
			HAL_GPIO_WritePin(pins.port, pins.segment_mask[n], GPIO_PIN_SET);
		}
		else {
			HAL_GPIO_WritePin(pins.port, pins.segment_mask[n], GPIO_PIN_RESET);
		}
	}
}
