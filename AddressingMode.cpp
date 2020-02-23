//
// Created by denis on 21/02/2020.
//

#include "AddressingMode.h"

uint8_t AddressingMode::IMM(cpu6502 &cpu) {
    cpu.absolute_adress = cpu.program_counter++;
    return 0;
}

uint8_t AddressingMode::IMP(cpu6502 &cpu) {
    cpu.memory_content = cpu.accumulator;
    return 0;
}

uint8_t AddressingMode::ZP0(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::ZPX(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::ZPY(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::REL(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::ABS(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::ABX(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::ABY(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::IND(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::IZX(cpu6502 &cpu) {
    return 0;
}

uint8_t AddressingMode::IZY(cpu6502 &cpu) {
    return 0;
}
