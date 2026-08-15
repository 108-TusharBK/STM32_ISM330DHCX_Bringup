/*
 * sd_spi.c
 *
 *  Created on: Aug 15, 2026
 *      Author: tushar
 */

#include "sd_spi.h"

extern SPI_HandleTypeDef hspi1;

void SD_CS_Low(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}

void SD_CS_High(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}

SD_Status SD_SPI_Transfer(uint8_t tx, uint8_t *rx)
{

}
