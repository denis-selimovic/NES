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
    const static uint32_t WINDOW_WIDTH = 1400;
    const static uint32_t WINDOW_HEIGHT = 1000;

    //NES
    cpu6502 cpu;
    ppu2C02 ppu;
    GamePak *gamePak = nullptr;
    Bus *bus = nullptr;

    //SDL
    void logError(std::ostream &os, const std::string &error);
    void throwError(const std::string &error);
    void createWindow();
    void createRenderer();

    template <typename T, typename... Args> void cleanup(T *, Args&&... args);
    void cleanup(SDL_Window *w);
    void cleanup(SDL_Renderer *r);

    //atributi
    SDL_Window *window;
    SDL_Renderer *renderer;

    //pomoćni atributi
    bool running = false;
public:
    Debugger(const std::string &test);
    ~Debugger();
    Debugger(const Debugger &debugger) = delete;
    Debugger(Debugger &&debugger) = delete;

    void run();
};


#endif //NES_DEBUGGER_H
