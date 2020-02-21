//
// Created by denis on 21/02/2020.
//

#include "Instruction.h"

Instruction::Instruction(std::string name, uint8_t cycles, uint8_t (*opp)(cpu6502 *cpu), uint8_t (*addrmode)(cpu6502 *cpu)) {
    this->name = name;
    this->total_cycles = cycles;
    this->operation = opp;
    this->addressing_mode = addrmode;
}
