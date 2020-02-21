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
    uint8_t (*operation) ();
    uint8_t (*addressing_mode) ();
    Instruction(std::string name, uint8_t cycles = 0, uint8_t (*opp) () = nullptr, uint8_t (*addrmode) () = nullptr);
};


#endif //NES_INSTRUCTION_H
