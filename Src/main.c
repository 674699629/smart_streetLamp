/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "core/AHT10.h"
#include "core/atprocess.h"
#include "core/jcFifo.h"
#include "core/atHandle.h"
#include "core/rs485Handle.h"
#include "core/bsp_645.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint32_t lock1OpenFlag = 0;
uint32_t lock2OpenFlag = 0;
Data_Frame Power;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch,FILE *f)
{
  uint8_t temp[1]={ch};
  HAL_UART_Transmit(&huart3,temp,1,2);
}
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
    uartRecvLen = strlen(uartRxBuf);

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxHalfCpltCallback can be implemented in the user file.
   */
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t recvTick = 0;
  uint8_t recvFlag = 0;
  uint8_t lock1StatusLast = 0;
  uint8_t lock2StatusLast = 0;
  uint32_t ad_buf[99] = {0};
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
  MX_ADC_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART4_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  printf("hello world.\r\n");
  // HAL_ADC_Start_DMA(&hadc, (uint32_t*) &ad_buf, 99);
  HAL_TIM_Base_Start_IT(&htim3);
  AHT_Init();
  BH1750_Init();
  // ADC?????
  uint16_t adc_value = 0;   
  uint16_t adcBuf[3] = {0};
  // ???
  float voltage = 0.0;
  // ???????
  char voltString[50] = {0};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // uartUserInit();
  atCmdSendStr(AT_CMD_ATE);
  // Power= Cmd_Bucket(69);
  while (1)
  {
    // for(int i=1;i<4;i++)
    // {
    //   HAL_ADC_Start(&hadc);
    //   HAL_ADC_PollForConversion(&hadc,100);//??ADC????
    //   adcBuf[i]=HAL_ADC_GetValue(&hadc);
    //   voltage = (float)adcBuf[i] / 4096 * 3.3;
    //   printf("------ch:%d--%d-------%.2f V\r\n",i,adcBuf[i],voltage);
    // }
    // HAL_ADC_Stop(&hadc);
    // HAL_Delay(1000);

    // Read_Ack(&Power,17);
    // rs485UartRxHandle();
    // HAL_Delay(1000);
    #if 1
    // AHT_GetData();
    // BH1750_GetData();
    uint8_t readValue = 0;
    if(jcFifoReadByte_del(&uartRxFifo,&readValue,1) > 0)
    {
      at_cmd_recv(readValue,0);
      recvTick = HAL_GetTick();
      recvFlag = 1;
    }
    // if(sysTimeCnt % 1000 == 0)
    // {
    //   rs485UartGetData(0);
		// }
    // rs485UartRxHandle();
    if ((HAL_GetTick() - recvTick) > AT_RX_TIMEOUT && recvFlag == 1) 
    {
      printf("recv timeout.\r\n");
      at_cmd_recv(readValue,1);
      recvFlag = 0;
	  }
	

		if(atRecvNorFlag == 0 && sysTimeCnt % 3000 == 0)
		{
      printf("send ate cmd.\r\n");
			atCmdSendStr(AT_CMD_ATE);
      uartUserInit();
		}
    if(sysTimeCnt % 30000 == 0)
    {
      uint8_t sendBuf[100]  = {0};
      printf("send HeartBeat cmd.\r\n");
      sprintf(sendBuf,"\{\"HeartBeat\":\"520\",\"signal\":\"%d\",\"Temperature\":\"%0.1f\"\}",signal,Temperature);
      atCmdsendTcpData(sendBuf,0x92);
		}

    uint8_t lockStatusCurrent = HAL_GPIO_ReadPin(LOCK_S1_GPIO_Port,LOCK_S1_Pin);
    if(lock1StatusLast != lockStatusCurrent)
    {
      uint8_t sendBuf[20]  = {0};
      lock1StatusLast = lockStatusCurrent;
      printf("send lock cmd.\r\n");
      if(lock1OpenFlag)
      {
        snprintf(sendBuf,20,"{\"lock1\":\"%d\"}",3);
      }
      else
      {
        snprintf(sendBuf,20,"{\"lock1\":\"%d\"}",lock1StatusLast==0?1:2);
      }
      atCmdsendTcpData(sendBuf,0x90);
    }
    lockStatusCurrent = HAL_GPIO_ReadPin(LOCK_S2_GPIO_Port,LOCK_S2_Pin);
    if(lock2StatusLast != lockStatusCurrent)
    {
      uint8_t sendBuf[20]  = {0};
      lock2StatusLast = lockStatusCurrent;
      printf("send lock cmd.\r\n");
      if(lock2OpenFlag)
      {
        snprintf(sendBuf,20,"{\"lock2\":\"%d\"}",3);
      }
      else
      {
        snprintf(sendBuf,20,"{\"lock2\":\"%d\"}",lock2StatusLast==0?1:2);
      }
      atCmdsendTcpData(sendBuf,0x90);
    }
    if(sysTimeCnt % 60000 == 0)
    {
      uint8_t sendBuf[40]  = {0};
      snprintf(sendBuf,40,"{\"lock1\":\"%d\",\"lock2\":\"%d\"}",lock1OpenFlag==1?3:(lock1StatusLast==0?1:2),lock2OpenFlag==1?3:(lock2StatusLast==0?1:2));
      atCmdsendTcpData(sendBuf,0x90);
    }
		#endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
