/*
 * Display.h
 *
 *  Created on: Aug 18, 2023
 *      Author: Caden Daffron
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include "main.h"
#include <assert.h>


class Display {
private:
public:
	void init(SPI_HandleTypeDef hspi);
	void clearScreen(SPI_HandleTypeDef hspi);
	void update(void);
	void drawDiag(SPI_HandleTypeDef hspi);
};

#endif /* SRC_DISPLAY_H_ */
