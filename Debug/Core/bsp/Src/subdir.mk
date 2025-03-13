################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/bsp/Src/LCD_RA8875.c \
../Core/bsp/Src/LCD_SPFD5420.c \
../Core/bsp/Src/bsp_i2c_gpio.c \
../Core/bsp/Src/bsp_key.c \
../Core/bsp/Src/bsp_led.c \
../Core/bsp/Src/bsp_ra8875_port.c \
../Core/bsp/Src/bsp_tft_lcd.c \
../Core/bsp/Src/bsp_timer.c \
../Core/bsp/Src/bsp_uart_fifo.c 

OBJS += \
./Core/bsp/Src/LCD_RA8875.o \
./Core/bsp/Src/LCD_SPFD5420.o \
./Core/bsp/Src/bsp_i2c_gpio.o \
./Core/bsp/Src/bsp_key.o \
./Core/bsp/Src/bsp_led.o \
./Core/bsp/Src/bsp_ra8875_port.o \
./Core/bsp/Src/bsp_tft_lcd.o \
./Core/bsp/Src/bsp_timer.o \
./Core/bsp/Src/bsp_uart_fifo.o 

C_DEPS += \
./Core/bsp/Src/LCD_RA8875.d \
./Core/bsp/Src/LCD_SPFD5420.d \
./Core/bsp/Src/bsp_i2c_gpio.d \
./Core/bsp/Src/bsp_key.d \
./Core/bsp/Src/bsp_led.d \
./Core/bsp/Src/bsp_ra8875_port.d \
./Core/bsp/Src/bsp_tft_lcd.d \
./Core/bsp/Src/bsp_timer.d \
./Core/bsp/Src/bsp_uart_fifo.d 


# Each subdirectory must supply rules for building sources it contributes
Core/bsp/Src/%.o Core/bsp/Src/%.su Core/bsp/Src/%.cyclo: ../Core/bsp/Src/%.c Core/bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/bsp -I../Core/bsp/Inc -I../Core/bsp/Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-bsp-2f-Src

clean-Core-2f-bsp-2f-Src:
	-$(RM) ./Core/bsp/Src/LCD_RA8875.cyclo ./Core/bsp/Src/LCD_RA8875.d ./Core/bsp/Src/LCD_RA8875.o ./Core/bsp/Src/LCD_RA8875.su ./Core/bsp/Src/LCD_SPFD5420.cyclo ./Core/bsp/Src/LCD_SPFD5420.d ./Core/bsp/Src/LCD_SPFD5420.o ./Core/bsp/Src/LCD_SPFD5420.su ./Core/bsp/Src/bsp_i2c_gpio.cyclo ./Core/bsp/Src/bsp_i2c_gpio.d ./Core/bsp/Src/bsp_i2c_gpio.o ./Core/bsp/Src/bsp_i2c_gpio.su ./Core/bsp/Src/bsp_key.cyclo ./Core/bsp/Src/bsp_key.d ./Core/bsp/Src/bsp_key.o ./Core/bsp/Src/bsp_key.su ./Core/bsp/Src/bsp_led.cyclo ./Core/bsp/Src/bsp_led.d ./Core/bsp/Src/bsp_led.o ./Core/bsp/Src/bsp_led.su ./Core/bsp/Src/bsp_ra8875_port.cyclo ./Core/bsp/Src/bsp_ra8875_port.d ./Core/bsp/Src/bsp_ra8875_port.o ./Core/bsp/Src/bsp_ra8875_port.su ./Core/bsp/Src/bsp_tft_lcd.cyclo ./Core/bsp/Src/bsp_tft_lcd.d ./Core/bsp/Src/bsp_tft_lcd.o ./Core/bsp/Src/bsp_tft_lcd.su ./Core/bsp/Src/bsp_timer.cyclo ./Core/bsp/Src/bsp_timer.d ./Core/bsp/Src/bsp_timer.o ./Core/bsp/Src/bsp_timer.su ./Core/bsp/Src/bsp_uart_fifo.cyclo ./Core/bsp/Src/bsp_uart_fifo.d ./Core/bsp/Src/bsp_uart_fifo.o ./Core/bsp/Src/bsp_uart_fifo.su

.PHONY: clean-Core-2f-bsp-2f-Src

