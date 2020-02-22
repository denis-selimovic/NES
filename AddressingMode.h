//
// Created by denis on 21/02/2020.
//

#ifndef NES_ADDRESSINGMODE_H
#define NES_ADDRESSINGMODE_H


#include "cpu6502.h"

class cpu6502;

class AddressingMode {
    //klasa koja prikazuje sve moguće načine adresiranja

public:
    static uint8_t IMP(cpu6502 *cpu);
    static uint8_t IMM(cpu6502 *cpu);
    static uint8_t ZP0(cpu6502 *cpu);
    static uint8_t ZPX(cpu6502 *cpu);
    static uint8_t ZPY(cpu6502 *cpu);
    static uint8_t REL(cpu6502 *cpu);
    static uint8_t ABS(cpu6502 *cpu);
    static uint8_t ABX(cpu6502 *cpu);
    static uint8_t ABY(cpu6502 *cpu);
    static uint8_t IND(cpu6502 *cpu);
    static uint8_t IZX(cpu6502 *cpu);
    static uint8_t IZY(cpu6502 *cpu);
};


#endif //NES_ADDRESSINGMODE_H
