/*
 * bsp_i2c_gpio.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "bsp.h"

// HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SDA_PIN, GPIO_PIN_SET)

#define GPIO_PORT_I2C	GPIOH
#define I2C_SCL_PIN		GPIO_PIN_4
#define I2C_SDA_PIN		GPIO_PIN_5

#define I2C_SCL_1()  HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SCL_PIN, GPIO_PIN_SET)				/* SCL = 1 */
#define I2C_SCL_0()  HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SCL_PIN, GPIO_PIN_RESET)				/* SCL = 0 */

#define I2C_SDA_1()  HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SDA_PIN, GPIO_PIN_SET)				/* SDA = 1 */
#define I2C_SDA_0()  HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SDA_PIN, GPIO_PIN_RESET)				/* SDA = 0 */


/*
*********************************************************************************************************
*	Func name: i2c_Delay
*********************************************************************************************************
*/
void i2c_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 30; i++);
}


/*
*********************************************************************************************************
*	Func name: i2c_Start
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	Func name: bsp_InitI2C
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOH_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStructure.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	HAL_GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	i2c_Stop();
}
