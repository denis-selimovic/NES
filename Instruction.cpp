//
// Created by denis on 21/02/2020.
//

#include "Instruction.h"

#include <utility>

Instruction::Instruction(std::string name, uint8_t (*opp)(cpu6502 &cpu), uint8_t (*addrmode)(cpu6502 &cpu), uint8_t cycles) {
    this->name = std::move(name);
    this->total_cycles = cycles;
    this->operation = opp;
    this->addressing_mode = addrmode;
}

Instruction::Instruction() {
    this->name = "XXX";
    this->total_cycles = 0;
    this->operation = nullptr;
    this->addressing_mode = nullptr;
}
