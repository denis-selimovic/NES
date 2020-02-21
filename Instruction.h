//
// Created by denis on 21/02/2020.
//

#ifndef NES_INSTRUCTION_H
#define NES_INSTRUCTION_H

#include <string>

class Instruction {
public:
    std::string name;
    uint8_t total_cycles;
    uint8_t (*operation) () = nullptr;
    uint8_t (*addressing_mode) () = nullptr;
    Instruction(std::string name, uint8_t cycles, uint8_t (*opp) (), uint8_t (*addrmode) ());
};


#endif //NES_INSTRUCTION_H
