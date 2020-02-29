cmake_minimum_required(VERSION 3.15)
project(NES)

set(CMAKE_CXX_STANDARD 17)
add_library(operation Operation.cpp)
add_library(adrmode AddressingMode.cpp)
add_library(cpu cpu6502.cpp)
target_link_libraries(cpu PUBLIC operation)
target_link_libraries(cpu PUBLIC adrmode)

find_package(SDL2 REQUIRED)
include_directories(${SDLA2_INCLUDE_DIRS})

add_executable(NES main.cpp Instruction.cpp Instruction.h AddressingMode.cpp AddressingMode.h Operation.cpp Operation.h Bus.cpp Bus.h cpu6502.cpp cpu6502.h ppu2C02.cpp ppu2C02.h GamePak.cpp GamePak.h Renderer.cpp Renderer.h Mapper.cpp Mapper.h Mapper000.cpp Mapper000.h Debugger.cpp Debugger.h Utility.h Utility.cpp)
target_link_libraries(NES ${SDL2_LIBRARIES})