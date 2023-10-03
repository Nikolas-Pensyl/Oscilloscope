/*
 * Seanqueue.h
 *
 *  Created on: Aug 12, 2023
 *      Author: docca
 */

#ifndef INC_SEANQUEUE_H_
#define INC_SEANQUEUE_H_

#include <cstdint>

const uint16_t QUEUE_CAPACITY = 10;
typedef enum {S_QUEUE_EMPTY, S_QUEUE_FULL, S_QUEUE_ACTIVE} S_queue_status_codes;

class Sean_queue {
private:
	int16_t buffer[QUEUE_CAPACITY];  /* Too big - done for demo purposes */
	int16_t head;
	int16_t use_count;
public:
	Sean_queue();
	virtual ~Sean_queue();
	Sean_queue(const Sean_queue& other);

	bool enqueue(int16_t msg);
	bool dequeue(int16_t* msg);
	S_queue_status_codes status();
	int16_t getUseCount();
};

#endif /* INC_SEANQUEUE_H_ */
