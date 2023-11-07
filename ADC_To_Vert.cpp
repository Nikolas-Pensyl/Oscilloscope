/*
 * ADC_To_Vert.cpp
 *
 *  Created on: Oct 1, 2023
 *      Author: simon
 */

#include "ADC_To_Vert.h"
#define MAX_ADC_RAW 4096

bool raw_To_Vert(int16_t raw, int16_t* pixel) {
	if(raw<0) {
		raw = 0;
	} else if(raw>4096) {
		raw = 4096;
	}

	int32_t temp_pixel = (int32_t)raw;

	//64 Vertical lines on Display
	temp_pixel *= 64;

	//12 bits of ADC
	temp_pixel /=4095;

	*pixel = (int16_t)temp_pixel;

	return true;
}


/*
 * Return 0 = Good
 * Return 1 = Failed Dequeue
 * Return 2 = Failed Conversion
 * Return 3 = Failed Enqueue
 */
int16_t raw_To_Vert_Queue(Sean_queue* raw_q, Sean_queue* vert_q) {
	int16_t raw, pixel = 0;

	if(!raw_q->dequeue(&raw)) { return 1; }

	if(!raw_To_Vert(raw, &pixel)) { return 2; }

	if(!vert_q->enqueue(pixel)) { return 3; }

	return 0;
}

//raw_To_Vert_Queue(&adc_raw_queue, &pixel_vertical_queue);
