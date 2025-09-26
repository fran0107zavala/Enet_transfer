################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/fsl_adapter_uart.c 

C_DEPS += \
./component/uart/fsl_adapter_uart.d 

OBJS += \
./component/uart/fsl_adapter_uart.o 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c component/uart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K64F -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\source" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\frdmk64f_enet_txrx_transfer\lwip\src\include\lwip" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\mdio" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\phy" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\utilities" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\drivers" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\device" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\component\uart" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\component\lists" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\CMSIS" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\board" -I"C:\Users\fcoda\Documents\MCUXpressoIDE_25.6.136\workspace\Practica1_NuevaCapa\frdmk64f\driver_examples\enet\txrx_transfer" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-uart

clean-component-2f-uart:
	-$(RM) ./component/uart/fsl_adapter_uart.d ./component/uart/fsl_adapter_uart.o

.PHONY: clean-component-2f-uart

