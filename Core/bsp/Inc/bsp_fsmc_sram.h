/*
 * bsp_fsmc_sram.h
 *
 *  Created on: Mar 17, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_FSMC_SRAM_H_
#define BSP_INC_BSP_FSMC_SRAM_H_

#define EXT_SRAM_ADDR  	((uint32_t)0x68000000)
#define EXT_SRAM_SIZE	(2 * 1024 * 1024)

void bsp_InitExtSRAM(void);
uint8_t bsp_TestExtSRAM(void);

#endif /* BSP_INC_BSP_FSMC_SRAM_H_ */
