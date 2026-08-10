/*
 * ssd1306.c
 *
 *  Created on: Aug 6, 2026
 *      Author: tushar
 */

#include "ssd1306.h"

#define SSD1306_CONTROL_COMMAND    0x00U
#define SSD1306_CONTROL_DATA       0x40U

static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];
static I2C_HandleTypeDef *m_hi2c = NULL;

bool SSD1306_Init(I2C_HandleTypeDef *hi2c)
{
	if (hi2c == NULL)
	{
		return false;
	}

	m_hi2c = hi2c;

	return true;
}

HAL_StatusTypeDef SSD1306_WriteCommand(uint8_t command)
{
    if (m_hi2c == NULL)
    {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Write(
        m_hi2c,
        SSD1306_I2C_ADDR,
		SSD1306_CONTROL_COMMAND,
        I2C_MEMADD_SIZE_8BIT,
        &command,
        1,
        HAL_MAX_DELAY);
}

HAL_StatusTypeDef SSD1306_WriteData(
    const uint8_t *data,
    uint16_t size)
{
	if (m_hi2c == NULL || (data == NULL) || (size == 0U))
	{
		return HAL_ERROR;
	}

	return HAL_I2C_Mem_Write(
			m_hi2c,
			SSD1306_I2C_ADDR,
			SSD1306_CONTROL_DATA,
			I2C_MEMADD_SIZE_8BIT,
			(uint8_t *)data,
			size,
			HAL_MAX_DELAY);
}

bool SSD1306_DrawPixel(
    uint8_t x,
    uint8_t y,
    SSD1306_Color_t color)
{

	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
	{
		return false;
	}

	uint8_t page = y / 8U;
	uint8_t bit = y % 8U;
	uint8_t index = page * SSD1306_WIDTH + x;

	if (color == SSD1306_COLOR_WHITE)
	{
		SSD1306_Buffer[index] |= (1U << bit);
	}
	else if(color == SSD1306_COLOR_BLACK)
	{
		SSD1306_Buffer[index] &= ~(1U << bit);
	}
	else
	{
		return false;
	}

	return true;
}

