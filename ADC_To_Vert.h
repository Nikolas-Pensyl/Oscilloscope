/*
 * ADC_To_Vert.h
 *
 *  Created on: Oct 1, 2023
 *      Author: simon
 */

#ifndef OSCILLOSCOPE_ADC_TO_VERT_H_
#define OSCILLOSCOPE_ADC_TO_VERT_H_

#include <cstdint>
#include "Sean_queue.h"

int16_t raw_To_Vert_Queue(Sean_queue* raw_q, Sean_queue* vert_q);
bool raw_To_Vert(int16_t raw, int16_t* pixel);

#endif /* OSCILLOSCOPE_ADC_TO_VERT_H_ */
