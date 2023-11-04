/*
 * Display.cpp
 *
 *  Created on: Aug 18, 2023
 *      Author: Caden Daffron
 */

#include "Display.h"

#define DOG_MAX_PAGE 7
#define DOG_MAX_COLUMN 131

Display::Display(SPI_HandleTypeDef *hspi, DataStoreObject *vert_data)
{
	this->hspi = hspi;
	this->vert_data = vert_data;
	init();
}


void Display::init() {

	uint8_t initCommands[16] = {0xE2, 0x40, 0xA1, 0xC0, 0xA4, 0xA6, 0xA2, 0x2F, 0x27, 0x81, 0x10, 0xFA, 0x90, 0xAF, 0x40, 0xB0};

	//Drive CD Pin Low to feed commands
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

	for(int i = 0; i < 16; i++) {
		HAL_SPI_Transmit(hspi, (uint8_t*) &initCommands[i], 1, HAL_MAX_DELAY);
	}

	//Set CD Line high to indicate data
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_SET);
}


void Display::clearScreen() {
	uint8_t clear = 0x00;
	uint8_t page_command = 0xB0;
	uint8_t column_commands[2] = {0x00, 0x10};

	// Iterate through all pages
	for (int i = 0; i <= DOG_MAX_PAGE; i++)
	{
		// Set the CD line low, to signal incoming commands
		HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

		// Advance the current page
		HAL_SPI_Transmit(hspi, (uint8_t*) &page_command, 1, HAL_MAX_DELAY);

		// Reset to column 0 to begin writing data
		HAL_SPI_Transmit(hspi, (uint8_t*) &column_commands[0], 1, HAL_MAX_DELAY);
		HAL_SPI_Transmit(hspi, (uint8_t*) &column_commands[1], 1, HAL_MAX_DELAY);

		// Set the CD line high, to signal incoming data
		HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_SET);

		// Send clear bytes for each column until
		for (int i = 30; i <= DOG_MAX_COLUMN; i++)
		{
			// Transmit an empty data byte to clear SRAM
			HAL_SPI_Transmit(hspi, (uint8_t*) &clear, 1, HAL_MAX_DELAY);
		}

		page_command++;
	}
}


void Display::clearColumn(uint8_t column) {
	uint8_t column_commands[2] = {0x00, 0x10};
	uint8_t page_command = 0xB0;
	uint8_t clear_data = 0x00;

	// Place the relevant column address into the column select commands
	column_commands[0] |= (column & 0x0F);
	column_commands[1] |= ((column & 0xF0)>>4);

	// Set the CD line low, to signal incoming commands
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

	// Send commands to select the relevant column
	HAL_SPI_Transmit(hspi, (uint8_t*) &column_commands[0], 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hspi, (uint8_t*) &column_commands[1], 1, HAL_MAX_DELAY);

	for (int i = 0; i <= DOG_MAX_PAGE; i++)
	{
		page_command = 0xB0 | i;
		HAL_SPI_Transmit(hspi, (uint8_t*) &page_command, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_SET);
		HAL_SPI_Transmit(hspi, (uint8_t*) &clear_data, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);
	}

}


void Display::update()
{

	int16_t temp;
	if (!vert_data->display_is_reading && buffer_finished->dequeue(&temp))  {
		vert_data->display_is_reading = true;
	}

	if (!vert_data->display_is_reading)
		return;

	static uint8_t buffer_index = 0;
	uint8_t page;
	uint8_t height, heightShift;
	uint8_t page_command = 0xB0;
	uint8_t column_commands[2] = {0x00, 0x10};
	int16_t* buffer = vert_data->getReadBuffer();

	clearColumn(buffer_index);

	// Set the CD line low, to signal incoming commands
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

	// Insert relevant column address into column commands
	column_commands[0] |= (buffer_index & 0x0F);
	column_commands[1] |= ((buffer_index & 0xF0)>>4);

	// Set to relevant column to begin writing data
	HAL_SPI_Transmit(hspi, (uint8_t*) &column_commands[0], 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hspi, (uint8_t*) &column_commands[1], 1, HAL_MAX_DELAY);

	// Calculate which page the point should be on and place it into the page command
	page = buffer[buffer_index] / 8;
	page_command = 0xB0 | page;

	// Calculate the position of the point within its page
	heightShift = buffer[buffer_index] % 8;

	// Shift a bit into the correct place
	height = 1<<heightShift;

	// Go to relevant page
	HAL_SPI_Transmit(hspi, (uint8_t*) &page_command, 1, HAL_MAX_DELAY);

	// Set the CD line high, to signal incoming data
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_SET);

	// Send data
	HAL_SPI_Transmit(hspi, (uint8_t*) &height, 1, HAL_MAX_DELAY);

	// Set the CD line low, to signal incoming commands
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

	// Advance buffer_index unless it's hit the end of the buffer
	if (buffer_index < 99)
		buffer_index++;
	else {
		buffer_index = 0;
		vert_data->display_is_reading = false;
	}

}

// Draw a diagonal line from one corner of the screen to the other
void Display::drawDiag() {
	uint8_t page;
	uint8_t height, heightShift;
	uint8_t page_command = 0xB0;
	uint8_t column_commands[2] = {0x00, 0x10};

	// Set the CD line low, to signal incoming commands
	HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);

	// Reset to column 0 to begin writing data
	HAL_SPI_Transmit(hspi, (uint8_t*) &column_commands[0], 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hspi, (uint8_t*) &column_commands[1], 1, HAL_MAX_DELAY);

	for (int i = 0; i < 64; i++)
	{
		page = i / 8;
		heightShift = i % 8;
		height = 1<<heightShift;
		page_command = 0xB0 | page;

		// Go to relevant page
		HAL_SPI_Transmit(hspi, (uint8_t*) &page_command, 1, HAL_MAX_DELAY);

		// Set the CD line high, to signal incoming data
		HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_SET);

		HAL_SPI_Transmit(hspi, (uint8_t*) &height, 1, HAL_MAX_DELAY);
		if (i % 2 == 0)
			 HAL_SPI_Transmit(hspi, (uint8_t*) &height, 1, HAL_MAX_DELAY);

		HAL_GPIO_WritePin(GPIOA, DOG_CD_Pin, GPIO_PIN_RESET);
	}
}
