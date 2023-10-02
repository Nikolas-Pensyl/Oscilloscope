/*
 * Display.h
 *
 *  Created on: Aug 18, 2023
 *      Author: docca
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include "main.h"
#include <assert.h>
#include "Singledigitcounter.h"


// To wire Segment A to PB3 and Segment B to PB2:
// port must be GPIOB
// Segment masks are in alphabetical order: [0] for A, [1] for B , ..., [6] for G.
// Example: A on PB3, assuming port = GPIOB
// segment_mask[0] = 0b00001000
// Example B on PB2
// segment_mask[1] = 0b00000100
typedef struct {
	GPIO_TypeDef *port;
	uint16_t segment_mask[7];  // Array Element [0] = single-bit-high mask for segment A
} Display_config;


class Display {
private:
	//const GPIO_TypeDef port;
public:
	Display(); //TODO:: REDO THIS CONSTRUCTOR
	Display(GPIO_TypeDef port);
	virtual ~Display();
	Display(const Display &other);

	void init(SPI_HandleTypeDef hspi);
	void update(void);
};

#endif /* SRC_DISPLAY_H_ */
