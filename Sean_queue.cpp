/*
 * Seanqueue.cpp
 *
 *  Created on: Aug 12, 2023
 *      Author: carrolls@trine.edu
 */

#include "Sean_queue.h"

#include <cstdint>


Sean_queue::Sean_queue() {
	// Std c'tor = Queue head = 0, queue contains nought; pattern in
	// buffer for debug purposes only. The buffer here is oversized
	// (10) for the usage (8) to show students wrapping-before-8.
	this->head = 0;
	this->use_count = 0;
	for (int n = 0; n < QUEUE_CAPACITY; n++){
		this->buffer[n] = n+n+n;
	}
}

Sean_queue::~Sean_queue() {
	// TODO Auto-generated destructor stub -- 3 attr, all explicit:: no effort needed.
	// Note -- C++ files are auto-gen with both comment styles, which often causes
	// style-mavens to gag.
}

Sean_queue::Sean_queue(const Sean_queue &other) {
	// Copy c'tor
	this->head = other.head;
	this->use_count = other.use_count;
	for (int n = 0; n < QUEUE_CAPACITY; n++){
		this->buffer[n] = other.buffer[n];
	}
}

int16_t Sean_queue::getUseCount() {
	return this->use_count;
}

bool Sean_queue::enqueue(const int16_t msg){
	// Head + Use_count style, and calculates tail - circular buffer.
	// enqueue() returns FALSE if buffer is full.
	// Also: uses only buffer cells [0] to [7]; [8] and [9] never used
	bool successQ = false;
	if (use_count >= QUEUE_CAPACITY){
		successQ = false;
	}
	else{
	   int slot = (head + use_count)%QUEUE_CAPACITY;
	   buffer[slot] = msg;
	   use_count++;
	   successQ = true;
	}
	return successQ;
}

bool Sean_queue::dequeue(int16_t* msg){
	// Head + Use_count style circular buffer; tail is computed.
	// Returns FALSE if empty.
	// Data returned in the writable location *msg. NOTE that
	// (msg), the address for storing our value is not changed in this code.
	// LOOK - (*msg), but never (msg) appears on the left, or with a ++.
	// So, I have marked it const. The data at the other end of the ptr
	// is (*msg) and it will change, so *msg is NOT marked const. In a
	// function this short this has little value, but in longer functions
	// maybe the compiler can save code time knowing that (msg) can be relied on.
	// Also: uses only buffer cells [0] to [7]; [8] and [9] never used
	bool successQ = false;
	if (use_count <= 0){
		successQ = false;
	}
	else {
		*msg = buffer[head];
		head = (head + 1)%QUEUE_CAPACITY;
		use_count--;
		successQ = true;
	}
	return successQ;
}


S_queue_status_codes Sean_queue::status()
{
	if (use_count == QUEUE_CAPACITY) return S_QUEUE_FULL;
	else if (use_count == 0) return S_QUEUE_EMPTY;
	else return S_QUEUE_ACTIVE;
}
