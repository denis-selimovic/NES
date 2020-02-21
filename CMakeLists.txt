cmake_minimum_required(VERSION 3.15)
project(NES)

set(CMAKE_CXX_STANDARD 17)

add_executable(NES main.cpp cpu6502.cpp cpu6502.h Instruction.cpp Instruction.h)