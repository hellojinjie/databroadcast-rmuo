################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/jsoncpp/json_reader.cpp \
../lib/jsoncpp/json_value.cpp \
../lib/jsoncpp/json_writer.cpp 

OBJS += \
./lib/jsoncpp/json_reader.o \
./lib/jsoncpp/json_value.o \
./lib/jsoncpp/json_writer.o 

CPP_DEPS += \
./lib/jsoncpp/json_reader.d \
./lib/jsoncpp/json_value.d \
./lib/jsoncpp/json_writer.d 


# Each subdirectory must supply rules for building sources it contributes
lib/jsoncpp/%.o: ../lib/jsoncpp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"D:\workspaces\cpp\databroadcast\lib\jsoncpp" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


