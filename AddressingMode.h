//
// Created by denis on 21/02/2020.
//

#ifndef NES_ADDRESSINGMODE_H
#define NES_ADDRESSINGMODE_H


#include "cpu6502.h"

class AddressingMode {
    //klasa koja prikazuje sve moguće načine adresiranja
    cpu6502 *cpu;

public:
    AddressingMode(cpu6502 *cpu = nullptr);
    uint8_t imp();
    uint8_t imm();
    uint8_t zp0();
    uint8_t zpx();
    uint8_t zpy();
    uint8_t rel();
    uint8_t abs();
    uint8_t abx();
    uint8_t aby();
    uint8_t ind();
    uint8_t izx();
    uint8_t izy();
};


#endif //NES_ADDRESSINGMODE_H
