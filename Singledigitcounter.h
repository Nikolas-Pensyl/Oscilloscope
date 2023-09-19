/*
 * Singledigitcounter.h
 *
 *  Created on: Aug 18, 2023
 *      Author: docca
 */

#ifndef SRC_SINGLEDIGITCOUNTER_H_
#define SRC_SINGLEDIGITCOUNTER_H_

#include "KnobFSM.h" // To interpret commands from queue
#include "Sean_queue.h"

class Single_digit_counter {
private:
	Sean_queue *q_cmd;
	uint8_t the_count;
	static const int16_t MAX_SHOWN = 9;
	static const int16_t MIN_SHOWN = 0;
	static const int16_t ERROR_SHOW = 5;
public:
	Single_digit_counter();
	Single_digit_counter(Sean_queue *q_cmd);
	virtual ~Single_digit_counter();
	Single_digit_counter(const Single_digit_counter &other);

	void update(void);
	void debug_setter(int n);
	uint8_t count(void) const;   // READ-ONLY getter declare const logic.
};

#endif /* SRC_SINGLEDIGITCOUNTER_H_ */
