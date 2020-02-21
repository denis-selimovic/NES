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

}

uint8_t cpu6502::getFlag(cpu6502::FLAGS flag) {
    return 0;
}


