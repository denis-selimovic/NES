//
// Created by denis on 28/02/2020.
//

#ifndef NES_DEBUGGER_H
#define NES_DEBUGGER_H


#include <iostream>
#include "Bus.h"
#include <SDL2/SDL.h>

class Debugger {

    //dimenzije ekrana
    const static uint32_t WINDOW_WIDTH = 640;
    const static uint32_t WINDOW_HEIGHT = 480;

    //NES
    cpu6502 cpu;
    ppu2C02 ppu;
    GamePak *gamePak;
    Bus * bus;

    //SDL
    void logError(std::ostream &os, const std::string &error);
    void createWindow();
    void createRenderer();

    //atributi
    SDL_Window *window;
    SDL_Renderer *renderer;
public:
    Debugger(const std::string &test);
    ~Debugger();
    Debugger(const Debugger &debugger) = delete;
    Debugger(Debugger &&debugger) = delete;
};


#endif //NES_DEBUGGER_H
