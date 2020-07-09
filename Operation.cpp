//
// Created by denis on 21/02/2020.
//

#include <iostream>
#include "Operation.h"

uint8_t Operation::ADC(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t result = uint16_t(cpu.accumulator) + uint16_t(cpu.memory_content) + uint16_t(cpu.getFlag(CPU::C));
    setZNC(cpu, result);
    cpu.setFlag(CPU::V, setAddV(cpu.accumulator, cpu.memory_content, result));
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
    cpu.setFlag(CPU::C, (shifted_value & 0xFF00u) > 0);
    cpu.setFlag(CPU::Z, (shifted_value & 0x00FFu) == 0x0000);
    cpu.setFlag(CPU::N, shifted_value & 0x80u);
    if(cpu.instruction->addressing_mode == &AddressingMode::IMP) cpu.accumulator = shifted_value & 0x00FFu;
    else cpu.write(cpu.absolute_address, shifted_value & 0x00FFu);
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
    cpu.setFlag(CPU::Z, (result & 0x00FFu) == 0x0000);
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
    // BRK ima dvobajtni opcode
    // zbog se sljedeća vrijednost pc-a ignoriše nakon što je pročitan prvi bajt opcode-a za BRK -> inkrementirano pc
    cpu.program_counter++;
    //postavimo I flag na 1
    cpu.setFlag(CPU::I, true);
    //programski brojač stavimo na stack
    //na stack prvo ide prvih 8 bita pc-a (najznačajnijih)
    cpu.write(0x0100 + cpu.stack_pointer, (cpu.program_counter  >> 8u) & 0x00FFu);
    cpu.stack_pointer--;
    //zatim na stack ide 8 najmanje značajnih bita
    cpu.write(0x0100 + cpu.stack_pointer, cpu.program_counter & 0x00FFu);
    cpu.stack_pointer--;

    //nakon ovog na stack se stavlja sadržaj statusnog registra sa B flagom postavljenim na 1
    //B flag se nakon toga vraća na 0
    cpu.setFlag(CPU::B, true);
    cpu.write(0x0100 + cpu.stack_pointer, cpu.status_register);
    cpu.stack_pointer--;
    cpu.setFlag(CPU::B, false);

    //adresa interrupt rutine se dobija sa lokacija 0xFFFF (high byte) i 0xFFFE (low byte)
    cpu.program_counter = (uint16_t(cpu.read(0xFFFE))) | (uint16_t(cpu.read(0xFFFF)) << 8u);
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
    cpu.getMemoryContent();
    uint16_t result = cpu.memory_content - 1;
    cpu.write(cpu.absolute_address, result & 0x00FFu);
    cpu.setFlag(CPU::N, result & 0x0080u);
    cpu.setFlag(CPU::Z, (result & 0x00FFu) == 0x0000);
    return 0;
}

uint8_t Operation::DEX(CPU &cpu) {
    cpu.x_register--;
    cpu.setFlag(CPU::N, cpu.x_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.x_register == 0x00);
    return 0;
}

uint8_t Operation::DEY(CPU &cpu) {
    cpu.y_register--;
    cpu.setFlag(CPU::N, cpu.y_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.y_register == 0x00);
    return 0;
}

uint8_t Operation::EOR(CPU &cpu) {
    logical(cpu, [](uint16_t x, uint16_t y) {return x ^ y;});
    return 1;
}

uint8_t Operation::INC(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t result = cpu.memory_content + 1;
    cpu.write(cpu.absolute_address, result & 0x00FFu);
    cpu.setFlag(CPU::N, result & 0x0080u);
    cpu.setFlag(CPU::Z, (result & 0x00FFu) == 0x0000);
    return 0;
}

uint8_t Operation::INX(CPU &cpu) {
    cpu.x_register++;
    cpu.setFlag(CPU::N, cpu.x_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.x_register == 0x00);
    return 0;
}

uint8_t Operation::INY(CPU &cpu) {
    cpu.y_register++;
    cpu.setFlag(CPU::N, cpu.y_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.y_register == 0x00);
    return 0;
}

uint8_t Operation::JMP(CPU &cpu) {
    cpu.program_counter = cpu.absolute_address;
    return 0;
}

uint8_t Operation::JSR(CPU &cpu) {
    //cuvamo mjesto povratka tj pc--
    cpu.program_counter--;

    //stavimo pc na stek
    cpu.write(0x0100 + cpu.stack_pointer, (cpu.program_counter >> 8u) & 0x00FFu);
    cpu.stack_pointer--;
    cpu.write(0x0100 + cpu.stack_pointer, cpu.program_counter & 0x00FFu);
    cpu.stack_pointer--;

    //mijenjamo pc
    cpu.program_counter = cpu.absolute_address;
    return 0;
}

uint8_t Operation::LDA(CPU &cpu) {
    cpu.getMemoryContent();
    cpu.accumulator = cpu.memory_content;
    cpu.setFlag(CPU::N, cpu.accumulator & 0x80u);
    cpu.setFlag(CPU::Z, cpu.accumulator == 0x00);
    return 1;
}

uint8_t Operation::LDX(CPU &cpu) {
    cpu.getMemoryContent();
    cpu.x_register = cpu.memory_content;
    cpu.setFlag(CPU::N, cpu.x_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.x_register == 0x00);
    return 1;
}

uint8_t Operation::LDY(CPU &cpu) {
    cpu.getMemoryContent();
    cpu.y_register = cpu.memory_content;
    cpu.setFlag(CPU::N, cpu.y_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.y_register == 0x00);
    return 1;
}

uint8_t Operation::LSR(CPU &cpu) {
    cpu.getMemoryContent();
    cpu.setFlag(CPU::C, cpu.memory_content & 0x0001u);
    uint16_t result = cpu.memory_content >> 1u;
    cpu.setFlag(CPU::N, result & 0x80u);
    cpu.setFlag(CPU::Z, (result & 0x00FFu) == 0x0000);
    if(cpu.instruction->addressing_mode == &AddressingMode::IMP) cpu.accumulator = result & 0x00FFu;
    else cpu.write(cpu.absolute_address, result & 0x00FFu);
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
    cpu.write(0x0100 + cpu.stack_pointer, cpu.accumulator);
    cpu.stack_pointer--;
    return 0;
}

uint8_t Operation::PHP(CPU &cpu) {
    cpu.setFlag(CPU::B, true);
    cpu.setFlag(CPU::U, true);
    cpu.write(0x0100 + cpu.stack_pointer, cpu.status_register);
    cpu.stack_pointer--;
    cpu.setFlag(CPU::B, false);
    cpu.setFlag(CPU::U, false);
    return 0;
}

uint8_t Operation::PLA(CPU &cpu) {
    cpu.stack_pointer++;
    cpu.accumulator = cpu.read(0x0100 + cpu.stack_pointer);
    cpu.setFlag(CPU::N, cpu.accumulator & 0x80u);
    cpu.setFlag(CPU::Z, cpu.accumulator == 0x00);
    return 0;
}

uint8_t Operation::PLP(CPU &cpu) {
    cpu.stack_pointer++;
    cpu.status_register = cpu.read(0x0100 + cpu.stack_pointer);
    cpu.setFlag(CPU::U, true);
    return 0;
}

uint8_t Operation::ROL(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t result = (uint16_t(cpu.memory_content) << 1u) | cpu.getFlag(CPU::C);
    cpu.setFlag(CPU::C, result & 0xFF00u);
    cpu.setFlag(CPU::N, result & 0x0080u);
    cpu.setFlag(CPU::Z, (result & 0x00FFu) == 0x0000);
    if(cpu.instruction->addressing_mode == &AddressingMode::IMP) cpu.accumulator = result & 0x00FFu;
    else cpu.write(cpu.absolute_address, result & 0x00FFu);
    return 0;
}

uint8_t Operation::ROR(CPU &cpu) {
    cpu.getMemoryContent();
    auto result = uint16_t((cpu.getFlag(CPU::C) << 7u) | (cpu.memory_content >> 1u));
    cpu.setFlag(CPU::C, cpu.memory_content & 0x01u);
    cpu.setFlag(CPU::N, result & 0x0080u);
    cpu.setFlag(CPU::Z, (result & 0x00FFu) == 0x0000);
    if(cpu.instruction->addressing_mode == &AddressingMode::IMP) cpu.accumulator = result & 0x00FFu;
    else cpu.write(cpu.absolute_address, result & 0x00FFu);
    return 0;
}

uint8_t Operation::RTI(CPU &cpu) {
    //čitamo statusni registar sa stacka
    cpu.stack_pointer++;
    cpu.status_register = cpu.read(0x0100 + cpu.stack_pointer);
    cpu.status_register &= ~CPU::B;
    cpu.status_register &= ~CPU::U;

    //uzimamo 8 najmanje značajnih bita pc-a
    cpu.stack_pointer++;
    cpu.program_counter = uint16_t(cpu.read(0x0100 + cpu.stack_pointer));

    //uzimamo high byte programskog brojača
    cpu.stack_pointer++;
    cpu.program_counter |= ((uint16_t(cpu.read(0x0100 + cpu.stack_pointer))) << 8u);
    return 0;
}

uint8_t Operation::RTS(CPU &cpu) {
    //uzimamo low byte programskog brojača
    cpu.stack_pointer++;
    cpu.program_counter = uint16_t(cpu.read(0x0100 + cpu.stack_pointer));

    //uzimamo high byte programskog brojača
    cpu.stack_pointer++;
    cpu.program_counter |= (uint16_t(cpu.read(0x0100 + cpu.stack_pointer))) << 8u;

    //ažuriramo pc
    cpu.program_counter++;
    return 0;
}

uint8_t Operation::SBC(CPU &cpu) {
    cpu.getMemoryContent();
    uint16_t complement = uint16_t(cpu.memory_content) ^ 0x00FFu;
    uint16_t result = uint16_t(cpu.accumulator) + complement + cpu.getFlag(CPU::C);
    cpu.setFlag(CPU::Z, (result & 0x00FFu) == 0x0000);
    cpu.setFlag(CPU::N, (result & 0x00FFu) & 0x0080u);
    cpu.setFlag(CPU::C, result & 0xFF00u);
    uint16_t acc_temp = uint16_t(cpu.accumulator);
    cpu.setFlag(CPU::V, (acc_temp ^ result) & (result ^ complement) & 0x0080u);
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
    cpu.x_register = cpu.accumulator;
    cpu.setFlag(CPU::N, cpu.x_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.x_register == 0x00);
    return 0;
}

uint8_t Operation::TAY(CPU &cpu) {
    cpu.y_register = cpu.accumulator;
    cpu.setFlag(CPU::N, cpu.y_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.y_register == 0x00);
    return 0;
}

uint8_t Operation::TSX(CPU &cpu) {
    cpu.x_register = cpu.stack_pointer;
    cpu.setFlag(CPU::N, cpu.x_register & 0x80u);
    cpu.setFlag(CPU::Z, cpu.x_register == 0x00);
    return 0;
}

uint8_t Operation::TXA(CPU &cpu) {
    cpu.accumulator = cpu.x_register;
    cpu.setFlag(CPU::N, cpu.accumulator & 0x80u);
    cpu.setFlag(CPU::Z, cpu.accumulator == 0x00);
    return 0;
}

uint8_t Operation::TXS(CPU &cpu) {
    cpu.stack_pointer = cpu.x_register;
    return 0;
}

uint8_t Operation::TYA(CPU &cpu) {
    cpu.accumulator = cpu.y_register;
    cpu.setFlag(CPU::N, cpu.accumulator & 0x80u);
    cpu.setFlag(CPU::Z, cpu.accumulator == 0x00);
    return 0;
}

uint8_t Operation::XXX(CPU &cpu) {
    throw std::logic_error("UNSUPPORTED OPERATION");
    //return 0;
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

bool Operation::setAddV(uint16_t acc, uint16_t mem, uint16_t data) {
    return (~acc ^ mem) & (data ^ acc) & 0x80u;
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




