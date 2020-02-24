cmake_minimum_required(VERSION 3.15)
project(NES)

set(CMAKE_CXX_STANDARD 17)
add_library(operation Operation.cpp)
add_library(adrmode AddressingMode.cpp)
add_library(cpu cpu6502.cpp)
target_link_libraries(cpu PUBLIC operation)
target_link_libraries(cpu PUBLIC adrmode)

add_executable(NES main.cpp Instruction.cpp Instruction.h AddressingMode.cpp AddressingMode.h Operation.cpp Operation.h Bus.cpp Bus.h cpu6502.cpp cpu6502.h ppu2C02.cpp ppu2C02.h)