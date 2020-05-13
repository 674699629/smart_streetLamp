/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCU_LED_Pin GPIO_PIN_4
#define MCU_LED_GPIO_Port GPIOA
#define ADC_L1_Pin GPIO_PIN_5
#define ADC_L1_GPIO_Port GPIOA
#define ADC_L2_Pin GPIO_PIN_6
#define ADC_L2_GPIO_Port GPIOA
#define LOCK_S2_Pin GPIO_PIN_13
#define LOCK_S2_GPIO_Port GPIOB
#define LOCK_S1_Pin GPIO_PIN_14
#define LOCK_S1_GPIO_Port GPIOB
#define LOCK2_Pin GPIO_PIN_15
#define LOCK2_GPIO_Port GPIOB
#define LOCK1_Pin GPIO_PIN_8
#define LOCK1_GPIO_Port GPIOA
#define RELAY1_Pin GPIO_PIN_9
#define RELAY1_GPIO_Port GPIOA
#define RELAY2_Pin GPIO_PIN_10
#define RELAY2_GPIO_Port GPIOA
#define SCL_T_Pin GPIO_PIN_11
#define SCL_T_GPIO_Port GPIOA
#define SDA_T_Pin GPIO_PIN_12
#define SDA_T_GPIO_Port GPIOA
#define ADDR_Pin GPIO_PIN_15
#define ADDR_GPIO_Port GPIOA
#define GPRS_DTR_Pin GPIO_PIN_3
#define GPRS_DTR_GPIO_Port GPIOB
#define GPRS_PWR_Pin GPIO_PIN_4
#define GPRS_PWR_GPIO_Port GPIOB
#define RTC_CE_Pin GPIO_PIN_5
#define RTC_CE_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_8
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_9
#define SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
