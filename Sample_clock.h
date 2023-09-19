/*
 * Sample_clock.h
 *
 *  Created on: Aug 13, 2023
 *      Author: docca
 */

#ifndef SRC_SAMPLE_CLOCK_H_
#define SRC_SAMPLE_CLOCK_H_
#include <cstdint>
#include "Sean_queue.h"

class Sample_clock {
private:
	Sean_queue *inQ;
	Sean_queue *outQ;
	uint16_t period;
    uint16_t countdown;
public:
    Sample_clock(Sean_queue *inQ, Sean_queue *outQ, uint16_t period);
	virtual ~Sample_clock();
	Sample_clock(const Sample_clock &other);

	void update();
};

#endif /* SRC_SAMPLE_CLOCK_H_ */
