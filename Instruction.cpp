//
// Created by denis on 21/02/2020.
//

#include "Instruction.h"

Instruction::Instruction(std::string name, uint8_t cycles = 0, uint8_t (*opp)() = nullptr, uint8_t (*addrmode)() = nullptr) {
    this->name = name;
    this->total_cycles = cycles;
    this->operation = opp;
    this->addressing_mode = addrmode;
}
