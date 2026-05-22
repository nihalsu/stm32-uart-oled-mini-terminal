################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ssd1306_Library/ssd1306.c \
../ssd1306_Library/ssd1306_fonts.c 

OBJS += \
./ssd1306_Library/ssd1306.o \
./ssd1306_Library/ssd1306_fonts.o 

C_DEPS += \
./ssd1306_Library/ssd1306.d \
./ssd1306_Library/ssd1306_fonts.d 


# Each subdirectory must supply rules for building sources it contributes
ssd1306_Library/%.o ssd1306_Library/%.su ssd1306_Library/%.cyclo: ../ssd1306_Library/%.c ssd1306_Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Dell/STM32CubeIDE/workspace_1.19.0/realtime_write/ssd1306_Library" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ssd1306_Library

clean-ssd1306_Library:
	-$(RM) ./ssd1306_Library/ssd1306.cyclo ./ssd1306_Library/ssd1306.d ./ssd1306_Library/ssd1306.o ./ssd1306_Library/ssd1306.su ./ssd1306_Library/ssd1306_fonts.cyclo ./ssd1306_Library/ssd1306_fonts.d ./ssd1306_Library/ssd1306_fonts.o ./ssd1306_Library/ssd1306_fonts.su

.PHONY: clean-ssd1306_Library

