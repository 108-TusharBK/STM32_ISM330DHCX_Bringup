/*
 * ssd1306.c
 *
 *  Created on: Aug 6, 2026
 *      Author: tushar
 */

#include "ssd1306.h"

#define SSD1306_CONTROL_COMMAND    0x00
#define SSD1306_CONTROL_DATA       0x40

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
			&buffer[0],
			1,
			HAL_MAX_DELAY);
}
