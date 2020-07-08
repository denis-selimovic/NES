//
// Created by denis on 21/02/2020.
//

#include <iostream>
#include "CPU.h"
#include "Operation.h"
#include "AddressingMode.h"

void CPU::setFlag(CPU::FLAGS flag, bool value) {
    value ? status_register |= flag : status_register &= ~flag;
}

uint8_t CPU::getFlag(CPU::FLAGS flag) {
    return ((status_register & flag) != 0) ? 1 : 0;
}

void CPU::connectToBus(Bus *bus) {
    this->bus = bus;
}

void CPU::write(uint16_t address, uint8_t data) {
    bus->writeCPUMemory(address, data);
}

uint8_t CPU::read(uint16_t address) {
    return bus->readCPUMemory(address);
}

CPU::CPU(MODE mode) {
    lookup =
            {
                    Instruction("BRK", &Operation::BRK, &AddressingMode::IMP, 7),
                    Instruction("ORA", &Operation::ORA, &AddressingMode::IZX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 3),
                    Instruction("ORA", &Operation::ORA, &AddressingMode::ZP0, 3),
                    Instruction("ASL", &Operation::ASL, &AddressingMode::ZP0, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("PHP", &Operation::PHP, &AddressingMode::IMP, 3),
                    Instruction("ORA", &Operation::ORA, &AddressingMode::IMM, 2),
                    Instruction("ASL", &Operation::ASL, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("ORA", &Operation::ORA, &AddressingMode::ABS, 4),
                    Instruction("ASL", &Operation::ASL, &AddressingMode::ABS, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("BPL", &Operation::BPL, &AddressingMode::REL, 2),
                    Instruction("ORA", &Operation::ORA, &AddressingMode::IZY, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("ORA", &Operation::ORA, &AddressingMode::ZPX, 4),
                    Instruction("ASL", &Operation::ASL, &AddressingMode::ZPX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("CLC", &Operation::CLC, &AddressingMode::IMP, 2),
                    Instruction("ORA", &Operation::ORA, &AddressingMode::ABY, 4),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("ORA", &Operation::ORA, &AddressingMode::ABX, 4),
                    Instruction("ASL", &Operation::ASL, &AddressingMode::ABX, 7),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("JSR", &Operation::JSR, &AddressingMode::ABS, 6),
                    Instruction("AND", &Operation::AND, &AddressingMode::IZX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("BIT", &Operation::BIT, &AddressingMode::ZP0, 3),
                    Instruction("AND", &Operation::AND, &AddressingMode::ZP0, 3),
                    Instruction("ROL", &Operation::ROL, &AddressingMode::ZP0, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("PLP", &Operation::PLP, &AddressingMode::IMP, 4),
                    Instruction("AND", &Operation::AND, &AddressingMode::IMM, 2),
                    Instruction("ROL", &Operation::ROL, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("BIT", &Operation::BIT, &AddressingMode::ABS, 4),
                    Instruction("AND", &Operation::AND, &AddressingMode::ABS, 4),
                    Instruction("ROL", &Operation::ROL, &AddressingMode::ABS, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("BMI", &Operation::BMI, &AddressingMode::REL, 2),
                    Instruction("AND", &Operation::AND, &AddressingMode::IZY, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("AND", &Operation::AND, &AddressingMode::ZPX, 4),
                    Instruction("ROL", &Operation::ROL, &AddressingMode::ZPX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("SEC", &Operation::SEC, &AddressingMode::IMP, 2),
                    Instruction("AND", &Operation::AND, &AddressingMode::ABY, 4),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("AND", &Operation::AND, &AddressingMode::ABX, 4),
                    Instruction("ROL", &Operation::ROL, &AddressingMode::ABX, 7),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("RTI", &Operation::RTI, &AddressingMode::IMP, 6),
                    Instruction("EOR", &Operation::EOR, &AddressingMode::IZX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 3),
                    Instruction("EOR", &Operation::EOR, &AddressingMode::ZP0, 3),
                    Instruction("LSR", &Operation::LSR, &AddressingMode::ZP0, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("PHA", &Operation::PHA, &AddressingMode::IMP, 3),
                    Instruction("EOR", &Operation::EOR, &AddressingMode::IMM, 2),
                    Instruction("LSR", &Operation::LSR, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("JMP", &Operation::JMP, &AddressingMode::ABS, 3),
                    Instruction("EOR", &Operation::EOR, &AddressingMode::ABS, 4),
                    Instruction("LSR", &Operation::LSR, &AddressingMode::ABS, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("BVC", &Operation::BVC, &AddressingMode::REL, 2),
                    Instruction("EOR", &Operation::EOR, &AddressingMode::IZY, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("EOR", &Operation::EOR, &AddressingMode::ZPX, 4),
                    Instruction("LSR", &Operation::LSR, &AddressingMode::ZPX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("CLI", &Operation::CLI, &AddressingMode::IMP, 2),
                    Instruction("EOR", &Operation::EOR, &AddressingMode::ABY, 4),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("EOR", &Operation::EOR, &AddressingMode::ABX, 4),
                    Instruction("LSR", &Operation::LSR, &AddressingMode::ABX, 7),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("RTS", &Operation::RTS, &AddressingMode::IMP, 6),
                    Instruction("ADC", &Operation::ADC, &AddressingMode::IZX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 3),
                    Instruction("ADC", &Operation::ADC, &AddressingMode::ZP0, 3),
                    Instruction("ROR", &Operation::ROR, &AddressingMode::ZP0, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("PLA", &Operation::PLA, &AddressingMode::IMP, 4),
                    Instruction("ADC", &Operation::ADC, &AddressingMode::IMM, 2),
                    Instruction("ROR", &Operation::ROR, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("JMP", &Operation::JMP, &AddressingMode::IND, 5),
                    Instruction("ADC", &Operation::ADC, &AddressingMode::ABS, 4),
                    Instruction("ROR", &Operation::ROR, &AddressingMode::ABS, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("BVS", &Operation::BVS, &AddressingMode::REL, 2),
                    Instruction("ADC", &Operation::ADC, &AddressingMode::IZY, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("ADC", &Operation::ADC, &AddressingMode::ZPX, 4),
                    Instruction("ROR", &Operation::ROR, &AddressingMode::ZPX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("SEI", &Operation::SEI, &AddressingMode::IMP, 2),
                    Instruction("ADC", &Operation::ADC, &AddressingMode::ABY, 4),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("ADC", &Operation::ADC, &AddressingMode::ABX, 4),
                    Instruction("ROR", &Operation::ROR, &AddressingMode::ABX, 7),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("STA", &Operation::STA, &AddressingMode::IZX, 6),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("STY", &Operation::STY, &AddressingMode::ZP0, 3),
                    Instruction("STA", &Operation::STA, &AddressingMode::ZP0, 3),
                    Instruction("STX", &Operation::STX, &AddressingMode::ZP0, 3),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 3),
                    Instruction("DEY", &Operation::DEY, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("TXA", &Operation::TXA, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("STY", &Operation::STY, &AddressingMode::ABS, 4),
                    Instruction("STA", &Operation::STA, &AddressingMode::ABS, 4),
                    Instruction("STX", &Operation::STX, &AddressingMode::ABS, 4),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 4),
                    Instruction("BCC", &Operation::BCC, &AddressingMode::REL, 2),
                    Instruction("STA", &Operation::STA, &AddressingMode::IZY, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("STY", &Operation::STY, &AddressingMode::ZPX, 4),
                    Instruction("STA", &Operation::STA, &AddressingMode::ZPX, 4),
                    Instruction("STX", &Operation::STX, &AddressingMode::ZPY, 4),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 4),
                    Instruction("TYA", &Operation::TYA, &AddressingMode::IMP, 2),
                    Instruction("STA", &Operation::STA, &AddressingMode::ABY, 5),
                    Instruction("TXS", &Operation::TXS, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 5),
                    Instruction("STA", &Operation::STA, &AddressingMode::ABX, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("LDY", &Operation::LDY, &AddressingMode::IMM, 2),
                    Instruction("LDA", &Operation::LDA, &AddressingMode::IZX, 6),
                    Instruction("LDX", &Operation::LDX, &AddressingMode::IMM, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("LDY", &Operation::LDY, &AddressingMode::ZP0, 3),
                    Instruction("LDA", &Operation::LDA, &AddressingMode::ZP0, 3),
                    Instruction("LDX", &Operation::LDX, &AddressingMode::ZP0, 3),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 3),
                    Instruction("TAY", &Operation::TAY, &AddressingMode::IMP, 2),
                    Instruction("LDA", &Operation::LDA, &AddressingMode::IMM, 2),
                    Instruction("TAX", &Operation::TAX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("LDY", &Operation::LDY, &AddressingMode::ABS, 4),
                    Instruction("LDA", &Operation::LDA, &AddressingMode::ABS, 4),
                    Instruction("LDX", &Operation::LDX, &AddressingMode::ABS, 4),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 4),
                    Instruction("BCS", &Operation::BCS, &AddressingMode::REL, 2),
                    Instruction("LDA", &Operation::LDA, &AddressingMode::IZY, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("LDY", &Operation::LDY, &AddressingMode::ZPX, 4),
                    Instruction("LDA", &Operation::LDA, &AddressingMode::ZPX, 4),
                    Instruction("LDX", &Operation::LDX, &AddressingMode::ZPY, 4),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 4),
                    Instruction("CLV", &Operation::CLV, &AddressingMode::IMP, 2),
                    Instruction("LDA", &Operation::LDA, &AddressingMode::ABY, 4),
                    Instruction("TSX", &Operation::TSX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 4),
                    Instruction("LDY", &Operation::LDY, &AddressingMode::ABX, 4),
                    Instruction("LDA", &Operation::LDA, &AddressingMode::ABX, 4),
                    Instruction("LDX", &Operation::LDX, &AddressingMode::ABY, 4),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 4),
                    Instruction("CPY", &Operation::CPY, &AddressingMode::IMM, 2),
                    Instruction("CMP", &Operation::CMP, &AddressingMode::IZX, 6),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("CPY", &Operation::CPY, &AddressingMode::ZP0, 3),
                    Instruction("CMP", &Operation::CMP, &AddressingMode::ZP0, 3),
                    Instruction("DEC", &Operation::DEC, &AddressingMode::ZP0, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("INY", &Operation::INY, &AddressingMode::IMP, 2),
                    Instruction("CMP", &Operation::CMP, &AddressingMode::IMM, 2),
                    Instruction("DEX", &Operation::DEX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("CPY", &Operation::CPY, &AddressingMode::ABS, 4),
                    Instruction("CMP", &Operation::CMP, &AddressingMode::ABS, 4),
                    Instruction("DEC", &Operation::DEC, &AddressingMode::ABS, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("BNE", &Operation::BNE, &AddressingMode::REL, 2),
                    Instruction("CMP", &Operation::CMP, &AddressingMode::IZY, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("CMP", &Operation::CMP, &AddressingMode::ZPX, 4),
                    Instruction("DEC", &Operation::DEC, &AddressingMode::ZPX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("CLD", &Operation::CLD, &AddressingMode::IMP, 2),
                    Instruction("CMP", &Operation::CMP, &AddressingMode::ABY, 4),
                    Instruction("NOP", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("CMP", &Operation::CMP, &AddressingMode::ABX, 4),
                    Instruction("DEC", &Operation::DEC, &AddressingMode::ABX, 7),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("CPX", &Operation::CPX, &AddressingMode::IMM, 2),
                    Instruction("SBC", &Operation::SBC, &AddressingMode::IZX, 6),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("CPX", &Operation::CPX, &AddressingMode::ZP0, 3),
                    Instruction("SBC", &Operation::SBC, &AddressingMode::ZP0, 3),
                    Instruction("INC", &Operation::INC, &AddressingMode::ZP0, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 5),
                    Instruction("INX", &Operation::INX, &AddressingMode::IMP, 2),
                    Instruction("SBC", &Operation::SBC, &AddressingMode::IMM, 2),
                    Instruction("NOP", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::SBC, &AddressingMode::IMP, 2),
                    Instruction("CPX", &Operation::CPX, &AddressingMode::ABS, 4),
                    Instruction("SBC", &Operation::SBC, &AddressingMode::ABS, 4),
                    Instruction("INC", &Operation::INC, &AddressingMode::ABS, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("BEQ", &Operation::BEQ, &AddressingMode::REL, 2),
                    Instruction("SBC", &Operation::SBC, &AddressingMode::IZY, 5),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 8),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("SBC", &Operation::SBC, &AddressingMode::ZPX, 4),
                    Instruction("INC", &Operation::INC, &AddressingMode::ZPX, 6),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 6),
                    Instruction("SED", &Operation::SED, &AddressingMode::IMP, 2),
                    Instruction("SBC", &Operation::SBC, &AddressingMode::ABY, 4),
                    Instruction("NOP", &Operation::NOP, &AddressingMode::IMP, 2),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
                    Instruction("???", &Operation::NOP, &AddressingMode::IMP, 4),
                    Instruction("SBC", &Operation::SBC, &AddressingMode::ABX, 4),
                    Instruction("INC", &Operation::INC, &AddressingMode::ABX, 7),
                    Instruction("???", &Operation::XXX, &AddressingMode::IMP, 7),
            };
    this->mode = mode;
}

CPU::~CPU() {
    if(!disassembler) return;
    delete disassembler;
    disassembler = nullptr;
}

void CPU::clock() {
    if(cycles == 0) {
        //ako je broj ciklusa 0 prelazimo na sljedeću instrukciju
        //opcode trenutne instrukcije dobijamo preko programskog brojača
        uint8_t opcode = read(program_counter);
        //za DEBUG mode
        debugAddress = program_counter;
        //povećamo programski brojač
        program_counter++;

        //ažuriramo novu instrukciju
        instruction = &lookup[opcode];
        //ažuriramo broj ciklusa
        cycles = instruction->total_cycles;

        //izvršimo adresiranje a zatim operaciju
        uint8_t cycles_addr = instruction->addressing_mode(*this);
        uint8_t cycle_opp = instruction->operation(*this);
        //povećamo broj ciklusa ako je ponovo potrebno
        cycles += (cycles_addr + cycle_opp);

        //postavimo U flag uvijek na 1
        setFlag(U, true);
        if(mode == DEBUG) {
            std::string ins = disassembler->getInstruction(instruction->name, opcode);
            disassembler->addInstruction(debugAddress, ins);
        }
        cycleCompleted = true;
    }
    cycles--;
}

void CPU::reset() {
    //ova tri registra se restartuju
    accumulator = x_register = y_register = 0x00;
    //stack pointer pokazuje na 0xFD
    stack_pointer = 0xFD;
    //svi statusni biti koji se koriste su 0
    status_register = 0x00u | U;
    //adresa sa koje se čita novi pc je 0xFFFC
    program_counter = readVectorPC(RESET_VECTOR);
    //broj ciklusa za reset je 8
    cycles = 8;
    //resetujemo pomoćne varijable
    absolute_address = relative_address = memory_content =  0x0000;
}

void CPU::interruptRequest() {
    if(getFlag(I) == 0) {
        //stavimo pc na stack
        pushPCToStack(program_counter);
        //stavimo statusni registar na stack
        setFlag(B, 0);
        pushToStack(status_register);
        //adresa u memoriji za novi pc je 0xFFFE
        program_counter = readVectorPC(IRQ_VECTOR);
        //broj ciklusa je 7
        cycles = 7;
    }
}

void CPU::nonMaskableInterrupt() {
    //nonmaskable interrupt se dešava uvijek
    //stavljamo pc na stack
    pushPCToStack(program_counter);
    //stavljamo statusni registar na stack
    setFlag(B, 0);
    pushToStack(status_register);
    //adresa koja se čita je 0xFFFA
    program_counter = readVectorPC(NMI_VECTOR);
    cycles = 8;
}

void CPU::getMemoryContent() {
    if(instruction->addressing_mode != &AddressingMode::IMP) memory_content = read(absolute_address);
}

void CPU::testMode() {
    accumulator = x_register = y_register = 0x00;
    stack_pointer = 0xFD;
    status_register = 0x00u | U | I;
    program_counter = 0xC000;
    debugAddress = program_counter;
}

void CPU::setDisassembler(Disassembler *disassembler) {
    this->disassembler = disassembler;
}

uint16_t CPU::formTwoByteAddress(uint16_t msb, uint16_t lsb) {
    return (msb << 8u) | lsb;
}

uint16_t CPU::readVectorPC(uint16_t address) {
    uint16_t low_byte = read(address);
    uint16_t high_byte = read(address + 1);
    return formTwoByteAddress(high_byte, low_byte);
}

void CPU::pushToStack(uint8_t data) {
    write(STACK_TOP + stack_pointer, (data & 0x00FFu));
    stack_pointer--;
}

uint8_t CPU::pullFromStack() {
    stack_pointer++;
    return read(STACK_TOP + stack_pointer);
}

void CPU::pushPCToStack(uint16_t data) {
    pushToStack(data >> 8u);
    pushToStack(data);
}

uint16_t CPU::pullPCFromStack() {
    return formTwoByteAddress(pullFromStack(), pullFromStack());
}





