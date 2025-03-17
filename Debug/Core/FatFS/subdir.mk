################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/FatFS/diskio.c \
../Core/FatFS/ff.c 

OBJS += \
./Core/FatFS/diskio.o \
./Core/FatFS/ff.o 

C_DEPS += \
./Core/FatFS/diskio.d \
./Core/FatFS/ff.d 


# Each subdirectory must supply rules for building sources it contributes
Core/FatFS/%.o Core/FatFS/%.su Core/FatFS/%.cyclo: ../Core/FatFS/%.c Core/FatFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/FatFS -I../Core/Fonts -I../Core/Image -I../Core/bsp -I../Core/bsp/Inc -I../Core/bsp/Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-FatFS

clean-Core-2f-FatFS:
	-$(RM) ./Core/FatFS/diskio.cyclo ./Core/FatFS/diskio.d ./Core/FatFS/diskio.o ./Core/FatFS/diskio.su ./Core/FatFS/ff.cyclo ./Core/FatFS/ff.d ./Core/FatFS/ff.o ./Core/FatFS/ff.su

.PHONY: clean-Core-2f-FatFS

