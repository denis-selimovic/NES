//
// Created by denis on 02/03/2020.
//

#include <sstream>
#include "Disassembler.h"

std::string Disassembler::toHex(int number, uint8_t length) {
    std::stringstream hex;
    hex << std::hex << number;
    return hex.str();
}

void Disassembler::addInstruction(const uint16_t &address, const std::string &instruction) {
    disassembly[address] = instruction;
}

std::string Disassembler::getInstruction(const std::string &instruction, uint32_t operand) {
    return instruction + " 0x " + toHex(operand, 4);
}

std::string Disassembler::getInstruction(const uint16_t &address) {
    return disassembly[address];
}

