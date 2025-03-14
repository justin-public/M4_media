/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
enum
{
	MS_MAIN_MENU = 0,	/* Ö÷½çÃæ */

	MS_HARDWARE_INFO,	/* Ó²¼þÐÅÏ¢ */
	MS_TEST_TOUCH,		/* ´¥ÃþÆÁºÍ°´¼ü */
	MS_RADIO,			/* ÊÕÒô»ú */
	MS_WM8978_REC,		/* WM8978Â¼Òô»ú */
	MS_MEMS,			/* MEMS ´«¸ÐÆ÷²âÊÔ£¨I2C£© ÈýÖáÍÓÂÝÒÇ£¬´ÅÁ¦¼Æ£¬ÆøÑ¹¼Æ */
	MS_ETH_WEB,			/* ÒÔÌ«ÍøDM9000ºÍDM9161 WEB·þÎñÆ÷ */

	MS_GPS,				/* GPS²âÊÔ */
	MS_GPRS,			/* GPRSÄ£¿é²âÊÔ */
	MS_MP3			,	/* MP3²¥·ÅÆ÷×´Ì¬ */
	MS_MASS_STORAGE,	/* SD¿¨£¬NANDÄ£ÄâUÅÌ */
	MS_AD7606_TEST,		/* AD7606Êý¾Ý²É¼¯Ä£¿é²âÊÔ */
	MS_CAMERA,			/* ÉãÏñÍ· */

	MS_OLED,			/* OLEDÏÔÊ¾Æ÷ */
	MS_NRF24L01,		/* nRF24L01+ */
	MS_NRF905,			/* nRF905(433M) */
	MS_RS485,			/* RS485Í¨ÐÅ */
	MS_CAN,				/* CANÍ¨ÐÅ */

	MS_RA8875,			/* RA8875×Ö¿â  */


	MS_CALIBRATION	,	/* Ð£×¼´¥Ãþ×´Ì¬ */
};
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

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
