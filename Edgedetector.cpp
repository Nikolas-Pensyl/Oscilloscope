/*
 * Edgedetector.cpp
 *
 *  Created on: Aug 17, 2023
 *      Author: docca
 */

#include "Edgedetector.h"

Edge_detector::Edge_detector() {
	this->q_Debouncer = B_LOW;
	this->gpio_port = GPIOB;
	this->pin_mask = 0x0001;
}

Edge_detector::~Edge_detector() {
	// TODO Auto-generated destructor stub
}

Edge_detector::Edge_detector(const Edge_detector &other) {
	this->gpio_port = other.gpio_port;
	this->pin_mask = other.pin_mask;
	this->q_Debouncer = other.q_Debouncer;
}


void Edge_detector::config_pinout(GPIO_TypeDef *gpio_port, uint16_t pin_mask){
	this->gpio_port = gpio_port;
	this->pin_mask = pin_mask;
}


Edge_sense Edge_detector::update(void){
	Edge_sense e = EDGE_NONE_STDY_1;
	GPIO_PinState x = HAL_GPIO_ReadPin(this->gpio_port, this->pin_mask);
	switch (this->q_Debouncer){
	case B_LOW:
		if (x == GPIO_PIN_SET){
			this->q_Debouncer = B_MAY_RISE;
			e = EDGE_NONE_STDY_0;
		}
		else {
			this->q_Debouncer = B_LOW;
			e = EDGE_NONE_STDY_0;
		}
		break;
	case B_MAY_RISE:
		if (x == GPIO_PIN_SET){
			this->q_Debouncer = B_HIGH;
			e = EDGE_RISING;
		}
		else {
			this->q_Debouncer = B_LOW;
			e = EDGE_NONE_STDY_0;
		}
		break;
	case B_HIGH:
		if (x == GPIO_PIN_RESET){
			this->q_Debouncer = B_MAY_FALL;
			e = EDGE_NONE_STDY_1;
		}
		else {
			this->q_Debouncer = B_HIGH;
			e = EDGE_NONE_STDY_1;
		}
		break;
	case B_MAY_FALL:
		if (x == GPIO_PIN_RESET){
			this->q_Debouncer = B_LOW;
			e = EDGE_FALLING;
		}
		else {
			this->q_Debouncer = B_HIGH;
			e = EDGE_NONE_STDY_1;
		}
		break;
	}
	return e;
}
