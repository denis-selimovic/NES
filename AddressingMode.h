//
// Created by denis on 21/02/2020.
//

#ifndef NES_ADDRESSINGMODE_H
#define NES_ADDRESSINGMODE_H


#include "CPU.h"

class CPU;

class AddressingMode {
    static uint16_t formAddress(uint8_t high_nibble, uint8_t low_nibble);
    static uint16_t formAbsoluteAddress(CPU &cpu);
    static uint16_t formIndexedAddress(CPU &cpu, uint8_t offset);
    static uint16_t formZeroPagedAddress(uint16_t address);

    static uint16_t zeroPaged(CPU &cpu, const uint8_t &offset = 0);
    static uint16_t absolute(CPU &cpu, const uint8_t &offset = 0);
    static bool samePage(uint16_t oldAddress, uint16_t newAddress);
public:
    static uint8_t IMP(CPU &cpu);
    static uint8_t IMM(CPU &cpu);
    static uint8_t ZP0(CPU &cpu);
    static uint8_t ZPX(CPU &cpu);
    static uint8_t ZPY(CPU &cpu);
    static uint8_t REL(CPU &cpu);
    static uint8_t ABS(CPU &cpu);
    static uint8_t ABX(CPU &cpu);
    static uint8_t ABY(CPU &cpu);
    static uint8_t IND(CPU &cpu);
    static uint8_t IZX(CPU &cpu);
    static uint8_t IZY(CPU &cpu);
};


#endif //NES_ADDRESSINGMODE_H
