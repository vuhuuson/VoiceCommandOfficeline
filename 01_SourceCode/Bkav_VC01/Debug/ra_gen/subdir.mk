################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_RA_ -DUART_LOG -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\src" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\ra\fsp\inc" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\ra\fsp\inc\api" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\ra\fsp\inc\instances" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\ra_gen" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\ra_cfg\fsp_cfg" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\ra_cfg\fsp_cfg\bsp" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\src\DSpotter\include" -I"C:\Users\SonVHc\Desktop\BkavDemo2022101115\01_SourceCode\Bkav_VC01\src\Speex\include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


