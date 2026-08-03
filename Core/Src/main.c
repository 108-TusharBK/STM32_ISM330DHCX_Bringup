/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ism330dhcx_reg.h"
#include <string.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
stmdev_ctx_t dev_ctx;

uint8_t whoamI;

char uart_buf[64];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
static int32_t platform_write(void *handle,
                              uint8_t reg,
                              const uint8_t *bufp,
                              uint16_t len);

static int32_t platform_read(void *handle,
                             uint8_t reg,
                             uint8_t *bufp,
                             uint16_t len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* uint8_t regs[] = {0x0F, 0x10, 0x11, 0x12, 0x13};
  uint8_t value;

  for (int i = 0; i < 5; i++)
  {
      HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
          &hi2c1,
          (0x6B << 1),
          regs[i],
          I2C_MEMADD_SIZE_8BIT,
          &value,
          1,
          1000);

      sprintf(uart_buf,
              "Reg 0x%02X  Status=%d  Value=0x%02X  Err=0x%08lX\r\n",
              regs[i],
              status,
              value,
              HAL_I2C_GetError(&hi2c1));

      HAL_UART_Transmit(&huart2,
                        (uint8_t *)uart_buf,
                        strlen(uart_buf),
                        HAL_MAX_DELAY);
  } */

  stmdev_ctx_t dev_ctx;

  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg  = platform_read;
  dev_ctx.handle    = &hi2c1;

  int32_t ret;

  ret = ism330dhcx_block_data_update_set(
          &dev_ctx,
          PROPERTY_ENABLE);

  sprintf(uart_buf,
          "BDU ret = %ld\r\n",
          ret);

  HAL_UART_Transmit(&huart2,
                    (uint8_t*)uart_buf,
                    strlen(uart_buf),
                    HAL_MAX_DELAY);

  ret = ism330dhcx_xl_data_rate_set(
          &dev_ctx,
          ISM330DHCX_XL_ODR_104Hz);

  sprintf(uart_buf,
          "XL ODR ret = %ld\r\n",
          ret);

  HAL_UART_Transmit(&huart2,
                    (uint8_t*)uart_buf,
                    strlen(uart_buf),
                    HAL_MAX_DELAY);

  ret = ism330dhcx_xl_full_scale_set(
          &dev_ctx,
          ISM330DHCX_2g);

  sprintf(uart_buf,
          "XL FS ret = %ld\r\n",
          ret);

  HAL_UART_Transmit(&huart2,
                    (uint8_t*)uart_buf,
                    strlen(uart_buf),
                    HAL_MAX_DELAY);


  int16_t acc[3];

  ret = ism330dhcx_acceleration_raw_get(
          &dev_ctx,
          acc);

  sprintf(uart_buf,
          "ACC Read ret = %ld\r\n",
          ret);

  HAL_UART_Transmit(&huart2,
                    (uint8_t *)uart_buf,
                    strlen(uart_buf),
                    HAL_MAX_DELAY);

  acc[0] = acc[0];
  acc[1] = acc[1];
  acc[2] = acc[2];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      ret = ism330dhcx_acceleration_raw_get(&dev_ctx, acc);

      sprintf(uart_buf,
              "X=%6d  Y=%6d  Z=%6d\r\n",
              acc[0],
              acc[1],
              acc[2]);

      float ax_g = acc[0] * 0.061f / 1000.0f;
      float ay_g = acc[1] * 0.061f / 1000.0f;
      float az_g = acc[2] * 0.061f / 1000.0f;

      sprintf(uart_buf,
              "X=%7.3f g  Y=%7.3f g  Z=%7.3f g\r\n",
              ax_g,
              ay_g,
              az_g);

      HAL_UART_Transmit(&huart2,
                        (uint8_t*)uart_buf,
                        strlen(uart_buf),
                        HAL_MAX_DELAY);

      HAL_UART_Transmit(&huart2,
                        (uint8_t*)uart_buf,
                        strlen(uart_buf),
                        HAL_MAX_DELAY);

      HAL_Delay(200);
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

#define ISM330DHCX_I2C_ADDR (0x6B << 1)

static int32_t platform_write(void *handle,
                              uint8_t reg,
                              const uint8_t *bufp,
                              uint16_t len)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Write(
                    &hi2c1,
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
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
