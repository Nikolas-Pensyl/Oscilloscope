/*
 * SampleClock.cpp
 *
 *  Created on: Aug 13, 2023
 *      Author: docca
 */
/* PURPOSE = Simple software timer.
 * Collect a few ticks (possibly milliseconds) from a specified
 * input queue, and when the given number have accumulated,
 * clear the count and post a tick message to the output
 * specified queue.
 */
#include "Sample_clock.h"

Sample_clock::Sample_clock(Sean_queue *inQ, Sean_queue *outQ, uint16_t period)
{
   this->inQ = inQ;
   this->outQ = outQ;
   this->period = period;
   this->countdown = period;
}

Sample_clock::~Sample_clock() {
	// TODO Auto-generated destructor stub
}

Sample_clock::Sample_clock(const Sample_clock &other) {
	// TODO Auto-generated constructor stub

}

void Sample_clock::update(void)
{
	// Countdown decrements from PERIOD to ZERO, but
	// only when there is a message in the input queue (inQ).
	// At zero, this, the (period)th message sent, will
	// be forwarded to the output queue (OutQ).
	// All other messages are noted and counted, but
	// discarded after being noticed.
	// The Sample_clock auto-resets after forwarding.
	int16_t msg;
	bool queue_ready = inQ->dequeue(&msg);
	if (queue_ready)
	{
		countdown--;
	}
	if (countdown == 0){
		outQ->enqueue(msg);
		countdown = period;
	}
}
