################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/rmuo/RMUOClient.cpp \
../src/rmuo/RMUOScheduler.cpp 

OBJS += \
./src/rmuo/RMUOClient.o \
./src/rmuo/RMUOScheduler.o 

CPP_DEPS += \
./src/rmuo/RMUOClient.d \
./src/rmuo/RMUOScheduler.d 


# Each subdirectory must supply rules for building sources it contributes
src/rmuo/%.o: ../src/rmuo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"D:\workspaces\cpp\databroadcast\lib\jsoncpp" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


