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
	uint8_t column_commands[2] = {0x00, 0x10};

	for (int i = 0; i <= DOG_MAX_PAGE; i++)
	{
		// Set the CD line low, to signal incoming commands
		HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

		// Advance the current page
		HAL_SPI_Transmit(&hspi1, (uint8_t*) &page_command, 1, HAL_MAX_DELAY);

		// Reset to column 0 to begin writing data
		HAL_SPI_Transmit(&hspi1, (uint8_t*) &column_command[0], 1, HAL_MAX_DELAY);
		HAL_SPI_Transmit(&hspi1, (uint8_t*) &column_command[1], 1, HAL_MAX_DELAY);

		// Set the CD line high, to signal incoming data
		HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_SET);

		// Send clear bytes for each column until
		for (int i = 0; i <= DOG_MAX_COLUMN; i++)
		{
			HAL_SPI_Transmit(&hspi1, (uint8_t*) &clear, 1, HAL_MAX_DELAY);
		}

		(&page_command)++;
	}
}


void Display::update(){

}
