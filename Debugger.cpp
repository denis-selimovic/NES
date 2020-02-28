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


