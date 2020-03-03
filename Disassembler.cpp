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

std::string Disassembler::getInstruction(std::string instruction, uint32_t operand) {
    return instruction + " $ " + toHex(operand, 4);
}

