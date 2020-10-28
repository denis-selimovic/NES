# NES emulator

## What is it?

This is a simple NES (Nintendo Entertainment System) emulator made for educational purposes as a graduation assignment at the Faculty of Electrical Engineering, University of Sarajevo. It should support all Mapper 0 ROM-s

## What do you need to use it ?

Following software/packages/libraries are required to run this emulator:
* CMake for your operating system
* SDL2 library for your operating system
* SDL2_TTF library for your operating system

## How do you use it? 

If you have access to Mapper 0 ROM, you can start this emulator. Use constructor of __Renderer__ class in __main.cpp__. It needs full path to the given ROM in your file system. After that, you can build source code with __cmake --build cmake-build-debug --target NES -- -j 6__ comand which should generate executable that can be started on your opearting system.

## Controls

* Arrow keys - UP, DOWN, LEFT, RIGHT
* Keys X, Z, A, S
