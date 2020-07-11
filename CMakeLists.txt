cmake_minimum_required(VERSION 3.15)
project(NES)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${NES_SOURCE_DIR}/cmake")
add_library(operation src/Operation.cpp)
add_library(adrmode src/AddressingMode.cpp)
add_library(cpu src/CPU.cpp)
target_link_libraries(cpu PUBLIC operation)
target_link_libraries(cpu PUBLIC adrmode)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(NES main.cpp src/Instruction.cpp include/Instruction.h src/AddressingMode.cpp include/AddressingMode.h src/Operation.cpp include/Operation.h src/Bus.cpp include/Bus.h src/CPU.cpp include/CPU.h src/PPU.cpp include/PPU.h src/GamePak.cpp include/GamePak.h src/Renderer.cpp include/Renderer.h src/Mapper.cpp include/Mapper.h src/NROMMapper.cpp include/NROMMapper.h src/Debugger.cpp include/Debugger.h src/Disassembler.cpp include/Disassembler.h src/Joystick.cpp include/Joystick.h src/ppu/Tile.cpp include/ppu/Tile.h src/ppu/Shifter.cpp include/ppu/Shifter.h src/ppu/Sprite.cpp include/ppu/Sprite.h)
target_link_libraries(NES ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})