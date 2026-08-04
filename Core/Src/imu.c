#include "imu.h"

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

int32_t IMU_ReadAccel(float *ax, float *ay, float *az)
{
    (void)ax;
    (void)ay;
    (void)az;

    return 0;
}

int32_t IMU_ReadGyro(float *gx, float *gy, float *gz)
{
    (void)gx;
    (void)gy;
    (void)gz;

    return 0;
}


int32_t IMU_Init(void)
{
    return 0;
}

int32_t IMU_ReadTemperature(float *temperature)
{
    (void)temperature;

    return 0;
}
