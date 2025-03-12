################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/bsp/Src/bsp_led.c 

OBJS += \
./Core/bsp/Src/bsp_led.o 

C_DEPS += \
./Core/bsp/Src/bsp_led.d 


# Each subdirectory must supply rules for building sources it contributes
Core/bsp/Src/%.o Core/bsp/Src/%.su Core/bsp/Src/%.cyclo: ../Core/bsp/Src/%.c Core/bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/bsp -I../Core/bsp/Inc -I../Core/bsp/Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-bsp-2f-Src

clean-Core-2f-bsp-2f-Src:
	-$(RM) ./Core/bsp/Src/bsp_led.cyclo ./Core/bsp/Src/bsp_led.d ./Core/bsp/Src/bsp_led.o ./Core/bsp/Src/bsp_led.su

.PHONY: clean-Core-2f-bsp-2f-Src

