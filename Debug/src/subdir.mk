################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Configure.cpp \
../src/MobileClient.cpp \
../src/Scheduler.cpp \
../src/Server.cpp \
../src/Statistics.cpp \
../src/main.cpp \
../src/util.cpp 

OBJS += \
./src/Configure.o \
./src/MobileClient.o \
./src/Scheduler.o \
./src/Server.o \
./src/Statistics.o \
./src/main.o \
./src/util.o 

CPP_DEPS += \
./src/Configure.d \
./src/MobileClient.d \
./src/Scheduler.d \
./src/Server.d \
./src/Statistics.d \
./src/main.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"D:\workspaces\cpp\databroadcast\lib\jsoncpp" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


