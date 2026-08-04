
#ifndef IMU_H
#define IMU_H

#include "main.h"
#include "ism330dhcx_reg.h"

/* Public API */

/**
 * @brief Initialize the IMU.
 * @return 0 on success, non-zero on failure.
 */

int32_t IMU_Init(I2C_HandleTypeDef *hi2c);

int32_t IMU_ReadAccel(float *ax,
                      float *ay,
                      float *az);

int32_t IMU_ReadGyro(float *gx,
                     float *gy,
                     float *gz);

int32_t IMU_ReadTemperature(float *temperature);

#endif /* IMU_H */
