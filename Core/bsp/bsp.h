/*
 * bsp.h
 *
 *  Created on: Mar 12, 2025
 *      Author: compro
 */

#ifndef BSP_BSP_H_
#define BSP_BSP_H_

#define STM32_V5
//#define STM32_X3

/* ¼ì²éÊÇ·ñ¶¨ÒåÁË¿ª·¢°åÐÍºÅ */
#if !defined (STM32_V5) && !defined (STM32_X3)
	#error "Please define the board model : STM32_X3 or STM32_V5"
#endif

/* ¶¨Òå BSP °æ±¾ºÅ */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU¿ÕÏÐÊ±Ö´ÐÐµÄº¯Êý */
//#define CPU_IDLE()		bsp_Idle()

/* ¿ª¹ØÈ«¾ÖÖÐ¶ÏµÄºê */
#define ENABLE_INT()	__set_PRIMASK(0)	/* Ê¹ÄÜÈ«¾ÖÖÐ¶Ï */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ½ûÖ¹È«¾ÖÖÐ¶Ï */

/* Õâ¸öºê½öÓÃÓÚµ÷ÊÔ½×¶ÎÅÅ´í */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "main.h"
#include <stdio.h>
#include <string.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define DEBUG_GPS_TO_COM1	/* ´òÓ¡GPSÊý¾Ýµ½´®¿Ú1 */

/* Í¨¹ýÈ¡Ïû×¢ÊÍ»òÕßÌí¼Ó×¢ÊÍµÄ·½Ê½¿ØÖÆÊÇ·ñ°üº¬µ×²ãÇý¶¯Ä£¿é */
#include "bsp_uart_fifo.h"
#include "bsp_led.h"
#include "bsp_timer.h"
#include "bsp_key.h"

//#include "bsp_spi_flash.h"
//#include "bsp_cpu_flash.h"
#include "bsp_sdio_sd.h"
#include "bsp_i2c_gpio.h"
#include "bsp_eeprom_24xx.h"
//#include "bsp_si4730.h"
//#include "bsp_hmc5883l.h"
//#include "bsp_mpu6050.h"
//#include "bsp_bh1750.h"
//#include "bsp_bmp085.h"
//#include "bsp_wm8978.h"

#include "bsp_fsmc_sram.h"
#include "bsp_nand_flash.h"
//#include "bsp_nor_flash.h"

#include "LCD_RA8875.h"
#include "LCD_SPFD5420.h"
#include "bsp_ra8875_port.h"
#include "bsp_tft_lcd.h"

#include "bsp_touch.h"
//#include "bsp_camera.h"

//#include "bsp_ad7606.h"
//#include "bsp_gps.h"
//#include "bsp_oled.h"
//#include "bsp_mg323.h"

//#include "bsp_ra8875_flash.h"
//#include "bsp_vs1053b.h"


/* Ìá¹©¸øÆäËûCÎÄ¼þµ÷ÓÃµÄº¯Êý */
void bsp_Init(void);
void bsp_Idle(void);

#endif /* BSP_BSP_H_ */
