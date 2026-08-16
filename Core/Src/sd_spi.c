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
    HAL_StatusTypeDef hal_status;

    if (rx == NULL)
    {
        return SD_ERROR;
    }

    hal_status = HAL_SPI_TransmitReceive(&hspi1,
                                         &tx,
                                         rx,
                                         1U,
                                         SD_SPI_TIMEOUT_MS);

    switch (hal_status)
    {
        case HAL_OK:
            return SD_OK;

        case HAL_BUSY:
            return SD_BUSY;

        case HAL_TIMEOUT:
            return SD_TIMEOUT;

        default:
            return SD_ERROR;
    }
}
