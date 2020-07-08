//
// Created by denis on 21/02/2020.
//

#ifndef NES_INSTRUCTION_H
#define NES_INSTRUCTION_H

#include <string>
#include "CPU.h"

class CPU;

class Instruction {
public:
    std::string name;
    uint8_t total_cycles;
    uint8_t (*operation) (CPU &cpu);
    uint8_t (*addressing_mode) (CPU &cpu);
    Instruction(std::string name = "XXX", uint8_t (*opp) (CPU &cpu) = nullptr, uint8_t (*addrmode) (CPU &cpu) = nullptr, uint8_t cycles = 0);
};


#endif //NES_INSTRUCTION_H
