//
// Created by denis on 21/02/2020.
//

#include "AddressingMode.h"

uint8_t AddressingMode::IMM(cpu6502 &cpu) {
    cpu.absolute_address = cpu.program_counter;
    cpu.program_counter++;
    return 0;
}

uint8_t AddressingMode::IMP(cpu6502 &cpu) {
    cpu.memory_content = cpu.accumulator;
    return 0;
}

uint8_t AddressingMode::ZP0(cpu6502 &cpu) {
    cpu.absolute_address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_address = zeroPaged(cpu.absolute_address);
    return 0;
}

uint8_t AddressingMode::ZPX(cpu6502 &cpu) {
    cpu.absolute_address = cpu.read(cpu.program_counter) + cpu.x_register;
    cpu.program_counter++;
    cpu.absolute_address = zeroPaged(cpu.absolute_address);
    return 0;
}

uint8_t AddressingMode::ZPY(cpu6502 &cpu) {
    cpu.absolute_address = cpu.read(cpu.program_counter) + cpu.y_register;
    cpu.program_counter++;
    cpu.absolute_address = zeroPaged(cpu.absolute_address);
    return 0;
}

uint8_t AddressingMode::REL(cpu6502 &cpu) {
    cpu.relative_address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    if(cpu.relative_address & 0x80) cpu.relative_address |= 0xFF00u;
    return 0;
}

uint8_t AddressingMode::ABS(cpu6502 &cpu) {
    cpu.absolute_address = formAbsoluteAddress(cpu);
    return 0;
}

uint8_t AddressingMode::ABX(cpu6502 &cpu) {
    uint16_t oldAddress = cpu.absolute_address;
    cpu.absolute_address = formAbsoluteAddress(cpu);
    cpu.absolute_address += cpu.x_register;
    return samePage(oldAddress, cpu.absolute_address) ? 0 : 1;
}

uint8_t AddressingMode::ABY(cpu6502 &cpu) {
    uint16_t oldAddress = cpu.absolute_address;
    cpu.absolute_address = formAbsoluteAddress(cpu);
    cpu.absolute_address += cpu.y_register;
    return samePage(oldAddress, cpu.absolute_address) ? 0 : 1;
}

uint8_t AddressingMode::IND(cpu6502 &cpu) {
    uint16_t indirect_address = formAbsoluteAddress(cpu);
    if((indirect_address & 0x00FFu) == 0x00FF) cpu.absolute_address = (cpu.read(indirect_address & 0xFF00u) << 8u) | cpu.read(indirect_address);
    else cpu.absolute_address = (cpu.read(indirect_address + 1u) << 8u) | cpu.read(indirect_address);
    return 0;
}

uint8_t AddressingMode::IZX(cpu6502 &cpu) {
    // sadržaj x registr se koristi kao offset pročitanoj adresi
    uint16_t address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    address += uint16_t(cpu.x_register);
    cpu.absolute_address = formIndexedAddress(cpu, address);
    return 0;
}

uint8_t AddressingMode::IZY(cpu6502 &cpu) {
    // čita se zero page adresa sa lokacije na koju pokazuje pc
    // na dobijenu absolutnu adresu doda se sadržaj y registra
    // ako se stranica promijeni dodatni ciklus
    uint16_t oldAddress = cpu.absolute_address;
    uint16_t address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    cpu.absolute_address = formIndexedAddress(cpu, address);
    cpu.absolute_address += cpu.y_register;
    return samePage(oldAddress, cpu.absolute_address) ? 0 : 1;
}

uint16_t AddressingMode::formAddress(uint8_t high_nibble, uint8_t low_nibble) {
    return (high_nibble << 8u) | low_nibble;
}

uint16_t AddressingMode::zeroPaged(uint16_t address) {
    return address & 0x00FFu;
}

uint16_t AddressingMode::formAbsoluteAddress(cpu6502 &cpu) {
    uint8_t low_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint8_t high_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    return formAddress(high_nibble, low_nibble);
}

uint16_t AddressingMode::formIndexedAddress(cpu6502 &cpu, uint8_t address) {
    uint16_t low_byte = cpu.read(address & 0x00FFu);
    uint16_t high_byte = cpu.read((address + 1u) & 0x00FFu);
    return formAddress(high_byte, low_byte);
}

bool AddressingMode::samePage(uint16_t oldAddress, uint16_t newAddress) {
    return (oldAddress & 0xFF00u) == (newAddress & 0xFF00u);
}
