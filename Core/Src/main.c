#include "bsp.h"
#include "param.h"
#include "main_menu.h"
#include "hard_test.h"


void SystemInit_clk(void);
void SystemClock_Config(void);
void IWDG_Init(void);
void Delay_ms(uint32_t ms);

IWDG_HandleTypeDef hiwdg;
HAL_StatusTypeDef status;

int main(void)
{
	uint16_t ucStatus;

	HAL_Init();

	SystemClock_Config();
	//SystemInit_clk();
	//IWDG_Init();

	bsp_Init();

	LoadParam();

	LCD_InitHard();

	TOUCH_InitHard();

	LCD_ClrScr(CL_YELLOW);
	LCD_SetBackLight(g_tParam.ucBackLight);

	ucStatus = MS_MAIN_MENU;

	uint16_t usAdcX;
	uint16_t usAdcY;

	while (1)
	{
#if 0
		//TOUCH_Poll();
		TOUCH_WaitRelease();
		usAdcX = TOUCH_ReadAdcX();
		usAdcY = TOUCH_ReadAdcY();

		char msg[50];
		sprintf(msg,"%f\n",usAdcX);
		UART1_Transmit_String(msg);
		bsp_DelayMS(500);
#endif
#if 1
		switch (ucStatus)
		{
			case MS_CALIBRATION:
			 	TOUCH_Calibration();		/* Ð£×¼´¥Ãþ, ¸Ã½çÃæÊµÏÖÔÚbsp_touch.c */
			 	ucStatus = MS_MAIN_MENU; 	/* Ð£×¼Íê±Ïºó£¬·µ»Øµ½Ö÷½çÃæ */
			break;

			case MS_MAIN_MENU:
				ucStatus = MainMenu();
			break;

			case MS_HARDWARE_INFO:
				HardInfo();
				ucStatus = MS_MAIN_MENU;
			break;

			default:
			break;
		}
#endif

	}
}

void Delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}



void SystemInit_clk(void)
{
	SystemClock_Config();

	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;

	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN;

	/* Set the VDD voltage to 3.3V (Main regulator output voltage scale 1) */
	// STM32F4의 경우 디폴트 설정이 scale1 모드임.

	/* Enable the Prefetch buffer (already disabled in the original config) */
	FLASH->ACR |= FLASH_ACR_PRFTEN;
}

void IWDG_Init(void)
{
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
	hiwdg.Init.Reload = 4095;  // 최대값 (약 26초 타임아웃)
	HAL_IWDG_Init(&hiwdg);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;                                  // 4
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

#if 0
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOI_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOH_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_SET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	  /*Configure GPIO pins : PI8 PI11 */
	  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_11;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	  /*Configure GPIO pin : PC13 */
	  GPIO_InitStruct.Pin = GPIO_PIN_13;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  /*Configure GPIO pins : PF7 PF8 */
	  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	  /*Configure GPIO pin : PC2 */
	  GPIO_InitStruct.Pin = GPIO_PIN_2;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}
#endif

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
