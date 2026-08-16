/*
 * sd_spi.h
 *
 *  Created on: Aug 15, 2026
 *      Author: tusha
 */

#ifndef INC_SD_SPI_H_
#define INC_SD_SPI_H_

#include "main.h"
#include <stdint.h>
#include <stddef.h>

#define SD_SPI_TIMEOUT_MS    100U
#define SD_DUMMY_BYTE        0xFFU

typedef enum
{
    SD_OK = 0,
    SD_ERROR,
    SD_TIMEOUT,
    SD_BUSY
} SD_Status;

void SD_CS_Low(void);
void SD_CS_High(void);

SD_Status SD_SPI_Transfer(uint8_t tx, uint8_t *rx);

#endif /* INC_SD_SPI_H_ */
