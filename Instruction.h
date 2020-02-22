//
// Created by denis on 21/02/2020.
//

#ifndef NES_INSTRUCTION_H
#define NES_INSTRUCTION_H

#include <string>
#include "cpu6502.h"

class cpu6502;

class Instruction {
public:
    std::string name;
    uint8_t total_cycles;
    uint8_t (*operation) (cpu6502 &cpu);
    uint8_t (*addressing_mode) (cpu6502 &cpu);
    Instruction(std::string name, uint8_t (*opp) (cpu6502 &cpu) = nullptr, uint8_t (*addrmode) (cpu6502 &cpu) = nullptr, uint8_t cycles = 0);
    Instruction();
};


#endif //NES_INSTRUCTION_H
