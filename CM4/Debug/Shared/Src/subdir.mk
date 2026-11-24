################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/sebaw/OneDrive/Robotic\ arm/Software/H7/MainBoard/Shared/Src/helpers.c 

OBJS += \
./Shared/Src/helpers.o 

C_DEPS += \
./Shared/Src/helpers.d 


# Each subdirectory must supply rules for building sources it contributes
Shared/Src/helpers.o: C:/Users/sebaw/OneDrive/Robotic\ arm/Software/H7/MainBoard/Shared/Src/helpers.c Shared/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/BSP/STM32H7xx_Nucleo -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/sebaw/OneDrive/Robotic arm/Software/H7/MainBoard/Shared/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -ftls-model=local-exec -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Shared-2f-Src

clean-Shared-2f-Src:
	-$(RM) ./Shared/Src/helpers.cyclo ./Shared/Src/helpers.d ./Shared/Src/helpers.o ./Shared/Src/helpers.su

.PHONY: clean-Shared-2f-Src

