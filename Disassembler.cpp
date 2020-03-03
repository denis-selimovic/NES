//
// Created by denis on 02/03/2020.
//

#include "Disassembler.h"

std::string Disassembler::toHex(uint32_t number, uint8_t length) {
    std::string hex(length, '0');
    for(int i = length - 1; i >= 0; --i) {
        hex[i] = "0123456789ABCDEF"[number & 0xFu];
        number >>= 4u;
    }
    return hex;
}

void Disassembler::addInstruction(const uint16_t &address, const std::string &instruction) {
    disassembly[address] = instruction;
}

std::string Disassembler::getInstruction(const std::string &instruction, uint32_t operand) {
    return instruction + " 0x " + toHex(operand, 4);
}

