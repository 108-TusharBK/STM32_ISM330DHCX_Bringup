/*
 * ssd1306.c
 *
 *  Created on: Aug 6, 2026
 *      Author: tushar
 */

#include "ssd1306.h"
#include <string.h>

#define SSD1306_CONTROL_COMMAND    0x00U
#define SSD1306_CONTROL_DATA       0x40U

static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];
static I2C_HandleTypeDef *m_hi2c = NULL;

static HAL_StatusTypeDef SSD1306_WriteCommandWithParam(
    uint8_t command,
    uint8_t parameter){

	 uint8_t param = parameter;

	 return SSD1306_WriteCommand(command, &param, 1U);

}

bool SSD1306_Init(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL)
    {
        return false;
    }

    // Temporarily use the supplied handle
    m_hi2c = hi2c;

    if (SSD1306_WriteCommand(0xAEU, NULL, 0U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }

    // Perform initialization
    if (SSD1306_WriteCommandWithParam(0x8DU, 0x14U) != HAL_OK)
    {
        m_hi2c = NULL;      // rollback driver state
        return false;
    }

    if (SSD1306_WriteCommandWithParam(0x20U, 0x02U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }

    if (SSD1306_WriteCommandWithParam(0xA8U, 0x3FU) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommandWithParam(0xD3U, 0x00U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommand(0x40U, NULL, 0U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommand(0xA0U, NULL, 0U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommand(0xC8U, NULL, 0U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommandWithParam(0x81U, 0x7FU) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommand(0xA4U, NULL, 0U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommand(0xA6U, NULL, 0U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommandWithParam(0xD5U, 0x80U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }

    if (SSD1306_WriteCommandWithParam(0xD9U, 0xF1U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }
    if (SSD1306_WriteCommandWithParam(0xDBU, 0x40U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }

    if (SSD1306_WriteCommandWithParam(0xDAU, 0x12U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }

    memset(SSD1306_Buffer, 0x00, sizeof(SSD1306_Buffer));

    if (SSD1306_UpdateScreen() != HAL_OK)
    {
        m_hi2c = NULL;
        return false;
    }

    if (SSD1306_WriteCommand(0xAFU, NULL, 0U) != HAL_OK)
    {
    	m_hi2c = NULL;
    	return false;
    }

    return true;
}


HAL_StatusTypeDef SSD1306_WriteCommand(
    uint8_t command,
    const uint8_t *parameters,
    uint8_t parameter_count)
{
    if (m_hi2c == NULL)
    {
        return HAL_ERROR;
    }

    if ((parameter_count > 0U) && (parameters == NULL))
    {
        return HAL_ERROR;
    }

    uint8_t command_buffer[1U + UINT8_MAX];

    command_buffer[0] = command;

    if (parameter_count > 0U)
    {
        memcpy(&command_buffer[1], parameters, parameter_count);
    }

    return HAL_I2C_Mem_Write(
        m_hi2c,
        SSD1306_I2C_ADDR,
        SSD1306_CONTROL_COMMAND,
        I2C_MEMADD_SIZE_8BIT,
        command_buffer,
        1U + parameter_count,
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
	uint16_t index = page * SSD1306_WIDTH + x;

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

HAL_StatusTypeDef SSD1306_UpdateScreen(void)
{
	uint8_t page;
	for (page = 0U; page < 8U; page ++)
	{

		if (SSD1306_WriteCommand(0xB0U + page, NULL, 0U) != HAL_OK)
		{
			return HAL_ERROR;
		}
		if (SSD1306_WriteCommand(0x00U, NULL, 0U) != HAL_OK)
		{
			return HAL_ERROR;
		}
		if (SSD1306_WriteCommand(0x10U, NULL, 0U) != HAL_OK)
		{
			return HAL_ERROR;
		}

		HAL_StatusTypeDef status = SSD1306_WriteData(
	        &SSD1306_Buffer[page * SSD1306_WIDTH],
	        SSD1306_WIDTH);

		if (status != HAL_OK)
		{
			return status;
		}
	}

	return HAL_OK;
}

