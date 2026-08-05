/**
 * @file imu.h
 * @brief Public interface for the ISM330DHCX IMU driver.
 *
 * Provides functions to initialize the IMU and read acceleration,
 * angular velocity, and temperature measurements.
 */
#ifndef IMU_H
#define IMU_H

#include "main.h"
#include "ism330dhcx_reg.h"

/* Public API */

/**
 * @brief Initializes the ISM330DHCX IMU.
 *
 * Configures the sensor and prepares it for data acquisition over I2C.
 *
 * @param[in] hi2c Pointer to the HAL I2C peripheral used to communicate
 *                 with the IMU.
 *
 * @retval 0  Initialization successful.
 * @retval -1 Initialization failed.
 */

int32_t IMU_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief Reads acceleration measurements.
 *
 * Reads the latest acceleration values from the IMU.
 *
 * @param[out] ax X-axis acceleration in g.
 * @param[out] ay Y-axis acceleration in g.
 * @param[out] az Z-axis acceleration in g.
 *
 * @retval 0  Success.
 * @retval -1 Read failed.
 */
int32_t IMU_ReadAccel(float *ax,
                      float *ay,
                      float *az);

/**
 * @brief Reads angular velocity measurements.
 *
 * @param[out] gx X-axis angular velocity in degrees per second (dps).
 * @param[out] gy Y-axis angular velocity in degrees per second (dps).
 * @param[out] gz Z-axis angular velocity in degrees per second (dps).
 *
 * @retval 0  Success.
 * @retval -1 Read failed.
 */
int32_t IMU_ReadGyro(float *gx,
                     float *gy,
                     float *gz);

/**
 * @brief Reads the IMU temperature.
 *
 * @param[out] temperature Temperature in degrees Celsius.
 *
 * @retval 0  Success.
 * @retval -1 Read failed.
 */
int32_t IMU_ReadTemperature(float *temperature);

#endif /* IMU_H */
