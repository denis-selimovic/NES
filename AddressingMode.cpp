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
    cpu.absolute_adress = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_adress &= 0x00FF;
    return 0;
}

uint8_t AddressingMode::ZPX(cpu6502 &cpu) {
    cpu.absolute_adress = cpu.read(cpu.program_counter) + cpu.x_register;
    cpu.program_counter++;
    cpu.absolute_adress &= 0x00FF;
    return 0;
}

uint8_t AddressingMode::ZPY(cpu6502 &cpu) {
    cpu.absolute_adress = cpu.read(cpu.program_counter) + cpu.y_register;
    cpu.program_counter++;
    cpu.absolute_adress &= 0x00FF;
    return 0;
}

uint8_t AddressingMode::REL(cpu6502 &cpu) {
    cpu.relative_address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    if(cpu.relative_address & 0x80) cpu.relative_address |= 0xFF00;
    return 0;
}

uint8_t AddressingMode::ABS(cpu6502 &cpu) {
    uint8_t low_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint8_t high_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_adress = high_nibble << 8 | low_nibble;
    return 0;
}

uint8_t AddressingMode::ABX(cpu6502 &cpu) {
    uint8_t low_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint8_t high_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_adress = high_nibble << 8 | low_nibble;
    cpu.absolute_adress += cpu.x_register;
    return ((cpu.absolute_adress & 0xFF00) != (high_nibble << 8)) ? 1 : 0;
}

uint8_t AddressingMode::ABY(cpu6502 &cpu) {
    uint8_t low_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint8_t high_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_adress = high_nibble << 8 | low_nibble;
    cpu.absolute_adress += cpu.y_register;
    return ((cpu.absolute_adress & 0xFF00) != (high_nibble << 8)) ? 1 : 0;
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
