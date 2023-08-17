################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/API/Src/API_delay.c \
../Core/API/Src/BM280.c \
../Core/API/Src/FSM_termometro.c \
../Core/API/Src/LCD44780.c \
../Core/API/Src/port.c 

OBJS += \
./Core/API/Src/API_delay.o \
./Core/API/Src/BM280.o \
./Core/API/Src/FSM_termometro.o \
./Core/API/Src/LCD44780.o \
./Core/API/Src/port.o 

C_DEPS += \
./Core/API/Src/API_delay.d \
./Core/API/Src/BM280.d \
./Core/API/Src/FSM_termometro.d \
./Core/API/Src/LCD44780.d \
./Core/API/Src/port.d 


# Each subdirectory must supply rules for building sources it contributes
Core/API/Src/%.o Core/API/Src/%.su Core/API/Src/%.cyclo: ../Core/API/Src/%.c Core/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/rome/STM32CubeIDE/workspace_1.12.1/RaulRomero_workspace/TF_Termometro2/Core/API/Inc" -I"/home/rome/STM32CubeIDE/workspace_1.12.1/RaulRomero_workspace/TF_Termometro2/Core/API" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-API-2f-Src

clean-Core-2f-API-2f-Src:
	-$(RM) ./Core/API/Src/API_delay.cyclo ./Core/API/Src/API_delay.d ./Core/API/Src/API_delay.o ./Core/API/Src/API_delay.su ./Core/API/Src/BM280.cyclo ./Core/API/Src/BM280.d ./Core/API/Src/BM280.o ./Core/API/Src/BM280.su ./Core/API/Src/FSM_termometro.cyclo ./Core/API/Src/FSM_termometro.d ./Core/API/Src/FSM_termometro.o ./Core/API/Src/FSM_termometro.su ./Core/API/Src/LCD44780.cyclo ./Core/API/Src/LCD44780.d ./Core/API/Src/LCD44780.o ./Core/API/Src/LCD44780.su ./Core/API/Src/port.cyclo ./Core/API/Src/port.d ./Core/API/Src/port.o ./Core/API/Src/port.su

.PHONY: clean-Core-2f-API-2f-Src

