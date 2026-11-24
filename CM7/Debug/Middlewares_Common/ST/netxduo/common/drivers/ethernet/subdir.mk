################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
./Middlewares_Common/ST/netxduo/common/drivers/ethernet/nx_stm32_eth_driver.o 

C_DEPS += \
./Middlewares_Common/ST/netxduo/common/drivers/ethernet/nx_stm32_eth_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares_Common/ST/netxduo/common/drivers/ethernet/nx_stm32_eth_driver.o: C:/Users/sebaw/OneDrive/Robotic\ arm/Software/H7/MainBoard/Middlewares/ST/netxduo/common/drivers/ethernet/nx_stm32_eth_driver.c Middlewares_Common/ST/netxduo/common/drivers/ethernet/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_PWR_LDO_SUPPLY -DSTM32_THREAD_SAFE_STRATEGY=2 -DTX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/BSP/STM32H7xx_Nucleo -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/ThreadSafe -I../AZURE_RTOS/App -I../NetXDuo/App -I../NetXDuo/Target -I../../Drivers/BSP/Components/lan8742/ -I../../Middlewares/ST/netxduo/common/drivers/ethernet/ -I../../Middlewares/ST/netxduo/addons/dhcp/ -I../../Middlewares/ST/netxduo/common/inc/ -I../../Middlewares/ST/netxduo/ports/cortex_m7/gnu/inc/ -I../../Middlewares/ST/threadx/common/inc/ -I../../Middlewares/ST/threadx/ports/cortex_m7/gnu/inc/ -I../../Middlewares/ST/netxduo/addons/sntp/ -I"C:/Users/sebaw/OneDrive/Robotic arm/Software/H7/MainBoard/Shared/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -ftls-model=local-exec -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares_Common-2f-ST-2f-netxduo-2f-common-2f-drivers-2f-ethernet

clean-Middlewares_Common-2f-ST-2f-netxduo-2f-common-2f-drivers-2f-ethernet:
	-$(RM) ./Middlewares_Common/ST/netxduo/common/drivers/ethernet/nx_stm32_eth_driver.cyclo ./Middlewares_Common/ST/netxduo/common/drivers/ethernet/nx_stm32_eth_driver.d ./Middlewares_Common/ST/netxduo/common/drivers/ethernet/nx_stm32_eth_driver.o ./Middlewares_Common/ST/netxduo/common/drivers/ethernet/nx_stm32_eth_driver.su

.PHONY: clean-Middlewares_Common-2f-ST-2f-netxduo-2f-common-2f-drivers-2f-ethernet

