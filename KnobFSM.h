/*
 * knobFSM.h
 *
 *  Created on: Aug 17, 2023
 *      Author: docca
 */

#ifndef SRC_KNOBFSM_H_
#define SRC_KNOBFSM_H_

#include "Edgedetector.h"
#include "Sean_queue.h"

enum {KNOB_STEADY, KNOB_CW, KNOB_CCW};  // I do not think that I'll need STEADY, but it's here just-in-case.

class Knob_FSM {

public:
	Knob_FSM(Sean_queue *q_out, Sean_queue *q_in_Sample_now, GPIO_TypeDef *A_gpio_port, uint16_t A_pin_mask,GPIO_TypeDef *B_gpio_port, uint16_t B_pin_mask);
	virtual ~Knob_FSM();
	Knob_FSM(const Knob_FSM &other);
	void update(void);

private:
	Sean_queue *q_in_Sample_now;
	Edge_detector pinA_debounced_edge;
	Edge_detector pinB_debounced_edge;
	Sean_queue *result;
};

#endif /* SRC_KNOBFSM_H_ */
