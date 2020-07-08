cmake_minimum_required(VERSION 3.15)
project(NES)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${NES_SOURCE_DIR}/cmake")
add_library(operation Operation.cpp)
add_library(adrmode AddressingMode.cpp)
add_library(cpu CPU.cpp)
target_link_libraries(cpu PUBLIC operation)
target_link_libraries(cpu PUBLIC adrmode)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(NES main.cpp Instruction.cpp Instruction.h AddressingMode.cpp AddressingMode.h Operation.cpp Operation.h Bus.cpp Bus.h CPU.cpp CPU.h ppu2C02.cpp ppu2C02.h GamePak.cpp GamePak.h Renderer.cpp Renderer.h Mapper.cpp Mapper.h Mapper000.cpp Mapper000.h Debugger.cpp Debugger.h Disassembler.cpp Disassembler.h Key.cpp Key.h)
target_link_libraries(NES ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})