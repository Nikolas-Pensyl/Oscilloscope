/*
 * DoubleDigitCounter.cpp
 *
 *  Created on: Oct 10, 2023
 *      Author: Nik
 */

#include "DoubleDigitCounter.h"
#include <cassert>

//////////////////////////////// FOR DEBUGGING //////////////////
// extern int16_t debug_mailbox;
//////////////////////////////////////////////////////////////////


DoubleDigitCounter::DoubleDigitCounter() {
	this->the_count = 1;
	this->q_cmd = 0;  // THIS IS NOT OK ...
}

DoubleDigitCounter::~DoubleDigitCounter() {
	// TODO Auto-generated destructor stub
}

DoubleDigitCounter::DoubleDigitCounter(const DoubleDigitCounter &other) {
	this->q_cmd = other.q_cmd;
	this->the_count = other.the_count;
}


DoubleDigitCounter::DoubleDigitCounter(Sean_queue *q_cmd){
	this->the_count = 1;
	this->q_cmd = q_cmd;
}


void DoubleDigitCounter::update(void){
	int16_t msg;
	bool has_a_msg = this->q_cmd->dequeue(&msg);
	if (has_a_msg){
		if (msg == KNOB_CW){
			if (this->the_count < MAX_SHOWN){
				this->the_count++;
				modified = true;
			}
			else{
				this->the_count= MAX_SHOWN;
			}
		}
		else if (msg == KNOB_CCW){
			if (this->the_count > MIN_SHOWN){
				this->the_count--;
				modified = true;
			}
			else{
				this->the_count = MIN_SHOWN;
			}
		}
	}
}


uint8_t DoubleDigitCounter::count(void) const {
	if ((this->the_count >= 0) && (this->the_count <= MAX_SHOWN)){
		modified = false;
		return this->the_count;
	}
	else {
		return ERROR_SHOW;
	}
}

bool DoubleDigitCounter::IsModified() {
	return modified;
}

void DoubleDigitCounter::debug_setter(int n){
    this->the_count = n;
}



