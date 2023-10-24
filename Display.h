/*
 * Display.h
 *
 *  Created on: Oct 22, 2023
 *      Author: cjdaf
 */

#ifndef OSCILLOSCOPE_DISPLAY_H_
#define OSCILLOSCOPE_DISPLAY_H_

#include "main.h"
#include "Sean_queue.h"
#include "Data_Store_Object.h"
#include <assert.h>

class Display {

private:
	SPI_HandleTypeDef *hspi;
	DataStoreObject *vert_data;
	Sean_queue *buffer_finished;
public:
	Display(SPI_HandleTypeDef *hspi, DataStoreObject *vert_data);
	void init();
	void clearScreen();
	void update(void);
	void drawDiag();
};

#endif /* OSCILLOSCOPE_DISPLAY_H_ */
