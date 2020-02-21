//
// Created by denis on 21/02/2020.
//

#include "Instruction.h"

Instruction::Instruction(std::string name, uint8_t cycles, uint8_t (*opp)(), uint8_t (*addrmode)()) {
    this->name = name;
    this->total_cycles = cycles;
    this->operation = opp;
    this->addressing_mode = addrmode;
}
