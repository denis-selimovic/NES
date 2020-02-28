//
// Created by denis on 28/02/2020.
//

#include "Debugger.h"

Debugger::Debugger(const std::string &test) {
    gamePak = new GamePak(test);
    bus = new Bus(cpu, ppu);
    bus->connectGamepak(gamePak);
}

Debugger::~Debugger() {
    delete bus;
    delete gamePak;
}

void Debugger::logError(std::ostream &os, const std::string &error) {
    os << error << " Error:" << SDL_GetError() <<std::endl;
    throw std::logic_error(error);
}

void Debugger::createWindow() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) logError(std::cout, "SDL_INIT");
    window = SDL_CreateWindow("NES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr) logError(std::cout, "SDL_CreateWindow");
}


