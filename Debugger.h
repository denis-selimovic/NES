//
// Created by denis on 28/02/2020.
//

#ifndef NES_DEBUGGER_H
#define NES_DEBUGGER_H


#include <string>
#include "Bus.h"

class Debugger {

    cpu6502 cpu;
    ppu2C02 ppu;
    GamePak *gamePak;
    Bus * bus;
public:
    Debugger(const std::string &test);
    ~Debugger();
    Debugger(const Debugger &debugger) = delete;
    Debugger(Debugger &&debugger) = delete;
};


#endif //NES_DEBUGGER_H
