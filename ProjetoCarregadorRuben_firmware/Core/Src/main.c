/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "adcFile.h"
#include "pwm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FACTOR_VOLTAGE	0.00886	// It's the factor to convert AD measure in Voltage: 3.3*11/4096
#define INPUT_VOLTAGE	4
#define OUTPUT_VOLTAGE	0
#define BATTERY_VOLTAGE	1
#define MAXIMUM_PWM	400
#define LOW_VOLTAGE	5
#define HIGH_VOLTAGE 5.5
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

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */
	uint16_t pwmBoost = MAXIMUM_PWM;
	uint16_t pwmBuck = MAXIMUM_PWM;
	uint8_t estado = 0;
	float voltage12V = 0;
	float voltage3dot7V = 0;
	float outputVoltage = 0;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	ConfigAdc();
	PwmConfig();
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */

	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */
		voltage12V = ReadAdc(INPUT_VOLTAGE) * FACTOR_VOLTAGE;
		switch(estado)
		{
		case 0:

			if(voltage12V < LOW_VOLTAGE)
			{
				estado = 1;
				TIM1->CCR1 = MAXIMUM_PWM;
			}
			else if(voltage12V > HIGH_VOLTAGE)
			{
				estado = 2;
				TIM1->CCR2 = MAXIMUM_PWM;
			}
			break;

		case 1:

			voltage3dot7V = ReadAdc(BATTERY_VOLTAGE) * FACTOR_VOLTAGE - 0.6;
			if(voltage3dot7V > 3.2)
			{
				outputVoltage = ReadAdc(OUTPUT_VOLTAGE) * FACTOR_VOLTAGE;
				if((outputVoltage > 5) && pwmBoost < 400)
				{
					pwmBoost++;
				}
				else if((outputVoltage < 5) && pwmBoost > 0)
				{
					pwmBoost--;
				}
				TIM1->CCR1 = pwmBoost;
			}
			else
			{
				TIM1->CCR1 = MAXIMUM_PWM;
			}

			if(voltage12V > HIGH_VOLTAGE)
			{
				estado = 2;
				TIM1->CCR1 = MAXIMUM_PWM;
			}

			break;

		case 2:

			outputVoltage = ReadAdc(OUTPUT_VOLTAGE) * FACTOR_VOLTAGE;
			if((outputVoltage > 5) && pwmBuck > 0)
			{
				pwmBuck--;
			}
			else if((outputVoltage < 5) && pwmBuck < 400)
			{
				pwmBuck++;
			}
			TIM1->CCR2 = pwmBuck;
			if(voltage12V < LOW_VOLTAGE)
			{
				estado = 1;
				TIM1->CCR2 = MAXIMUM_PWM;
			}
			break;
		}
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

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
}

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
