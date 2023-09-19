/*
 * Singledigitcounter.cpp
 *
 *  Created on: Aug 18, 2023
 *      Author: docca
 */

#include "Singledigitcounter.h"
#include <cassert>

//////////////////////////////// FOR DEBUGGING //////////////////
// extern int16_t debug_mailbox;
//////////////////////////////////////////////////////////////////


Single_digit_counter::Single_digit_counter() {
	this->the_count = 0;
	this->q_cmd = 0;  // THIS IS NOT OK ...
}

Single_digit_counter::~Single_digit_counter() {
	// TODO Auto-generated destructor stub
}

Single_digit_counter::Single_digit_counter(const Single_digit_counter &other) {
	this->q_cmd = other.q_cmd;
	this->the_count = other.the_count;
}


Single_digit_counter::Single_digit_counter(Sean_queue *q_cmd){
	this->the_count = 0;
	this->q_cmd = q_cmd;
}


void Single_digit_counter::update(void){
	int16_t msg;
	bool has_a_msg = this->q_cmd->dequeue(&msg);
	if (has_a_msg){
		if (msg == KNOB_CW){
			if (this->the_count < MAX_SHOWN){
				this->the_count++;
			}
			else{
				this->the_count= MAX_SHOWN;
			}
		}
		else if (msg == KNOB_CCW){
			if (this->the_count > MIN_SHOWN){
				this->the_count--;
			}
			else{
				this->the_count = MIN_SHOWN;
			}
		}
	}
}


uint8_t Single_digit_counter::count(void) const {
	if ((this->the_count >= 0) && (this->the_count <= MAX_SHOWN)){
		return this->the_count;
	}
	else {
		return ERROR_SHOW;
	}
}

void Single_digit_counter::debug_setter(int n){
    this->the_count = n;
}
