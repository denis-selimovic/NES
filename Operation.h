//
// Created by denis on 21/02/2020.
//

#ifndef NES_OPERATION_H
#define NES_OPERATION_H


#include <cstdint>
#include <functional>
#include "CPU.h"

class Operation {
    const static uint16_t STACK_TOP = 0x0100;

    static void branch(CPU &cpu);
    static void setZN(CPU &cpu, uint16_t data);
    static void setZNC(CPU &cpu, uint16_t data);
    static void setZNC(CPU &cpu, uint16_t data, uint16_t reg, uint16_t cmp);

    static void logical(CPU &cpu, std::function<uint16_t(uint16_t, uint16_t)> func);
    static void compare(CPU &cpu, uint16_t reg);
    static void updateXY(const std::function<void()> &func);
    static void updateMem(CPU &cpu, const uint8_t &value);
    static void load(CPU &cpu, uint8_t &reg);
    static void transfer(CPU &cpu, uint8_t &dest, uint8_t &source);

    static void pushToStack(CPU &cpu, uint16_t data);
    static uint8_t pullFromStack(CPU &cpu);
    static void push2BToStack(CPU &cpu, uint16_t data);
    static uint16_t pull2BFromStack(CPU &cpu);
    static uint16_t mergeBytes(uint16_t high, uint16_t low);

    static bool setZ(uint16_t data);
    static bool setN(uint16_t data);
    static bool setC(uint16_t data);
    static bool setC(uint16_t data, uint16_t cmp);
    static bool setV(uint16_t first, uint16_t second, uint16_t third, uint16_t fourth);
    static bool samePage(uint16_t oldAddress, uint16_t newAddress);
public:
    static uint8_t ADC(CPU &cpu);
    static uint8_t AND(CPU &cpu);
    static uint8_t ASL(CPU &cpu);
    static uint8_t BCC(CPU &cpu);
    static uint8_t BCS(CPU &cpu);
    static uint8_t BEQ(CPU &cpu);
    static uint8_t BIT(CPU &cpu);
    static uint8_t BMI(CPU &cpu);
    static uint8_t BNE(CPU &cpu);
    static uint8_t BPL(CPU &cpu);
    static uint8_t BRK(CPU &cpu);
    static uint8_t BVC(CPU &cpu);
    static uint8_t BVS(CPU &cpu);
    static uint8_t CLC(CPU &cpu);
    static uint8_t CLD(CPU &cpu);
    static uint8_t CLI(CPU &cpu);
    static uint8_t CLV(CPU &cpu);
    static uint8_t CMP(CPU &cpu);
    static uint8_t CPX(CPU &cpu);
    static uint8_t CPY(CPU &cpu);
    static uint8_t DEC(CPU &cpu);
    static uint8_t DEX(CPU &cpu);
    static uint8_t DEY(CPU &cpu);
    static uint8_t EOR(CPU &cpu);
    static uint8_t INC(CPU &cpu);
    static uint8_t INX(CPU &cpu);
    static uint8_t INY(CPU &cpu);
    static uint8_t JMP(CPU &cpu);
    static uint8_t JSR(CPU &cpu);
    static uint8_t LDA(CPU &cpu);
    static uint8_t LDX(CPU &cpu);
    static uint8_t LDY(CPU &cpu);
    static uint8_t LSR(CPU &cpu);
    static uint8_t NOP(CPU &cpu);
    static uint8_t ORA(CPU &cpu);
    static uint8_t PHA(CPU &cpu);
    static uint8_t PHP(CPU &cpu);
    static uint8_t PLA(CPU &cpu);
    static uint8_t PLP(CPU &cpu);
    static uint8_t ROL(CPU &cpu);
    static uint8_t ROR(CPU &cpu);
    static uint8_t RTI(CPU &cpu);
    static uint8_t RTS(CPU &cpu);
    static uint8_t SBC(CPU &cpu);
    static uint8_t SEC(CPU &cpu);
    static uint8_t SED(CPU &cpu);
    static uint8_t SEI(CPU &cpu);
    static uint8_t STA(CPU &cpu);
    static uint8_t STX(CPU &cpu);
    static uint8_t STY(CPU &cpu);
    static uint8_t TAX(CPU &cpu);
    static uint8_t TAY(CPU &cpu);
    static uint8_t TSX(CPU &cpu);
    static uint8_t TXA(CPU &cpu);
    static uint8_t TXS(CPU &cpu);
    static uint8_t TYA(CPU &cpu);
    static uint8_t XXX(CPU &cpu);
};


#endif //NES_OPERATION_H
