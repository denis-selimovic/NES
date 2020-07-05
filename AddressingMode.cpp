//
// Created by denis on 21/02/2020.
//

#include "AddressingMode.h"

uint8_t AddressingMode::IMM(cpu6502 &cpu) {
    cpu.absolute_address = cpu.program_counter++;
    return 0;
}

uint8_t AddressingMode::IMP(cpu6502 &cpu) {
    cpu.memory_content = cpu.accumulator;
    return 0;
}

uint8_t AddressingMode::ZP0(cpu6502 &cpu) {
    cpu.absolute_address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_address &= 0x00FFu;
    return 0;
}

uint8_t AddressingMode::ZPX(cpu6502 &cpu) {
    cpu.absolute_address = cpu.read(cpu.program_counter) + cpu.x_register;
    cpu.program_counter++;
    cpu.absolute_address &= 0x00FFu;
    return 0;
}

uint8_t AddressingMode::ZPY(cpu6502 &cpu) {
    cpu.absolute_address = cpu.read(cpu.program_counter) + cpu.y_register;
    cpu.program_counter++;
    cpu.absolute_address &= 0x00FFu;
    return 0;
}

uint8_t AddressingMode::REL(cpu6502 &cpu) {
    cpu.relative_address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    if(cpu.relative_address & 0x80) cpu.relative_address |= 0xFF00u;
    return 0;
}

uint8_t AddressingMode::ABS(cpu6502 &cpu) {
    uint16_t low_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint16_t high_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_address = (high_nibble << 8u) | low_nibble;
    return 0;
}

uint8_t AddressingMode::ABX(cpu6502 &cpu) {
    uint8_t low_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint8_t high_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_address = (high_nibble << 8u) | low_nibble;
    cpu.absolute_address += cpu.x_register;
    return ((cpu.absolute_address & 0xFF00u) != (high_nibble << 8u)) ? 1 : 0;
}

uint8_t AddressingMode::ABY(cpu6502 &cpu) {
    uint8_t low_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint8_t high_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_address = (high_nibble << 8u) | low_nibble;
    cpu.absolute_address += cpu.y_register;
    return ((cpu.absolute_address & 0xFF00u) != (high_nibble << 8u)) ? 1 : 0;
}

uint8_t AddressingMode::IND(cpu6502 &cpu) {
    uint16_t low_byte = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint16_t high_byte = cpu.read(cpu.program_counter);
    cpu.program_counter++;

    uint16_t indirect_address = (high_byte << 8u) | low_byte;
    if(low_byte == 0x00FF) cpu.absolute_address = (cpu.read(indirect_address & 0xFF00u) << 8u) | cpu.read(indirect_address);
    else cpu.absolute_address = (cpu.read(indirect_address + 1u) << 8u) | cpu.read(indirect_address);
    return 0;
}

uint8_t AddressingMode::IZX(cpu6502 &cpu) {
    // sadržaj x registr se koristi kao offset pročitanoj adresi
    uint16_t address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    address += uint16_t(cpu.x_register);
    uint16_t low_byte = cpu.read(address & 0x00FFu);
    uint16_t high_byte = cpu.read((address + 1u) & 0x00FFu);
    cpu.absolute_address = (high_byte << 8u) | low_byte;
    return 0;
}

uint8_t AddressingMode::IZY(cpu6502 &cpu) {
    // čita se zero page adresa sa lokacije na koju pokazuje pc
    // na dobijenu absolutnu adresu doda se sadržaj y registra
    // ako se stranica promijeni dodatni ciklus
    uint16_t address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint16_t low_byte = cpu.read(address & 0x00FFu);
    uint16_t high_byte = cpu.read((address + 1u) & 0x00FFu);
    // dodamo y na apsolutnu adresu
    cpu.absolute_address = (high_byte << 8u) | low_byte;
    cpu.absolute_address += cpu.y_register;
    return ((cpu.absolute_address & 0xFF00u) != (high_byte << 8u)) ? 1 : 0;
}
