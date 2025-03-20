################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bmp_file.c \
../Core/Src/hard_test.c \
../Core/Src/main.c \
../Core/Src/main_menu.c \
../Core/Src/param.c \
../Core/Src/stm32f4xx_hal_iwdg.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_spi.c \
../Core/Src/stm32f4xx_hal_sram.c \
../Core/Src/stm32f4xx_hal_uart.c \
../Core/Src/stm32f4xx_hal_usart.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/stm32f4xx_ll_fmc.c \
../Core/Src/stm32f4xx_ll_fsmc.c \
../Core/Src/stm32f4xx_ll_spi.c \
../Core/Src/stm32f4xx_ll_usart.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/bmp_file.o \
./Core/Src/hard_test.o \
./Core/Src/main.o \
./Core/Src/main_menu.o \
./Core/Src/param.o \
./Core/Src/stm32f4xx_hal_iwdg.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_spi.o \
./Core/Src/stm32f4xx_hal_sram.o \
./Core/Src/stm32f4xx_hal_uart.o \
./Core/Src/stm32f4xx_hal_usart.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/stm32f4xx_ll_fmc.o \
./Core/Src/stm32f4xx_ll_fsmc.o \
./Core/Src/stm32f4xx_ll_spi.o \
./Core/Src/stm32f4xx_ll_usart.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/bmp_file.d \
./Core/Src/hard_test.d \
./Core/Src/main.d \
./Core/Src/main_menu.d \
./Core/Src/param.d \
./Core/Src/stm32f4xx_hal_iwdg.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_spi.d \
./Core/Src/stm32f4xx_hal_sram.d \
./Core/Src/stm32f4xx_hal_uart.d \
./Core/Src/stm32f4xx_hal_usart.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/stm32f4xx_ll_fmc.d \
./Core/Src/stm32f4xx_ll_fsmc.d \
./Core/Src/stm32f4xx_ll_spi.d \
./Core/Src/stm32f4xx_ll_usart.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/FatFS -I../Core/Fonts -I../Core/Image -I../Core/bsp -I../Core/bsp/Inc -I../Core/bsp/Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/bmp_file.cyclo ./Core/Src/bmp_file.d ./Core/Src/bmp_file.o ./Core/Src/bmp_file.su ./Core/Src/hard_test.cyclo ./Core/Src/hard_test.d ./Core/Src/hard_test.o ./Core/Src/hard_test.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/main_menu.cyclo ./Core/Src/main_menu.d ./Core/Src/main_menu.o ./Core/Src/main_menu.su ./Core/Src/param.cyclo ./Core/Src/param.d ./Core/Src/param.o ./Core/Src/param.su ./Core/Src/stm32f4xx_hal_iwdg.cyclo ./Core/Src/stm32f4xx_hal_iwdg.d ./Core/Src/stm32f4xx_hal_iwdg.o ./Core/Src/stm32f4xx_hal_iwdg.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_spi.cyclo ./Core/Src/stm32f4xx_hal_spi.d ./Core/Src/stm32f4xx_hal_spi.o ./Core/Src/stm32f4xx_hal_spi.su ./Core/Src/stm32f4xx_hal_sram.cyclo ./Core/Src/stm32f4xx_hal_sram.d ./Core/Src/stm32f4xx_hal_sram.o ./Core/Src/stm32f4xx_hal_sram.su ./Core/Src/stm32f4xx_hal_uart.cyclo ./Core/Src/stm32f4xx_hal_uart.d ./Core/Src/stm32f4xx_hal_uart.o ./Core/Src/stm32f4xx_hal_uart.su ./Core/Src/stm32f4xx_hal_usart.cyclo ./Core/Src/stm32f4xx_hal_usart.d ./Core/Src/stm32f4xx_hal_usart.o ./Core/Src/stm32f4xx_hal_usart.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/stm32f4xx_ll_fmc.cyclo ./Core/Src/stm32f4xx_ll_fmc.d ./Core/Src/stm32f4xx_ll_fmc.o ./Core/Src/stm32f4xx_ll_fmc.su ./Core/Src/stm32f4xx_ll_fsmc.cyclo ./Core/Src/stm32f4xx_ll_fsmc.d ./Core/Src/stm32f4xx_ll_fsmc.o ./Core/Src/stm32f4xx_ll_fsmc.su ./Core/Src/stm32f4xx_ll_spi.cyclo ./Core/Src/stm32f4xx_ll_spi.d ./Core/Src/stm32f4xx_ll_spi.o ./Core/Src/stm32f4xx_ll_spi.su ./Core/Src/stm32f4xx_ll_usart.cyclo ./Core/Src/stm32f4xx_ll_usart.d ./Core/Src/stm32f4xx_ll_usart.o ./Core/Src/stm32f4xx_ll_usart.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

