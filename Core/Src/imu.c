#include "imu.h"

/* Private Variables */
static stmdev_ctx_t dev_ctx;
static I2C_HandleTypeDef *imu_i2c;
#define IMU_ACCEL_SENSITIVITY_2G_MG_PER_LSB   (0.061f)

static int32_t platform_write(void *handle,
                              uint8_t reg,
                              const uint8_t *bufp,
                              uint16_t len);

static int32_t platform_read(void *handle,
                             uint8_t reg,
                             uint8_t *bufp,
                             uint16_t len);

#define ISM330DHCX_I2C_ADDR (0x6B << 1)

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

int32_t IMU_ReadAccel(float *ax,
                      float *ay,
                      float *az)
{
    int16_t raw_acc[3];

    if (ism330dhcx_acceleration_raw_get(&dev_ctx, raw_acc) != 0)
    {
        return -1;
    }

    *ax = raw_acc[0] * IMU_ACCEL_SENSITIVITY_2G_MG_PER_LSB / 1000.0f;
    *ay = raw_acc[1] * IMU_ACCEL_SENSITIVITY_2G_MG_PER_LSB / 1000.0f;
    *az = raw_acc[2] * IMU_ACCEL_SENSITIVITY_2G_MG_PER_LSB / 1000.0f;

    return 0;
}

int32_t IMU_ReadGyro(float *gx, float *gy, float *gz)
{
    (void)gx;
    (void)gy;
    (void)gz;

    return 0;
}

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

	return 0;
}


int32_t IMU_ReadTemperature(float *temperature)
{
    (void)temperature;

    return 0;
}
