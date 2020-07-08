//
// Created by denis on 21/02/2020.
//

#include "Instruction.h"

#include <utility>

Instruction::Instruction(std::string name, uint8_t (*opp)(CPU &cpu), uint8_t (*addrmode)(CPU &cpu), uint8_t cycles) {
    this->name = std::move(name);
    this->total_cycles = cycles;
    this->operation = opp;
    this->addressing_mode = addrmode;
}
