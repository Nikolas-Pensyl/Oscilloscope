/*
 * RamHealth.cpp
 *
 *  Created on: Oct 10, 2023
 *      Author: Nik
 */

#include "RamHealth.h"

#define MAGIC_NUM 0xDAAB
#define STACK_LIMIT_LOC 0x2000034C

void init_mem_barrier() {
	*(uint16_t*)STACK_LIMIT_LOC = MAGIC_NUM;
}


bool getRamHealth() {
	static uint32_t barrier1 = 0xBAADF00D;
	if(barrier1 != 0xBAADF00D) { return false; }

	static uint32_t barrier2;
	if(barrier2 != 0) { return false; }

	if(*(uint16_t*)STACK_LIMIT_LOC != MAGIC_NUM) { return false; }

	return true;
}

