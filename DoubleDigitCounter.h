/*
 * DoubleDigitCounter.h
 *
 *  Created on: Oct 10, 2023
 *      Author: Nik
 */

#ifndef REPO_CODE_DOUBLEDIGITCOUNTER_H_
#define REPO_CODE_DOUBLEDIGITCOUNTER_H_


#include "KnobFSM.h" // To interpret commands from queue
#include "Sean_queue.h"

class DoubleDigitCounter {
private:
	Sean_queue *q_cmd;
	uint8_t the_count;
	bool modified;
	static const int16_t MAX_SHOWN = 99;
	static const int16_t MIN_SHOWN = 0;
	static const int16_t ERROR_SHOW = 5;
public:
	DoubleDigitCounter();
	DoubleDigitCounter(Sean_queue *q_cmd);
	virtual ~DoubleDigitCounter();
	DoubleDigitCounter(const DoubleDigitCounter &other);

	void update(void);
	void debug_setter(int n);
	uint8_t count(void) const;   // READ-ONLY getter declare const logic.
	bool IsModified();
};


#endif /* REPO_CODE_DOUBLEDIGITCOUNTER_H_ */
