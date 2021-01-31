#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>

// Create to https://shawnhymel.com/1873/how-to-use-printf-on-stm32/
#include "retarget.h"

void SystemClock_Config(void);

#define BUFFER_LIMIT 3
uint16_t adc_buff[BUFFER_LIMIT];

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();

  if(HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
  {
	  Error_Handler();
  }

  RetargetInit(&huart2);
  printf("\r\nBegin Log Session ------- \r\n\r\n");

  if(HAL_ADC_Start_DMA(&hadc, (uint32_t *)adc_buff, BUFFER_LIMIT) != HAL_OK)
  {
	  Error_Handler();
  }


  while (1)
  {
	  printf("\r\n------ ADC Cycle -------");
	  printf("\r\nADC0 = %d\r\nADC1 = %d\r\nADC4 = %d\r\n", adc_buff[0], adc_buff[1], adc_buff[2]);
	  HAL_Delay(1000);
  }

}

// DMA ADC Callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{

}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef * hadc)
{

}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef * hadc)
{
	printf("\r\n ------ ADC ERROR ------ \r\n");
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
  printf("\r\n------ ERROR HAS OCCURED ------\r\n");
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
