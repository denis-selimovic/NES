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


