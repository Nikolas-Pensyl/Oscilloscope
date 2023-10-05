/*
 * Display.cpp
 *
 *  Created on: Aug 18, 2023
 *      Author: Caden Daffron
 */

#include "Display.h"

#define DOG_MAX_PAGE 7
#define DOG_MAX_SCROLL_LINE 63
#define DOG_MAX_COLUMN 131

Display::Display() {
}

Display::~Display() {
	// TODO Auto-generated destructor stub
}

Display::Display(const Display &other) {
}

Display::Display(GPIO_TypeDef port1){
}


void Display::init(SPI_HandleTypeDef hspi1) {

	uint8_t initCommands[15] = {0x40, 0xA1, 0xC0, 0xA4, 0xA6, 0xA2, 0x2F, 0x27, 0x81, 0x10, 0xFA, 0x90, 0xAF, 0x40, 0xB0};

	//Drive CD Pin Low to feed commands
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

	for(int i = 0; i<15; i++) {
		HAL_SPI_Transmit(&hspi1, (uint8_t*) &initCommands[i], 1, HAL_MAX_DELAY);
	}

	//Set CD Line high to indicate data
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_SET);
}


void Display::clearScreen(SPI_HandleTypeDef hspi1) {
	uint8_t clear = 0x00;
	uint8_t page_command = 0xB0;
	uint8_t scroll_command = 0x40;
	uint16_t column_command_1 = 0x

	// Ready the DOG to receive commands
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi1, (uint8_t*) &scroll_command, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &column_command_1, 1, HAL_MAX_DELAY);


	for (int i = 0; i <= DOG_MAX_SCROLL_LINE; i++)
	{
		for (int j = 0; j < DOG_MAX_COLUMN/8; j++) {
			HAL_SPI_Transmit(&hspi1, (uint8_t*) &clear, 1, HAL_MAX_DELAY);

		}

	}
}


void Display::update(){

}
