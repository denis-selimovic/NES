//
// Created by denis on 21/02/2020.
//

#ifndef NES_ADDRESSINGMODE_H
#define NES_ADDRESSINGMODE_H


#include "cpu6502.h"

class AddressingMode {
    //klasa koja prikazuje sve moguće načine adresiranja

public:
    uint8_t imp(cpu6502 *cpu);
    uint8_t imm(cpu6502 *cpu);
    uint8_t zp0(cpu6502 *cpu);
    uint8_t zpx(cpu6502 *cpu);
    uint8_t zpy(cpu6502 *cpu);
    uint8_t rel(cpu6502 *cpu);
    uint8_t abs(cpu6502 *cpu);
    uint8_t abx(cpu6502 *cpu);
    uint8_t aby(cpu6502 *cpu);
    uint8_t ind(cpu6502 *cpu);
    uint8_t izx(cpu6502 *cpu);
    uint8_t izy(cpu6502 *cpu);
};


#endif //NES_ADDRESSINGMODE_H
