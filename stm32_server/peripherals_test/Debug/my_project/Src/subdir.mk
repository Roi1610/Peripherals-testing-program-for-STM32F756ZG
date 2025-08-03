################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../my_project/Src/server.c \
../my_project/Src/test_adc.c \
../my_project/Src/test_i2c.c \
../my_project/Src/test_spi.c \
../my_project/Src/test_timer.c \
../my_project/Src/test_uart.c 

OBJS += \
./my_project/Src/server.o \
./my_project/Src/test_adc.o \
./my_project/Src/test_i2c.o \
./my_project/Src/test_spi.o \
./my_project/Src/test_timer.o \
./my_project/Src/test_uart.o 

C_DEPS += \
./my_project/Src/server.d \
./my_project/Src/test_adc.d \
./my_project/Src/test_i2c.d \
./my_project/Src/test_spi.d \
./my_project/Src/test_timer.d \
./my_project/Src/test_uart.d 


# Each subdirectory must supply rules for building sources it contributes
my_project/Src/%.o my_project/Src/%.su my_project/Src/%.cyclo: ../my_project/Src/%.c my_project/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Drivers/CMSIS/Include -I"D:/rtg2024/ARM/lesson15/peripherals_test/my_project/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_project-2f-Src

clean-my_project-2f-Src:
	-$(RM) ./my_project/Src/server.cyclo ./my_project/Src/server.d ./my_project/Src/server.o ./my_project/Src/server.su ./my_project/Src/test_adc.cyclo ./my_project/Src/test_adc.d ./my_project/Src/test_adc.o ./my_project/Src/test_adc.su ./my_project/Src/test_i2c.cyclo ./my_project/Src/test_i2c.d ./my_project/Src/test_i2c.o ./my_project/Src/test_i2c.su ./my_project/Src/test_spi.cyclo ./my_project/Src/test_spi.d ./my_project/Src/test_spi.o ./my_project/Src/test_spi.su ./my_project/Src/test_timer.cyclo ./my_project/Src/test_timer.d ./my_project/Src/test_timer.o ./my_project/Src/test_timer.su ./my_project/Src/test_uart.cyclo ./my_project/Src/test_uart.d ./my_project/Src/test_uart.o ./my_project/Src/test_uart.su

.PHONY: clean-my_project-2f-Src

