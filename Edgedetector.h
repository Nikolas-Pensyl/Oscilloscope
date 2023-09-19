/*
 * Edgedetector.h
 *
 *  Created on: Aug 17, 2023
 *      Author: docca
 */

#ifndef SRC_EDGEDETECTOR_H_
#define SRC_EDGEDETECTOR_H_

#include "main.h"

typedef enum {EDGE_NONE_STDY_1, EDGE_NONE_STDY_0, EDGE_RISING, EDGE_FALLING} Edge_sense;

class Edge_detector {
private:
	typedef enum {B_LOW, B_MAY_RISE, B_HIGH, B_MAY_FALL} Debounce_state;
	Debounce_state q_Debouncer;
	GPIO_TypeDef *gpio_port;
	uint16_t pin_mask;

public:
	Edge_detector();
	virtual ~Edge_detector();
	Edge_detector(const Edge_detector &other);

	void config_pinout(GPIO_TypeDef *gpio_port, uint16_t pin_mask);
	Edge_sense update();
};

#endif /* SRC_EDGEDETECTOR_H_ */
