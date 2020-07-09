//
// Created by denis on 21/02/2020.
//

#include "AddressingMode.h"

uint8_t AddressingMode::IMM(CPU &cpu) {
    cpu.absolute_address = cpu.program_counter;
    cpu.program_counter++;
    return 0;
}

uint8_t AddressingMode::IMP(CPU &cpu) {
    cpu.memory_content = cpu.accumulator;
    return 0;
}

uint8_t AddressingMode::ZP0(CPU &cpu) {
    zeroPaged(cpu);
    return 0;
}

uint8_t AddressingMode::ZPX(CPU &cpu) {
    zeroPaged(cpu, cpu.x_register);
    return 0;
}

uint8_t AddressingMode::ZPY(CPU &cpu) {
    zeroPaged(cpu, cpu.y_register);
    return 0;
}

uint8_t AddressingMode::REL(CPU &cpu) {
    cpu.relative_address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    if(cpu.relative_address & 0x80) cpu.relative_address |= 0xFF00u;
    return 0;
}

uint8_t AddressingMode::ABS(CPU &cpu) {
    cpu.absolute_address = formAbsoluteAddress(cpu);
    return 0;
}

uint8_t AddressingMode::ABX(CPU &cpu) {
    uint16_t oldAddress = cpu.absolute_address;
    absolute(cpu, cpu.x_register);
    return samePage(oldAddress, cpu.absolute_address) ? 0 : 1;
}

uint8_t AddressingMode::ABY(CPU &cpu) {
    uint16_t oldAddress = cpu.absolute_address;
    absolute(cpu, cpu.y_register);
    return samePage(oldAddress, cpu.absolute_address) ? 0 : 1;
}

uint8_t AddressingMode::IND(CPU &cpu) {
    uint16_t indirect_address = formAbsoluteAddress(cpu);
    if((indirect_address & 0x00FFu) == 0x00FF) cpu.absolute_address = (cpu.read(indirect_address & 0xFF00u) << 8u) | cpu.read(indirect_address);
    else cpu.absolute_address = (cpu.read(indirect_address + 1u) << 8u) | cpu.read(indirect_address);
    return 0;
}

uint8_t AddressingMode::IZX(CPU &cpu) {
    // sadržaj x registr se koristi kao offset pročitanoj adresi
    uint16_t address = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    address += uint16_t(cpu.x_register);
    cpu.absolute_address = formIndexedAddress(cpu, address);
    return 0;
}

uint8_t AddressingMode::IZY(CPU &cpu) {
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

uint16_t AddressingMode::formZeroPagedAddress(uint16_t address) {
    return address & 0x00FFu;
}

uint16_t AddressingMode::formAbsoluteAddress(CPU &cpu) {
    uint8_t low_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    uint8_t high_nibble = cpu.read(cpu.program_counter);
    cpu.program_counter++;
    return formAddress(high_nibble, low_nibble);
}

uint16_t AddressingMode::formIndexedAddress(CPU &cpu, uint8_t address) {
    uint16_t low_byte = cpu.read(address & 0x00FFu);
    uint16_t high_byte = cpu.read((address + 1u) & 0x00FFu);
    return formAddress(high_byte, low_byte);
}

uint16_t AddressingMode::zeroPaged(CPU &cpu, const uint8_t &offset) {
    cpu.absolute_address = cpu.read(cpu.program_counter) + offset;
    cpu.program_counter++;
    cpu.absolute_address = formZeroPagedAddress(cpu.absolute_address);
    return 0;
}

uint16_t AddressingMode::absolute(CPU &cpu, const uint8_t &offset) {
    cpu.absolute_address = formAbsoluteAddress(cpu);
    cpu.absolute_address += offset;
}

bool AddressingMode::samePage(uint16_t oldAddress, uint16_t newAddress) {
    return (oldAddress & 0xFF00u) == (newAddress & 0xFF00u);
}
