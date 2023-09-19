/*
 * knobFSM.cpp
 *
 *  Created on: Aug 17, 2023
 *      Author: docca
 */

#include "KnobFSM.h"

////////////// For debus info only /////////////////
//extern int16_t debug_mailbox;
////////////////////////////////////////////////////

Knob_FSM::Knob_FSM(Sean_queue *q_out, Sean_queue *q_in_Sample_now, GPIO_TypeDef *A_gpio_port, uint16_t A_pin_mask,GPIO_TypeDef *B_gpio_port, uint16_t B_pin_mask) {
	this->q_in_Sample_now = q_in_Sample_now;
	this->result = q_out;
	this->pinA_debounced_edge.config_pinout(A_gpio_port, A_pin_mask);
	this->pinB_debounced_edge.config_pinout(B_gpio_port, B_pin_mask);
}

Knob_FSM::~Knob_FSM() {
	// TODO Auto-generated destructor stub
}

Knob_FSM::Knob_FSM(const Knob_FSM &other) {
	this->q_in_Sample_now = other.q_in_Sample_now;
	this->result = other.result;
	this->pinA_debounced_edge = other.pinA_debounced_edge;
	this->pinB_debounced_edge = other.pinB_debounced_edge;
}

void Knob_FSM::update(void){
	int16_t message;

	// AWAIT a message. If none, abort this call.
	if (q_in_Sample_now->dequeue(&message)){
		Edge_sense A_edge = this->pinA_debounced_edge.update();
		Edge_sense B_edge = this->pinB_debounced_edge.update();

		//debug_mailbox = (4 * (int) A_edge) + ((int) B_edge);

		// Intended: Pin A is pulled up, so when the knob turns CW, the A switch
		// opens first (thus rising), and the B opens & rises while A is still
		// high.
		if ((A_edge == EDGE_NONE_STDY_1)&&(B_edge == EDGE_RISING)){
			result->enqueue(KNOB_CW);
		}
		else if ((A_edge ==  EDGE_NONE_STDY_1) && (B_edge == EDGE_FALLING)){
			result->enqueue(KNOB_CCW);
		}
	}
	else {} // ABORT
}

