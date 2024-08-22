################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/MyDrivers/Src/Barometre.cpp \
../Drivers/MyDrivers/Src/Gps.cpp \
../Drivers/MyDrivers/Src/Imu.cpp \
../Drivers/MyDrivers/Src/IsaretIsleme.cpp \
../Drivers/MyDrivers/Src/Kalman.cpp \
../Drivers/MyDrivers/Src/Mag.cpp \
../Drivers/MyDrivers/Src/Motor.cpp \
../Drivers/MyDrivers/Src/Paket.cpp \
../Drivers/MyDrivers/Src/Timer.cpp \
../Drivers/MyDrivers/Src/VoltajSensor.cpp \
../Drivers/MyDrivers/Src/gorevyonetici.cpp 

OBJS += \
./Drivers/MyDrivers/Src/Barometre.o \
./Drivers/MyDrivers/Src/Gps.o \
./Drivers/MyDrivers/Src/Imu.o \
./Drivers/MyDrivers/Src/IsaretIsleme.o \
./Drivers/MyDrivers/Src/Kalman.o \
./Drivers/MyDrivers/Src/Mag.o \
./Drivers/MyDrivers/Src/Motor.o \
./Drivers/MyDrivers/Src/Paket.o \
./Drivers/MyDrivers/Src/Timer.o \
./Drivers/MyDrivers/Src/VoltajSensor.o \
./Drivers/MyDrivers/Src/gorevyonetici.o 

CPP_DEPS += \
./Drivers/MyDrivers/Src/Barometre.d \
./Drivers/MyDrivers/Src/Gps.d \
./Drivers/MyDrivers/Src/Imu.d \
./Drivers/MyDrivers/Src/IsaretIsleme.d \
./Drivers/MyDrivers/Src/Kalman.d \
./Drivers/MyDrivers/Src/Mag.d \
./Drivers/MyDrivers/Src/Motor.d \
./Drivers/MyDrivers/Src/Paket.d \
./Drivers/MyDrivers/Src/Timer.d \
./Drivers/MyDrivers/Src/VoltajSensor.d \
./Drivers/MyDrivers/Src/gorevyonetici.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MyDrivers/Src/%.o Drivers/MyDrivers/Src/%.su Drivers/MyDrivers/Src/%.cyclo: ../Drivers/MyDrivers/Src/%.cpp Drivers/MyDrivers/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/MyDrivers/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MyDrivers-2f-Src

clean-Drivers-2f-MyDrivers-2f-Src:
	-$(RM) ./Drivers/MyDrivers/Src/Barometre.cyclo ./Drivers/MyDrivers/Src/Barometre.d ./Drivers/MyDrivers/Src/Barometre.o ./Drivers/MyDrivers/Src/Barometre.su ./Drivers/MyDrivers/Src/Gps.cyclo ./Drivers/MyDrivers/Src/Gps.d ./Drivers/MyDrivers/Src/Gps.o ./Drivers/MyDrivers/Src/Gps.su ./Drivers/MyDrivers/Src/Imu.cyclo ./Drivers/MyDrivers/Src/Imu.d ./Drivers/MyDrivers/Src/Imu.o ./Drivers/MyDrivers/Src/Imu.su ./Drivers/MyDrivers/Src/IsaretIsleme.cyclo ./Drivers/MyDrivers/Src/IsaretIsleme.d ./Drivers/MyDrivers/Src/IsaretIsleme.o ./Drivers/MyDrivers/Src/IsaretIsleme.su ./Drivers/MyDrivers/Src/Kalman.cyclo ./Drivers/MyDrivers/Src/Kalman.d ./Drivers/MyDrivers/Src/Kalman.o ./Drivers/MyDrivers/Src/Kalman.su ./Drivers/MyDrivers/Src/Mag.cyclo ./Drivers/MyDrivers/Src/Mag.d ./Drivers/MyDrivers/Src/Mag.o ./Drivers/MyDrivers/Src/Mag.su ./Drivers/MyDrivers/Src/Motor.cyclo ./Drivers/MyDrivers/Src/Motor.d ./Drivers/MyDrivers/Src/Motor.o ./Drivers/MyDrivers/Src/Motor.su ./Drivers/MyDrivers/Src/Paket.cyclo ./Drivers/MyDrivers/Src/Paket.d ./Drivers/MyDrivers/Src/Paket.o ./Drivers/MyDrivers/Src/Paket.su ./Drivers/MyDrivers/Src/Timer.cyclo ./Drivers/MyDrivers/Src/Timer.d ./Drivers/MyDrivers/Src/Timer.o ./Drivers/MyDrivers/Src/Timer.su ./Drivers/MyDrivers/Src/VoltajSensor.cyclo ./Drivers/MyDrivers/Src/VoltajSensor.d ./Drivers/MyDrivers/Src/VoltajSensor.o ./Drivers/MyDrivers/Src/VoltajSensor.su ./Drivers/MyDrivers/Src/gorevyonetici.cyclo ./Drivers/MyDrivers/Src/gorevyonetici.d ./Drivers/MyDrivers/Src/gorevyonetici.o ./Drivers/MyDrivers/Src/gorevyonetici.su

.PHONY: clean-Drivers-2f-MyDrivers-2f-Src

