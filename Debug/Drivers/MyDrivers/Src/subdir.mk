################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/MyDrivers/Src/Gps.cpp \
../Drivers/MyDrivers/Src/Imu.cpp \
../Drivers/MyDrivers/Src/Motor.cpp \
../Drivers/MyDrivers/Src/Paket.cpp \
../Drivers/MyDrivers/Src/RfCom.cpp \
../Drivers/MyDrivers/Src/gorevyonetici.cpp \
../Drivers/MyDrivers/Src/myUart.cpp \
../Drivers/MyDrivers/Src/mytimer.cpp 

OBJS += \
./Drivers/MyDrivers/Src/Gps.o \
./Drivers/MyDrivers/Src/Imu.o \
./Drivers/MyDrivers/Src/Motor.o \
./Drivers/MyDrivers/Src/Paket.o \
./Drivers/MyDrivers/Src/RfCom.o \
./Drivers/MyDrivers/Src/gorevyonetici.o \
./Drivers/MyDrivers/Src/myUart.o \
./Drivers/MyDrivers/Src/mytimer.o 

CPP_DEPS += \
./Drivers/MyDrivers/Src/Gps.d \
./Drivers/MyDrivers/Src/Imu.d \
./Drivers/MyDrivers/Src/Motor.d \
./Drivers/MyDrivers/Src/Paket.d \
./Drivers/MyDrivers/Src/RfCom.d \
./Drivers/MyDrivers/Src/gorevyonetici.d \
./Drivers/MyDrivers/Src/myUart.d \
./Drivers/MyDrivers/Src/mytimer.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MyDrivers/Src/%.o Drivers/MyDrivers/Src/%.su Drivers/MyDrivers/Src/%.cyclo: ../Drivers/MyDrivers/Src/%.cpp Drivers/MyDrivers/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/MyDrivers/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MyDrivers-2f-Src

clean-Drivers-2f-MyDrivers-2f-Src:
	-$(RM) ./Drivers/MyDrivers/Src/Gps.cyclo ./Drivers/MyDrivers/Src/Gps.d ./Drivers/MyDrivers/Src/Gps.o ./Drivers/MyDrivers/Src/Gps.su ./Drivers/MyDrivers/Src/Imu.cyclo ./Drivers/MyDrivers/Src/Imu.d ./Drivers/MyDrivers/Src/Imu.o ./Drivers/MyDrivers/Src/Imu.su ./Drivers/MyDrivers/Src/Motor.cyclo ./Drivers/MyDrivers/Src/Motor.d ./Drivers/MyDrivers/Src/Motor.o ./Drivers/MyDrivers/Src/Motor.su ./Drivers/MyDrivers/Src/Paket.cyclo ./Drivers/MyDrivers/Src/Paket.d ./Drivers/MyDrivers/Src/Paket.o ./Drivers/MyDrivers/Src/Paket.su ./Drivers/MyDrivers/Src/RfCom.cyclo ./Drivers/MyDrivers/Src/RfCom.d ./Drivers/MyDrivers/Src/RfCom.o ./Drivers/MyDrivers/Src/RfCom.su ./Drivers/MyDrivers/Src/gorevyonetici.cyclo ./Drivers/MyDrivers/Src/gorevyonetici.d ./Drivers/MyDrivers/Src/gorevyonetici.o ./Drivers/MyDrivers/Src/gorevyonetici.su ./Drivers/MyDrivers/Src/myUart.cyclo ./Drivers/MyDrivers/Src/myUart.d ./Drivers/MyDrivers/Src/myUart.o ./Drivers/MyDrivers/Src/myUart.su ./Drivers/MyDrivers/Src/mytimer.cyclo ./Drivers/MyDrivers/Src/mytimer.d ./Drivers/MyDrivers/Src/mytimer.o ./Drivers/MyDrivers/Src/mytimer.su

.PHONY: clean-Drivers-2f-MyDrivers-2f-Src

