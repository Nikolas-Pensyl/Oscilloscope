/*
 * Display.h
 *
 *  Created on: Aug 18, 2023
 *      Author: Caden Daffron
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include "main.h"
#include "Data_Store_Object.h"
#include "Sean_queue.h"
#include <assert.h>


class Display {
private:
	SPI_HandleTypeDef *hspi;
	DataStoreObject *vert_data;
	Sean_queue *buffer_finished;
public:
	Display(SPI_HandleTypeDef *hspi, DataStoreObject *vert_data, Sean_queue *buffer_finished);
	void init();
	void clearScreen();
	void update(void);
	void drawDiag();
};

#endif /* SRC_DISPLAY_H_ */
