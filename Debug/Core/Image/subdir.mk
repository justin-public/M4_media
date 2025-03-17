################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Image/ADC.c \
../Core/Image/Camera.c \
../Core/Image/ChiLun.c \
../Core/Image/FM.c \
../Core/Image/GPS.c \
../Core/Image/LCD.c \
../Core/Image/MP3.c \
../Core/Image/Phone.c \
../Core/Image/Recorder.c \
../Core/Image/USBDisk.c \
../Core/Image/Wireless.c \
../Core/Image/chujiao.c \
../Core/Image/hwinfo.c \
../Core/Image/radio.c \
../Core/Image/web.c 

OBJS += \
./Core/Image/ADC.o \
./Core/Image/Camera.o \
./Core/Image/ChiLun.o \
./Core/Image/FM.o \
./Core/Image/GPS.o \
./Core/Image/LCD.o \
./Core/Image/MP3.o \
./Core/Image/Phone.o \
./Core/Image/Recorder.o \
./Core/Image/USBDisk.o \
./Core/Image/Wireless.o \
./Core/Image/chujiao.o \
./Core/Image/hwinfo.o \
./Core/Image/radio.o \
./Core/Image/web.o 

C_DEPS += \
./Core/Image/ADC.d \
./Core/Image/Camera.d \
./Core/Image/ChiLun.d \
./Core/Image/FM.d \
./Core/Image/GPS.d \
./Core/Image/LCD.d \
./Core/Image/MP3.d \
./Core/Image/Phone.d \
./Core/Image/Recorder.d \
./Core/Image/USBDisk.d \
./Core/Image/Wireless.d \
./Core/Image/chujiao.d \
./Core/Image/hwinfo.d \
./Core/Image/radio.d \
./Core/Image/web.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Image/%.o Core/Image/%.su Core/Image/%.cyclo: ../Core/Image/%.c Core/Image/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/FatFS -I../Core/Fonts -I../Core/Image -I../Core/bsp -I../Core/bsp/Inc -I../Core/bsp/Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Image

clean-Core-2f-Image:
	-$(RM) ./Core/Image/ADC.cyclo ./Core/Image/ADC.d ./Core/Image/ADC.o ./Core/Image/ADC.su ./Core/Image/Camera.cyclo ./Core/Image/Camera.d ./Core/Image/Camera.o ./Core/Image/Camera.su ./Core/Image/ChiLun.cyclo ./Core/Image/ChiLun.d ./Core/Image/ChiLun.o ./Core/Image/ChiLun.su ./Core/Image/FM.cyclo ./Core/Image/FM.d ./Core/Image/FM.o ./Core/Image/FM.su ./Core/Image/GPS.cyclo ./Core/Image/GPS.d ./Core/Image/GPS.o ./Core/Image/GPS.su ./Core/Image/LCD.cyclo ./Core/Image/LCD.d ./Core/Image/LCD.o ./Core/Image/LCD.su ./Core/Image/MP3.cyclo ./Core/Image/MP3.d ./Core/Image/MP3.o ./Core/Image/MP3.su ./Core/Image/Phone.cyclo ./Core/Image/Phone.d ./Core/Image/Phone.o ./Core/Image/Phone.su ./Core/Image/Recorder.cyclo ./Core/Image/Recorder.d ./Core/Image/Recorder.o ./Core/Image/Recorder.su ./Core/Image/USBDisk.cyclo ./Core/Image/USBDisk.d ./Core/Image/USBDisk.o ./Core/Image/USBDisk.su ./Core/Image/Wireless.cyclo ./Core/Image/Wireless.d ./Core/Image/Wireless.o ./Core/Image/Wireless.su ./Core/Image/chujiao.cyclo ./Core/Image/chujiao.d ./Core/Image/chujiao.o ./Core/Image/chujiao.su ./Core/Image/hwinfo.cyclo ./Core/Image/hwinfo.d ./Core/Image/hwinfo.o ./Core/Image/hwinfo.su ./Core/Image/radio.cyclo ./Core/Image/radio.d ./Core/Image/radio.o ./Core/Image/radio.su ./Core/Image/web.cyclo ./Core/Image/web.d ./Core/Image/web.o ./Core/Image/web.su

.PHONY: clean-Core-2f-Image

