//
// Created by denis on 02/03/2020.
//

#ifndef NES_DISASSEMBLER_H
#define NES_DISASSEMBLER_H


#include <string>
#include "Instruction.h"

class Disassembler {

    std::string toHex(uint32_t number, uint8_t length);
    std::string getInstruction(std::string instruction, uint32_t operand);
public:
    Disassembler() = default;
    ~Disassembler() = default;
};


#endif //NES_DISASSEMBLER_H
