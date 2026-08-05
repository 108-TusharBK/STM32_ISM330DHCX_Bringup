/**
 * @file imu.c
 * @brief Implementation of the IMU driver for the ISM330DHCX sensor.
 *
 * This module provides functions to initialize the IMU and retrieve
 * acceleration, angular velocity, and temperature measurements over I2C.
 */

#include "imu.h"

/* Private macros */
/**
 * @brief Accelerometer sensitivity for ±2 g configuration.
 *
 * Units: mg/LSB
 */
#define IMU_ACCEL_SENSITIVITY_2G_MG_PER_LSB    (0.061f)

/**
 * @brief Gyroscope sensitivity for ±250 dps configuration.
 *
 * Units: mdps/LSB
 */
#define IMU_GYRO_SENSITIVITY_250_MDPS_PER_LSB  (8.75f)

#define MG_TO_G     (1000.0f)
#define MDPS_TO_DPS (1000.0f)
/**
 * @brief 8-bit I2C address of the ISM330DHCX.
 *
 * The STM32 HAL expects the 7-bit device address left-shifted by one bit.
 */
#define ISM330DHCX_I2C_ADDR (0x6B << 1)


/* Private Variables */
/**
 * @brief ST driver context.
 */
static stmdev_ctx_t dev_ctx;

/**
 * @brief Pointer to the active I2C peripheral.
 */
static I2C_HandleTypeDef *imu_i2c;


static int32_t platform_write(void *handle,
                              uint8_t reg,
                              const uint8_t *bufp,
                              uint16_t len);

static int32_t platform_read(void *handle,
                             uint8_t reg,
                             uint8_t *bufp,
                             uint16_t len);



/**
 * @brief Writes registers over I2C.
 *
 * Callback used by the ST sensor driver.
 *
 * @param[in] handle HAL I2C handle.
 * @param[in] reg Register address.
 * @param[in] bufp Data buffer.
 * @param[in] len Number of bytes.
 *
 * @retval 0 Success
 * @retval -1 Communication failure
 */
static int32_t platform_write(void *handle,
                              uint8_t reg,
                              const uint8_t *bufp,
                              uint16_t len)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Write(
    		(I2C_HandleTypeDef *)handle,
                    ISM330DHCX_I2C_ADDR,
                    reg,
                    I2C_MEMADD_SIZE_8BIT,
                    (uint8_t *)bufp,
                    len,
                    HAL_MAX_DELAY);

    return (status == HAL_OK) ? 0 : -1;
}

/**
 * @brief Reads registers over I2C.
 *
 * Callback used by the ST sensor driver.
 *
 * @param[in] handle HAL I2C handle.
 * @param[in] reg Register address.
 * @param[in] bufp Data buffer.
 * @param[in] len Number of bytes.
 *
 * @retval 0 Success.
 * @retval -1 Communication failure.
 */
static int32_t platform_read(void *handle,
                             uint8_t reg,
                             uint8_t *bufp,
                             uint16_t len)
{
    if (HAL_I2C_Mem_Read((I2C_HandleTypeDef *)handle,
                         ISM330DHCX_I2C_ADDR,
                         reg,
                         I2C_MEMADD_SIZE_8BIT,
                         bufp,
                         len,
                         1000) != HAL_OK)
    {
        return -1;
    }

    return 0;
}

/**
 * @brief Reads acceleration data from the IMU.
 *
 * Reads the latest acceleration measurement from the ISM330DHCX sensor
 * and converts it to units of g.
 *
 * @param[out] ax Pointer to X-axis acceleration in g.
 * @param[out] ay Pointer to Y-axis acceleration in g.
 * @param[out] az Pointer to Z-axis acceleration in g.
 *
 * @retval 0  Success
 * @retval -1 Communication error
 */

int32_t IMU_ReadAccel(float *ax,
                      float *ay,
                      float *az)
{
    int16_t raw_acc[3];

    if (ism330dhcx_acceleration_raw_get(&dev_ctx, raw_acc) != 0)
    {
        return -1;
    }

    *ax = raw_acc[0] * IMU_ACCEL_SENSITIVITY_2G_MG_PER_LSB / MG_TO_G;
    *ay = raw_acc[1] * IMU_ACCEL_SENSITIVITY_2G_MG_PER_LSB / MG_TO_G;
    *az = raw_acc[2] * IMU_ACCEL_SENSITIVITY_2G_MG_PER_LSB / MG_TO_G;

    return 0;
}

/**
 * @brief Reads angular velocity measurements from the IMU.
 *
 * Reads the latest gyroscope measurement from the ISM330DHCX sensor
 * and converts it to units of dps.
 *
 * @param[out] gx X-axis angular velocity in degrees per second (dps).
 * @param[out] gy Y-axis angular velocity in degrees per second (dps).
 * @param[out] gz Z-axis angular velocity in degrees per second (dps).
 *
 * @retval 0  Success
 * @retval -1 Communication error
 */
int32_t IMU_ReadGyro(float *gx,
                     float *gy,
                     float *gz)
{
    int16_t raw_gyro[3];

    if (ism330dhcx_angular_rate_raw_get(&dev_ctx, raw_gyro) != 0)
    {
        return -1;
    }

    *gx = raw_gyro[0] * IMU_GYRO_SENSITIVITY_250_MDPS_PER_LSB / MDPS_TO_DPS;
    *gy = raw_gyro[1] * IMU_GYRO_SENSITIVITY_250_MDPS_PER_LSB / MDPS_TO_DPS;
    *gz = raw_gyro[2] * IMU_GYRO_SENSITIVITY_250_MDPS_PER_LSB / MDPS_TO_DPS;

    return 0;
}

/**
 * @brief Initializes the IMU sensor.
 *
 * Configures the ISM330DHCX accelerometer and gyroscope with:
 * - Accelerometer: ±2 g, 104 Hz
 * - Gyroscope: ±250 dps, 104 Hz
 * - Block Data Update enabled
 *
 * @param[in] hi2c Pointer to the HAL I2C peripheral.
 *
 * @retval 0  Initialization successful.
 * @retval -1 Configuration failed.
 */

int32_t IMU_Init(I2C_HandleTypeDef *hi2c){
	imu_i2c = hi2c;

	dev_ctx.write_reg = platform_write;
	dev_ctx.read_reg  = platform_read;
	dev_ctx.handle = (void *)imu_i2c;

	if (ism330dhcx_block_data_update_set(&dev_ctx, PROPERTY_ENABLE) != 0)
	{
      return -1;
	}

	if (ism330dhcx_xl_data_rate_set(&dev_ctx, ISM330DHCX_XL_ODR_104Hz) != 0)
	{
		return -1;
	}

	if (ism330dhcx_xl_full_scale_set(&dev_ctx, ISM330DHCX_2g) != 0)
	{
		return -1;
	}

	if (ism330dhcx_gy_data_rate_set(&dev_ctx,
	                                ISM330DHCX_GY_ODR_104Hz) != 0)
	{
	    return -1;
	}

	if (ism330dhcx_gy_full_scale_set(&dev_ctx,
	                                 ISM330DHCX_250dps) != 0)
	{
	    return -1;
	}

	return 0;
}

/**
 * @brief Reads temperature data from the IMU.
 *
 * Reads the latest temperature measurement from the ISM330DHCX sensor
 * and converts it to degree Celsius.
 *
 * @param[out] temperature Pointer to the measured temperature in degrees Celsius (°C).
 *
 * @retval 0  Success
 * @retval -1 Communication error
 */
int32_t IMU_ReadTemperature(float *temperature)
{
    int16_t temp;

    if (ism330dhcx_temperature_raw_get(&dev_ctx, &temp) != 0)
    {
    	return -1;
    }

    *temperature = ism330dhcx_from_lsb_to_celsius(temp);


    return 0;
}
