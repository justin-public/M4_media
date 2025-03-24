################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/bsp/Src/LCD_RA8875.c \
../Core/bsp/Src/LCD_SPFD5420.c \
../Core/bsp/Src/bsp_bh1750.c \
../Core/bsp/Src/bsp_bmp085.c \
../Core/bsp/Src/bsp_eeprom_24xx.c \
../Core/bsp/Src/bsp_fsmc_sram.c \
../Core/bsp/Src/bsp_hmc5883l.c \
../Core/bsp/Src/bsp_i2c_gpio.c \
../Core/bsp/Src/bsp_key.c \
../Core/bsp/Src/bsp_led.c \
../Core/bsp/Src/bsp_mpu6050.c \
../Core/bsp/Src/bsp_nand_flash.c \
../Core/bsp/Src/bsp_nor_flash.c \
../Core/bsp/Src/bsp_ra8875_port.c \
../Core/bsp/Src/bsp_sdio_sd.c \
../Core/bsp/Src/bsp_si4730.c \
../Core/bsp/Src/bsp_spi_flash.c \
../Core/bsp/Src/bsp_tft_lcd.c \
../Core/bsp/Src/bsp_timer.c \
../Core/bsp/Src/bsp_touch.c \
../Core/bsp/Src/bsp_uart_fifo.c \
../Core/bsp/Src/bsp_wm8978.c 

OBJS += \
./Core/bsp/Src/LCD_RA8875.o \
./Core/bsp/Src/LCD_SPFD5420.o \
./Core/bsp/Src/bsp_bh1750.o \
./Core/bsp/Src/bsp_bmp085.o \
./Core/bsp/Src/bsp_eeprom_24xx.o \
./Core/bsp/Src/bsp_fsmc_sram.o \
./Core/bsp/Src/bsp_hmc5883l.o \
./Core/bsp/Src/bsp_i2c_gpio.o \
./Core/bsp/Src/bsp_key.o \
./Core/bsp/Src/bsp_led.o \
./Core/bsp/Src/bsp_mpu6050.o \
./Core/bsp/Src/bsp_nand_flash.o \
./Core/bsp/Src/bsp_nor_flash.o \
./Core/bsp/Src/bsp_ra8875_port.o \
./Core/bsp/Src/bsp_sdio_sd.o \
./Core/bsp/Src/bsp_si4730.o \
./Core/bsp/Src/bsp_spi_flash.o \
./Core/bsp/Src/bsp_tft_lcd.o \
./Core/bsp/Src/bsp_timer.o \
./Core/bsp/Src/bsp_touch.o \
./Core/bsp/Src/bsp_uart_fifo.o \
./Core/bsp/Src/bsp_wm8978.o 

C_DEPS += \
./Core/bsp/Src/LCD_RA8875.d \
./Core/bsp/Src/LCD_SPFD5420.d \
./Core/bsp/Src/bsp_bh1750.d \
./Core/bsp/Src/bsp_bmp085.d \
./Core/bsp/Src/bsp_eeprom_24xx.d \
./Core/bsp/Src/bsp_fsmc_sram.d \
./Core/bsp/Src/bsp_hmc5883l.d \
./Core/bsp/Src/bsp_i2c_gpio.d \
./Core/bsp/Src/bsp_key.d \
./Core/bsp/Src/bsp_led.d \
./Core/bsp/Src/bsp_mpu6050.d \
./Core/bsp/Src/bsp_nand_flash.d \
./Core/bsp/Src/bsp_nor_flash.d \
./Core/bsp/Src/bsp_ra8875_port.d \
./Core/bsp/Src/bsp_sdio_sd.d \
./Core/bsp/Src/bsp_si4730.d \
./Core/bsp/Src/bsp_spi_flash.d \
./Core/bsp/Src/bsp_tft_lcd.d \
./Core/bsp/Src/bsp_timer.d \
./Core/bsp/Src/bsp_touch.d \
./Core/bsp/Src/bsp_uart_fifo.d \
./Core/bsp/Src/bsp_wm8978.d 


# Each subdirectory must supply rules for building sources it contributes
Core/bsp/Src/%.o Core/bsp/Src/%.su Core/bsp/Src/%.cyclo: ../Core/bsp/Src/%.c Core/bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/FatFS -I../Core/Fonts -I../Core/Image -I../Core/bsp -I../Core/bsp/Inc -I../Core/bsp/Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-bsp-2f-Src

clean-Core-2f-bsp-2f-Src:
	-$(RM) ./Core/bsp/Src/LCD_RA8875.cyclo ./Core/bsp/Src/LCD_RA8875.d ./Core/bsp/Src/LCD_RA8875.o ./Core/bsp/Src/LCD_RA8875.su ./Core/bsp/Src/LCD_SPFD5420.cyclo ./Core/bsp/Src/LCD_SPFD5420.d ./Core/bsp/Src/LCD_SPFD5420.o ./Core/bsp/Src/LCD_SPFD5420.su ./Core/bsp/Src/bsp_bh1750.cyclo ./Core/bsp/Src/bsp_bh1750.d ./Core/bsp/Src/bsp_bh1750.o ./Core/bsp/Src/bsp_bh1750.su ./Core/bsp/Src/bsp_bmp085.cyclo ./Core/bsp/Src/bsp_bmp085.d ./Core/bsp/Src/bsp_bmp085.o ./Core/bsp/Src/bsp_bmp085.su ./Core/bsp/Src/bsp_eeprom_24xx.cyclo ./Core/bsp/Src/bsp_eeprom_24xx.d ./Core/bsp/Src/bsp_eeprom_24xx.o ./Core/bsp/Src/bsp_eeprom_24xx.su ./Core/bsp/Src/bsp_fsmc_sram.cyclo ./Core/bsp/Src/bsp_fsmc_sram.d ./Core/bsp/Src/bsp_fsmc_sram.o ./Core/bsp/Src/bsp_fsmc_sram.su ./Core/bsp/Src/bsp_hmc5883l.cyclo ./Core/bsp/Src/bsp_hmc5883l.d ./Core/bsp/Src/bsp_hmc5883l.o ./Core/bsp/Src/bsp_hmc5883l.su ./Core/bsp/Src/bsp_i2c_gpio.cyclo ./Core/bsp/Src/bsp_i2c_gpio.d ./Core/bsp/Src/bsp_i2c_gpio.o ./Core/bsp/Src/bsp_i2c_gpio.su ./Core/bsp/Src/bsp_key.cyclo ./Core/bsp/Src/bsp_key.d ./Core/bsp/Src/bsp_key.o ./Core/bsp/Src/bsp_key.su ./Core/bsp/Src/bsp_led.cyclo ./Core/bsp/Src/bsp_led.d ./Core/bsp/Src/bsp_led.o ./Core/bsp/Src/bsp_led.su ./Core/bsp/Src/bsp_mpu6050.cyclo ./Core/bsp/Src/bsp_mpu6050.d ./Core/bsp/Src/bsp_mpu6050.o ./Core/bsp/Src/bsp_mpu6050.su ./Core/bsp/Src/bsp_nand_flash.cyclo ./Core/bsp/Src/bsp_nand_flash.d ./Core/bsp/Src/bsp_nand_flash.o ./Core/bsp/Src/bsp_nand_flash.su ./Core/bsp/Src/bsp_nor_flash.cyclo ./Core/bsp/Src/bsp_nor_flash.d ./Core/bsp/Src/bsp_nor_flash.o ./Core/bsp/Src/bsp_nor_flash.su ./Core/bsp/Src/bsp_ra8875_port.cyclo ./Core/bsp/Src/bsp_ra8875_port.d ./Core/bsp/Src/bsp_ra8875_port.o ./Core/bsp/Src/bsp_ra8875_port.su ./Core/bsp/Src/bsp_sdio_sd.cyclo ./Core/bsp/Src/bsp_sdio_sd.d ./Core/bsp/Src/bsp_sdio_sd.o ./Core/bsp/Src/bsp_sdio_sd.su ./Core/bsp/Src/bsp_si4730.cyclo ./Core/bsp/Src/bsp_si4730.d ./Core/bsp/Src/bsp_si4730.o ./Core/bsp/Src/bsp_si4730.su ./Core/bsp/Src/bsp_spi_flash.cyclo ./Core/bsp/Src/bsp_spi_flash.d ./Core/bsp/Src/bsp_spi_flash.o ./Core/bsp/Src/bsp_spi_flash.su ./Core/bsp/Src/bsp_tft_lcd.cyclo ./Core/bsp/Src/bsp_tft_lcd.d ./Core/bsp/Src/bsp_tft_lcd.o ./Core/bsp/Src/bsp_tft_lcd.su ./Core/bsp/Src/bsp_timer.cyclo ./Core/bsp/Src/bsp_timer.d ./Core/bsp/Src/bsp_timer.o ./Core/bsp/Src/bsp_timer.su ./Core/bsp/Src/bsp_touch.cyclo ./Core/bsp/Src/bsp_touch.d ./Core/bsp/Src/bsp_touch.o ./Core/bsp/Src/bsp_touch.su ./Core/bsp/Src/bsp_uart_fifo.cyclo ./Core/bsp/Src/bsp_uart_fifo.d ./Core/bsp/Src/bsp_uart_fifo.o ./Core/bsp/Src/bsp_uart_fifo.su ./Core/bsp/Src/bsp_wm8978.cyclo ./Core/bsp/Src/bsp_wm8978.d ./Core/bsp/Src/bsp_wm8978.o ./Core/bsp/Src/bsp_wm8978.su

.PHONY: clean-Core-2f-bsp-2f-Src

