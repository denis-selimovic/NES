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
    lookup =
            {
                    { "BRK", &Operation::BRK, &AddressingMode::IMM, 7 },{ "ORA", &Operation::ORA, &AddressingMode::IZX, 6 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 3 },{ "ORA", &Operation::ORA, &AddressingMode::ZP0, 3 },{ "ASL", &Operation::ASL, &AddressingMode::ZP0, 5 },{ "???", &Operation::XXX, &AddressingMode::IMP, 5 },{ "PHP", &Operation::PHP, &AddressingMode::IMP, 3 },{ "ORA", &Operation::ORA, &AddressingMode::IMM, 2 },{ "ASL", &Operation::ASL, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::NOP, &AddressingMode::IMP, 4 },{ "ORA", &Operation::ORA, &AddressingMode::ABS, 4 },{ "ASL", &Operation::ASL, &AddressingMode::ABS, 6 },{ "???", &Operation::XXX, &AddressingMode::IMP, 6 },
                    { "BPL", &Operation::BPL, &AddressingMode::REL, 2 },{ "ORA", &Operation::ORA, &AddressingMode::IZY, 5 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 4 },{ "ORA", &Operation::ORA, &AddressingMode::ZPX, 4 },{ "ASL", &Operation::ASL, &AddressingMode::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "JSR", &Operation::JSR, &AddressingMode::ABS, 6 },{ "AND", &Operation::AND, &AddressingMode::IZX, 6 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "BIT", &Operation::BIT, &AddressingMode::ZP0, 3 },{ "AND", &Operation::AND, &AddressingMode::ZP0, 3 },{ "ROL", &Operation::ROL, &AddressingMode::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BMI", &Operation::BMI, &AddressingMode::REL, 2 },{ "AND", &Operation::AND, &AddressingMode::IZY, 5 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 4 },{ "AND", &Operation::AND, &AddressingMode::ZPX, 4 },{ "ROL", &Operation::ROL, &AddressingMode::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "RTI", &Operation::RTI, &AddressingMode::IMP, 6 },{ "EOR", &Operation::EOR, &AddressingMode::IZX, 6 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 3 },{ "EOR", &Operation::EOR, &AddressingMode::ZP0, 3 },{ "LSR", &Operation::LSR, &AddressingMode::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BVC", &Operation::BVC, &AddressingMode::REL, 2 },{ "EOR", &Operation::EOR, &AddressingMode::IZY, 5 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 4 },{ "EOR", &Operation::EOR, &AddressingMode::ZPX, 4 },{ "LSR", &Operation::LSR, &AddressingMode::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "RTS", &Operation::RTS, &AddressingMode::IMP, 6 },{ "ADC", &Operation::ADC, &AddressingMode::IZX, 6 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 3 },{ "ADC", &Operation::ADC, &AddressingMode::ZP0, 3 },{ "ROR", &Operation::ROR, &AddressingMode::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BVS", &Operation::BVS, &AddressingMode::REL, 2 },{ "ADC", &Operation::ADC, &AddressingMode::IZY, 5 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 4 },{ "ADC", &Operation::ADC, &AddressingMode::ZPX, 4 },{ "ROR", &Operation::ROR, &AddressingMode::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "???", &Operation::NOP, &AddressingMode::IMP, 2 },{ "STA", &Operation::STA, &AddressingMode::IZX, 6 },{ "???", &Operation::NOP, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 6 },{ "STY", &Operation::STY, &AddressingMode::ZP0, 3 },{ "STA", &Operation::STA, &AddressingMode::ZP0, 3 },{ "STX", &Operation::STX, &AddressingMode::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
                    { "BCC", &Operation::BCC, &AddressingMode::REL, 2 },{ "STA", &Operation::STA, &AddressingMode::IZY, 6 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 6 },{ "STY", &Operation::STY, &AddressingMode::ZPX, 4 },{ "STA", &Operation::STA, &AddressingMode::ZPX, 4 },{ "STX", &Operation::STX, &AddressingMode::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
                    { "LDY", &Operation::LDY, &AddressingMode::IMM, 2 },{ "LDA", &Operation::LDA, &AddressingMode::IZX, 6 },{ "LDX", &Operation::LDX, &AddressingMode::IMM, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 6 },{ "LDY", &Operation::LDY, &AddressingMode::ZP0, 3 },{ "LDA", &Operation::LDA, &AddressingMode::ZP0, 3 },{ "LDX", &Operation::LDX, &AddressingMode::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
                    { "BCS", &Operation::BCS, &AddressingMode::REL, 2 },{ "LDA", &Operation::LDA, &AddressingMode::IZY, 5 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 5 },{ "LDY", &Operation::LDY, &AddressingMode::ZPX, 4 },{ "LDA", &Operation::LDA, &AddressingMode::ZPX, 4 },{ "LDX", &Operation::LDX, &AddressingMode::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
                    { "CPY", &Operation::CPY, &AddressingMode::IMM, 2 },{ "CMP", &Operation::CMP, &AddressingMode::IZX, 6 },{ "???", &Operation::NOP, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "CPY", &Operation::CPY, &AddressingMode::ZP0, 3 },{ "CMP", &Operation::CMP, &AddressingMode::ZP0, 3 },{ "DEC", &Operation::DEC, &AddressingMode::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BNE", &Operation::BNE, &AddressingMode::REL, 2 },{ "CMP", &Operation::CMP, &AddressingMode::IZY, 5 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 4 },{ "CMP", &Operation::CMP, &AddressingMode::ZPX, 4 },{ "DEC", &Operation::DEC, &AddressingMode::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
                    { "CPX", &Operation::CPX, &AddressingMode::IMM, 2 },{ "SBC", &Operation::SBC, &AddressingMode::IZX, 6 },{ "???", &Operation::NOP, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "CPX", &Operation::CPX, &AddressingMode::ZP0, 3 },{ "SBC", &Operation::SBC, &AddressingMode::ZP0, 3 },{ "INC", &Operation::INC, &AddressingMode::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
                    { "BEQ", &Operation::BEQ, &AddressingMode::REL, 2 },{ "SBC", &Operation::SBC, &AddressingMode::IZY, 5 },{ "???", &Operation::XXX, &AddressingMode::IMP, 2 },{ "???", &Operation::XXX, &AddressingMode::IMP, 8 },{ "???", &Operation::NOP, &AddressingMode::IMP, 4 },{ "SBC", &Operation::SBC, &AddressingMode::ZPX, 4 },{ "INC", &Operation::INC, &AddressingMode::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            };
}


