################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Fonts/asc12.c \
../Core/Fonts/asc16.c \
../Core/Fonts/hz12.c \
../Core/Fonts/hz16.c 

OBJS += \
./Core/Fonts/asc12.o \
./Core/Fonts/asc16.o \
./Core/Fonts/hz12.o \
./Core/Fonts/hz16.o 

C_DEPS += \
./Core/Fonts/asc12.d \
./Core/Fonts/asc16.d \
./Core/Fonts/hz12.d \
./Core/Fonts/hz16.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Fonts/%.o Core/Fonts/%.su Core/Fonts/%.cyclo: ../Core/Fonts/%.c Core/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/FatFS -I../Core/Fonts -I../Core/Image -I../Core/bsp -I../Core/bsp/Inc -I../Core/bsp/Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Fonts

clean-Core-2f-Fonts:
	-$(RM) ./Core/Fonts/asc12.cyclo ./Core/Fonts/asc12.d ./Core/Fonts/asc12.o ./Core/Fonts/asc12.su ./Core/Fonts/asc16.cyclo ./Core/Fonts/asc16.d ./Core/Fonts/asc16.o ./Core/Fonts/asc16.su ./Core/Fonts/hz12.cyclo ./Core/Fonts/hz12.d ./Core/Fonts/hz12.o ./Core/Fonts/hz12.su ./Core/Fonts/hz16.cyclo ./Core/Fonts/hz16.d ./Core/Fonts/hz16.o ./Core/Fonts/hz16.su

.PHONY: clean-Core-2f-Fonts

