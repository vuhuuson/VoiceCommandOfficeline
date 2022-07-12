################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/Model/CybModel1.asm \
../src/Model/CybModel2.asm \
../src/Model/SpeexData1.asm \
../src/Model/SpeexData2.asm 

OBJS += \
./src/Model/CybModel1.o \
./src/Model/CybModel2.o \
./src/Model/SpeexData1.o \
./src/Model/SpeexData2.o 

ASM_DEPS += \
./src/Model/CybModel1.d \
./src/Model/CybModel2.d \
./src/Model/SpeexData1.d \
./src/Model/SpeexData2.d 


# Each subdirectory must supply rules for building sources it contributes
src/Model/%.o: ../src/Model/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -x assembler-with-cpp -D_RENESAS_RA_ -I"C:\Users\sonvhc\Desktop\1-TU\Bkav_VC01\src" -I"C:\Users\sonvhc\Desktop\1-TU\Bkav_VC01\ra\fsp\inc" -I"C:\Users\sonvhc\Desktop\1-TU\Bkav_VC01\ra\fsp\inc\api" -I"C:\Users\sonvhc\Desktop\1-TU\Bkav_VC01\ra\fsp\inc\instances" -I"C:\Users\sonvhc\Desktop\1-TU\Bkav_VC01\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\sonvhc\Desktop\1-TU\Bkav_VC01\ra_gen" -I"C:\Users\sonvhc\Desktop\1-TU\Bkav_VC01\ra_cfg\fsp_cfg" -I"C:\Users\sonvhc\Desktop\1-TU\Bkav_VC01\ra_cfg\fsp_cfg\bsp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


