################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/audio_processor.c \
../Src/bsp.c \
../Src/clock.c \
../Src/fpu.c \
../Src/i2s.c \
../Src/main.c \
../Src/pdm2pcm.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f7xx.c \
../Src/timebase.c \
../Src/uart.c 

OBJS += \
./Src/adc.o \
./Src/audio_processor.o \
./Src/bsp.o \
./Src/clock.o \
./Src/fpu.o \
./Src/i2s.o \
./Src/main.o \
./Src/pdm2pcm.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f7xx.o \
./Src/timebase.o \
./Src/uart.o 

C_DEPS += \
./Src/adc.d \
./Src/audio_processor.d \
./Src/bsp.d \
./Src/clock.d \
./Src/fpu.d \
./Src/i2s.d \
./Src/main.d \
./Src/pdm2pcm.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f7xx.d \
./Src/timebase.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F746VGTx -DSTM32F7 -DSTM32F746xx -DUSE_HAL_DRIVER -c -I../Inc -I"C:/Users/ACER/Documents/GitHub/Firmware-design/stm32_project/12_pdm_audio_processing/Inc" -I"C:/Users/ACER/Documents/GitHub/Firmware-design/stm32_project/12_pdm_audio_processing/Middlewares/ST/STM32_Audio/Addons/PDM/Inc" -I"C:/Users/ACER/Documents/GitHub/Firmware-design/stm32_project/12_pdm_audio_processing/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/ACER/Documents/GitHub/Firmware-design/stm32_project/12_pdm_audio_processing/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/ACER/Documents/GitHub/Firmware-design/stm32_project/12_pdm_audio_processing/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/adc.cyclo ./Src/adc.d ./Src/adc.o ./Src/adc.su ./Src/audio_processor.cyclo ./Src/audio_processor.d ./Src/audio_processor.o ./Src/audio_processor.su ./Src/bsp.cyclo ./Src/bsp.d ./Src/bsp.o ./Src/bsp.su ./Src/clock.cyclo ./Src/clock.d ./Src/clock.o ./Src/clock.su ./Src/fpu.cyclo ./Src/fpu.d ./Src/fpu.o ./Src/fpu.su ./Src/i2s.cyclo ./Src/i2s.d ./Src/i2s.o ./Src/i2s.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/pdm2pcm.cyclo ./Src/pdm2pcm.d ./Src/pdm2pcm.o ./Src/pdm2pcm.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32f7xx.cyclo ./Src/system_stm32f7xx.d ./Src/system_stm32f7xx.o ./Src/system_stm32f7xx.su ./Src/timebase.cyclo ./Src/timebase.d ./Src/timebase.o ./Src/timebase.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

