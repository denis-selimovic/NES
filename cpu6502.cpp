//
// Created by denis on 21/02/2020.
//

#include "cpu6502.h"

void cpu6502::setAddressingMode(AddressingMode *mode) {
    this->addr_mode = mode;
}

void cpu6502::setOperation(Operation *operation) {
    this->operation = operation;
}

void cpu6502::setFlag(cpu6502::FLAGS flag, bool value) {
    if(value) status_register |= flag;
    else status_register &= ~flag;
}

uint8_t cpu6502::getFlag(cpu6502::FLAGS flag) {
    return ((status_register & flag) > 0) ? 1 : 0;
}

void cpu6502::connectToBus(Bus *bus) {
    this->bus = bus;
}

void cpu6502::write(uint16_t address, uint8_t data) {
    bus->writeMemory(address, data);
}

uint8_t cpu6502::read(uint16_t address) {
    memory_content = bus->readMemory(address);
    return memory_content;
}

cpu6502::cpu6502() {
    lookup = {
            {Instruction("BRK", 7, &Operation::BRK, &AddressingMode::IMP), Instruction("ORA", 6, &Operation::ORA, &AddressingMode::IZX), Instruction("XXX", 0, nullptr, nullptr), Instruction("XXX", 0, nullptr, nullptr), Instruction("XXX", 0, nullptr, nullptr), Instruction("ORA", 3, &Operation::ORA, &AddressingMode::ZP0), Instruction("ASL", 5, &Operation::ASL, &AddressingMode::ZP0), Instruction("XXX", 0, nullptr, nullptr),
             Instruction("PHP", 3, &Operation::PHP, &AddressingMode::IMP), Instruction("ORA", 2, &Operation::ORA, &AddressingMode::IMP), Instruction("ASL", 2, &Operation::ASL, &AddressingMode::IMP), Instruction("XXX", 0, nullptr, nullptr), Instruction("XXX", 0, nullptr, nullptr), Instruction("ORA", 4, &Operation::ORA, &AddressingMode::ABS), Instruction("ASL", 6, &Operation::ASL, &AddressingMode::ABS), Instruction("XXX", 0, nullptr, nullptr)},
            {Instruction("BPL", 2, &Operation::BPL, &AddressingMode::REL), Instruction("ORA", 5, &Operation::ORA, &AddressingMode::IZY), Instruction("XXX", 0, nullptr, nullptr), Instruction("XXX", 0, nullptr, nullptr), Instruction("XXX", 0, nullptr, nullptr), Instruction("ORA", 4, &Operation::ORA, &AddressingMode::ZPX), Instruction("ASL", 6, &Operation::ASL, &AddressingMode::ZPX), Instruction("XXX", 0, nullptr, nullptr),
             Instruction("CLC",23, &Operation::CLC, &AddressingMode::IMP), Instruction("ORA", 4, &Operation::ORA, &AddressingMode::ABY), Instruction("XXX", 0, nullptr, nullptr), Instruction("XXX", 0, nullptr, nullptr), Instruction("XXX", 0, nullptr, nullptr), Instruction("ORA", 4, &Operation::ORA, &AddressingMode::ABX), Instruction("ASL", 7, &Operation::ASL, &AddressingMode::ABX), Instruction("XXX", 0, nullptr, nullptr)},
            {Instruction("JSR", 6, &Operation::JSR, &AddressingMode::ABS), Instruction("AND", 6, &Operation::AND, &AddressingMode::IZX), Instruction("XXX", 0, nullptr, nullptr), Instruction("XXX", 0, nullptr, nullptr), Instruction("BIT", 3, &Operation::BIT, &AddressingMode::ZP0), Instruction("AND", 3, &Operation::AND, &AddressingMode::ZP0), Instruction("ROL", 5, &Operation::ROL, &AddressingMode::ZP0), Instruction("XXX", 0, nullptr, nullptr),
             Instruction("PLP", 4, &Operation::PLP, &AddressingMode::IMP), Instruction("AND", 2, &Operation::AND, &AddressingMode::IMM), Instruction("ROL", 2, &Operation::ROL, &AddressingMode::IMP), Instruction("XXX", 0, nullptr, nullptr), Instruction("BIT", 4, &Operation::BIT, &AddressingMode::ABS), Instruction("AND", 4, &Operation::AND, &AddressingMode::ABS), Instruction("ROL", 6, &Operation::ROL, &AddressingMode::ABS), Instruction("XXX", 0, nullptr, nullptr)},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {}
    };
}


