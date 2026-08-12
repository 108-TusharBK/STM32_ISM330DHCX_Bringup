/*
 * ssd1306.h
 *
 *  Created on: Aug 6, 2026
 *      Author: tushar
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

#include "main.h"
#include <stdbool.h>

/**
 * @brief SSD1306 display resolution.
 *
 * 128 × 64 = 8192 pixels
 * 8192 bits = 1024-byte frame buffer.
 */
#define SSD1306_WIDTH      128U
#define SSD1306_HEIGHT      64U

#define SSD1306_I2C_ADDR  (0x3CU << 1)
#define SSD1306_BUFFER_SIZE \
    ((SSD1306_WIDTH * SSD1306_HEIGHT) / 8U)


typedef enum
{
    SSD1306_COLOR_BLACK = 0,
    SSD1306_COLOR_WHITE
} SSD1306_Color_t;

bool SSD1306_Init(I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef SSD1306_WriteCommand(
		uint8_t command,
		const uint8_t *parameters,
		uint8_t parameter_count);

HAL_StatusTypeDef SSD1306_WriteData(const uint8_t *data,
                                    uint16_t size);

HAL_StatusTypeDef SSD1306_UpdateScreen(void);

bool SSD1306_DrawPixel(
    uint8_t x,
    uint8_t y,
    SSD1306_Color_t color);



#endif /* INC_SSD1306_H_ */
