//
// Created by denis on 21/02/2020.
//

#include <iostream>
#include "Operation.h"

uint8_t Operation::ADC(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t result = uint16_t(cpu.accumulator) + uint16_t(cpu.memory_content) + uint16_t(cpu.getFlag(CPU::C));
    setZNC(cpu, result);
    cpu.setFlag(CPU::V, setV(cpu.accumulator, cpu.memory_content, result, cpu.accumulator));
    cpu.accumulator = result & 0x00FFu;
    return 1;
}

uint8_t Operation::AND(CPU &cpu) {
    logical(cpu, [](uint16_t x, uint16_t y) {return x & y;});
    return 1;
}

uint8_t Operation::ASL(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t shifted_value = (uint16_t(cpu.memory_content) << 1u);
    cpu.setFlag(CPU::C, setC(shifted_value));
    shift(cpu, shifted_value);
    return 0;
}

uint8_t Operation::BCC(CPU &cpu) {
    if(cpu.getFlag(CPU::C) == 0) branch(cpu);
    return 0;
}

uint8_t Operation::BCS(CPU &cpu) {
    if(cpu.getFlag(CPU::C) == 1) branch(cpu);
    return 0;
}

uint8_t Operation::BEQ(CPU &cpu) {
    if(cpu.getFlag(CPU::Z) == 1) branch(cpu);
    return 0;
}

uint8_t Operation::BIT(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t result = cpu.accumulator & cpu.memory_content;
    cpu.setFlag(CPU::V, (cpu.memory_content & 0x00FFu) & 0b01000000u);
    cpu.setFlag(CPU::N, (cpu.memory_content & 0x00FFu) & 0b10000000u);
    cpu.setFlag(CPU::Z, setZ(result));
    return 0;
}

uint8_t Operation::BMI(CPU &cpu) {
    if(cpu.getFlag(CPU::N) == 1) branch(cpu);
    return 0;
}

uint8_t Operation::BNE(CPU &cpu) {
    if(cpu.getFlag(CPU::Z) == 0) branch(cpu);
    return 0;
}

uint8_t Operation::BPL(CPU &cpu) {
    if(cpu.getFlag(CPU::N) == 0) branch(cpu);
    return 0;
}

uint8_t Operation::BRK(CPU &cpu) {
    cpu.program_counter++;
    cpu.status_register |= CPU::I | CPU::B;
    push2BToStack(cpu, cpu.program_counter);
    pushToStack(cpu, cpu.status_register);
    cpu.status_register &= ~CPU::B;
    cpu.program_counter = mergeBytes(cpu.read(BRK_VECTOR + 1), cpu.read(BRK_VECTOR));
    return 0;
}

uint8_t Operation::BVC(CPU &cpu) {
    if(cpu.getFlag(CPU::V) == 0) branch(cpu);
    return 0;
}

uint8_t Operation::BVS(CPU &cpu) {
    if(cpu.getFlag(CPU::V) == 1) branch(cpu);
    return 0;
}

uint8_t Operation::CLC(CPU &cpu) {
    cpu.setFlag(CPU::C, false);
    return 0;
}

uint8_t Operation::CLD(CPU &cpu) {
    cpu.setFlag(CPU::D, false);
    return 0;
}

uint8_t Operation::CLI(CPU &cpu) {
    cpu.setFlag(CPU::I, false);
    return 0;
}

uint8_t Operation::CLV(CPU &cpu) {
    cpu.setFlag(CPU::V, false);
    return 0;
}

uint8_t Operation::CMP(CPU &cpu) {
    compare(cpu, cpu.accumulator);
    return 1;
}

uint8_t Operation::CPX(CPU &cpu) {
    compare(cpu, cpu.x_register);
    return 0;
}

uint8_t Operation::CPY(CPU &cpu) {
    compare(cpu, cpu.y_register);
    return 0;
}

uint8_t Operation::DEC(CPU &cpu) {
    updateMem(cpu, -1);
    return 0;
}

uint8_t Operation::DEX(CPU &cpu) {
    updateXY([&cpu](){ cpu.x_register--;});
    setZN(cpu, cpu.x_register);
    return 0;
}

uint8_t Operation::DEY(CPU &cpu) {
    updateXY([&cpu](){ cpu.y_register--;});
    setZN(cpu, cpu.y_register);
    return 0;
}

uint8_t Operation::EOR(CPU &cpu) {
    logical(cpu, [](uint16_t x, uint16_t y) {return x ^ y;});
    return 1;
}

uint8_t Operation::INC(CPU &cpu) {
    updateMem(cpu, 1);
    return 0;
}

uint8_t Operation::INX(CPU &cpu) {
    updateXY([&cpu](){ cpu.x_register++;});
    setZN(cpu, cpu.x_register);
    return 0;
}

uint8_t Operation::INY(CPU &cpu) {
    updateXY([&cpu](){ cpu.y_register++;});
    setZN(cpu, cpu.y_register);
    return 0;
}

uint8_t Operation::JMP(CPU &cpu) {
    cpu.program_counter = cpu.absolute_address;
    return 0;
}

uint8_t Operation::JSR(CPU &cpu) {
    cpu.program_counter--;
    push2BToStack(cpu, cpu.program_counter);
    cpu.program_counter = cpu.absolute_address;
    return 0;
}

uint8_t Operation::LDA(CPU &cpu) {
    load(cpu, cpu.accumulator);
    return 1;
}

uint8_t Operation::LDX(CPU &cpu) {
    load(cpu, cpu.x_register);
    return 1;
}

uint8_t Operation::LDY(CPU &cpu) {
    load(cpu, cpu.y_register);
    return 1;
}

uint8_t Operation::LSR(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t shifted_value = cpu.memory_content >> 1u;
    cpu.setFlag(CPU::C, cpu.memory_content & 0x1u);
    shift(cpu, shifted_value);
    return 0;
}

uint8_t Operation::NOP(CPU &cpu) {
    return 0;
}

uint8_t Operation::ORA(CPU &cpu) {
    logical(cpu, [](uint16_t x, uint16_t y) {return x | y;});
    return 1;
}

uint8_t Operation::PHA(CPU &cpu) {
    pushToStack(cpu, cpu.accumulator);
    return 0;
}

uint8_t Operation::PHP(CPU &cpu) {
    cpu.status_register |= CPU::B | CPU::U;
    pushToStack(cpu, cpu.status_register);
    cpu.status_register &= ~CPU::B & ~CPU::U;
    return 0;
}

uint8_t Operation::PLA(CPU &cpu) {
    cpu.accumulator = pullFromStack(cpu);
    setZN(cpu, cpu.accumulator);
    return 0;
}

uint8_t Operation::PLP(CPU &cpu) {
    cpu.status_register = pullFromStack(cpu);
    cpu.status_register |= CPU::B;
    return 0;
}

uint8_t Operation::ROL(CPU &cpu) {
    cpu.getMemoryContent();
    auto result = (uint16_t(cpu.memory_content) << 1u) | cpu.getFlag(CPU::C);
    cpu.setFlag(CPU::C, setC(result));
    shift(cpu, result);
    return 0;
}

uint8_t Operation::ROR(CPU &cpu) {
    cpu.getMemoryContent();
    auto result = uint16_t((cpu.getFlag(CPU::C) << 7u) | (cpu.memory_content >> 1u));
    cpu.setFlag(CPU::C, cpu.memory_content & 0x1u);
    shift(cpu, result);
    return 0;
}

uint8_t Operation::RTI(CPU &cpu) {
    cpu.status_register = pullFromStack(cpu);
    cpu.status_register &= ~CPU::B & ~CPU::U;
    cpu.program_counter = pull2BFromStack(cpu);
    return 0;
}

uint8_t Operation::RTS(CPU &cpu) {
    cpu.program_counter = pull2BFromStack(cpu);
    cpu.program_counter++;
    return 0;
}

uint8_t Operation::SBC(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t complement = uint16_t(cpu.memory_content) ^ 0x00FFu;
    uint16_t result = uint16_t(cpu.accumulator) + complement + cpu.getFlag(CPU::C);
    setZNC(cpu, result);
    cpu.setFlag(CPU::V, setV(~result, cpu.accumulator, complement, result));
    cpu.accumulator = result & 0x00FFu;
    return 1;
}

uint8_t Operation::SEC(CPU &cpu) {
    cpu.setFlag(CPU::C, true);
    return 0;
}

uint8_t Operation::SED(CPU &cpu) {
    cpu.setFlag(CPU::D, true);
    return 0;
}

uint8_t Operation::SEI(CPU &cpu) {
    cpu.setFlag(CPU::I, true);
    return 0;
}

uint8_t Operation::STA(CPU &cpu) {
    cpu.write(cpu.absolute_address, cpu.accumulator);
    return 0;
}

uint8_t Operation::STX(CPU &cpu) {
    cpu.write(cpu.absolute_address, cpu.x_register);
    return 0;
}

uint8_t Operation::STY(CPU &cpu) {
    cpu.write(cpu.absolute_address, cpu.y_register);
    return 0;
}

uint8_t Operation::TAX(CPU &cpu) {
    transfer(cpu, cpu.x_register, cpu.accumulator);
    return 0;
}

uint8_t Operation::TAY(CPU &cpu) {
    transfer(cpu, cpu.y_register, cpu.accumulator);
    return 0;
}

uint8_t Operation::TSX(CPU &cpu) {
    transfer(cpu, cpu.x_register, cpu.stack_pointer);
    return 0;
}

uint8_t Operation::TXA(CPU &cpu) {
    transfer(cpu, cpu.accumulator, cpu.x_register);
    return 0;
}

uint8_t Operation::TXS(CPU &cpu) {
    cpu.stack_pointer = cpu.x_register;
    return 0;
}

uint8_t Operation::TYA(CPU &cpu) {
    transfer(cpu, cpu.accumulator, cpu.y_register);
    return 0;
}

uint8_t Operation::XXX(CPU &cpu) {
    throw std::logic_error("UNSUPPORTED OPERATION");
}

void Operation::branch(CPU &cpu) {
    cpu.cycles++;
    cpu.absolute_address = cpu.program_counter + cpu.relative_address;
    if(!samePage(cpu.absolute_address, cpu.program_counter)) cpu.cycles++;
    cpu.program_counter = cpu.absolute_address;
}

bool Operation::samePage(uint16_t oldAddress, uint16_t newAddress) {
    return (oldAddress & 0xFF00u) == (newAddress & 0xFF00u);
}

bool Operation::setZ(uint16_t data) {
    return (data & 0x00FFu) == 0x0000;
}

bool Operation::setN(uint16_t data) {
    return data & 0x0080u;
}

bool Operation::setC(uint16_t data) {
    return data & 0xFF00u;
}

bool Operation::setC(uint16_t data, uint16_t cmp) {
    return data >= cmp;
}

bool Operation::setV(uint16_t first, uint16_t second, uint16_t third, uint16_t fourth) {
    return (~first ^ second) & (third ^ fourth) & 0x80u;
}

void Operation::setZN(CPU &cpu, uint16_t data) {
    cpu.setFlag(CPU::Z, setZ(data));
    cpu.setFlag(CPU::N, setN(data));
}

void Operation::setZNC(CPU &cpu, uint16_t data) {
    setZN(cpu, data);
    cpu.setFlag(CPU::C, setC(data));
}

void Operation::setZNC(CPU &cpu, uint16_t data, uint16_t reg, uint16_t cmp) {
    setZN(cpu, data);
    cpu.setFlag(CPU::C, setC(reg, cmp));
}

void Operation::logical(CPU &cpu, std::function<uint16_t(uint16_t, uint16_t)> func) {
    cpu.getMemoryContent();
    cpu.accumulator = func(cpu.accumulator, cpu.memory_content);
    setZN(cpu, cpu.accumulator);
}

void Operation::compare(CPU &cpu, uint16_t reg) {
    cpu.getMemoryContent();
    uint16_t result = reg - uint16_t(cpu.memory_content);
    setZNC(cpu, result, reg, cpu.memory_content);
}

void Operation::updateXY(const std::function<void()> &func) {
    func();
}

void Operation::updateMem(CPU &cpu, const uint8_t &value) {
    cpu.getMemoryContent();
    uint16_t result = cpu.memory_content + value;
    cpu.write(cpu.absolute_address, result & 0x00FFu);
    setZN(cpu, result);
}

void Operation::load(CPU &cpu, uint8_t &reg) {
    cpu.getMemoryContent();
    reg = cpu.memory_content;
    setZN(cpu, reg);
}

void Operation::transfer(CPU &cpu, uint8_t &dest, uint8_t &source) {
    dest = source;
    setZN(cpu, dest);
}

void Operation::shift(CPU &cpu, uint16_t shifted_value) {
    setZN(cpu, shifted_value);
    if(cpu.instruction->addressing_mode == &AddressingMode::IMP) cpu.accumulator = shifted_value & 0x00FFu;
    else cpu.write(cpu.absolute_address, shifted_value & 0x00FFu);
}

void Operation::pushToStack(CPU &cpu, uint16_t data) {
    cpu.write(STACK_TOP + cpu.stack_pointer, data & 0x00FFu);
    cpu.stack_pointer--;
}

uint8_t Operation::pullFromStack(CPU &cpu) {
    cpu.stack_pointer++;
    return cpu.read(STACK_TOP + cpu.stack_pointer);
}

void Operation::push2BToStack(CPU &cpu, uint16_t data) {
    pushToStack(cpu, (data >> 8u));
    pushToStack(cpu, data);
}

uint16_t Operation::pull2BFromStack(CPU &cpu) {
    uint16_t low_byte = pullFromStack(cpu);
    uint16_t high_byte = pullFromStack(cpu);
    return mergeBytes(high_byte, low_byte);
}

uint16_t Operation::mergeBytes(uint16_t high, uint16_t low) {
    return (high << 8u) | low;
}


