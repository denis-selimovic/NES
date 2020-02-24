//
// Created by denis on 21/02/2020.
//

#include "Operation.h"

uint8_t Operation::BRK(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::ADC(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::AND(cpu6502 &cpu) {
    cpu.getMemoryContent();
    cpu.accumulator &= cpu.memory_content;
    cpu.setFlag(cpu6502::Z, cpu.accumulator == 0x00);
    cpu.setFlag(cpu6502::N, cpu.accumulator & 0x80u);
    return 0;
}

uint8_t Operation::ASL(cpu6502 &cpu) {
    cpu.getMemoryContent();
    uint16_t shifted_value = uint16_t(cpu.memory_content) << 1u;
    cpu.setFlag(cpu6502::C, (shifted_value & 0xFF00u) > 0);
    cpu.setFlag(cpu6502::Z, (shifted_value & 0x00FFu) == 0x0000);
    cpu.setFlag(cpu6502::N, shifted_value & 0x80u);
    if(cpu.instruction->addressing_mode == &AddressingMode::IMP) cpu.accumulator = shifted_value & 0x00FFu;
    else cpu.write(cpu.absolute_adress, shifted_value & 0x00FFu);
    return 0;
}

uint8_t Operation::BCC(cpu6502 &cpu) {
    if(cpu.getFlag(cpu6502::C) == 0) {
        cpu.cycles++;
        cpu.absolute_adress = cpu.program_counter + cpu.relative_address;
        //Ako je dodavanje relativnog skoka promijenilo stranicu u memoriju broj ciklusa se ponovo povecava
        //Ovo provjeravamo poredeći broj stranice pc-a i nove apsolutne adrese
        if((cpu.absolute_adress & 0xFF00u) != (cpu.program_counter & 0xFF00u)) cpu.cycles++;
        cpu.program_counter = cpu.absolute_adress;
    }
    return 0;
}

uint8_t Operation::BCS(cpu6502 &cpu) {
    if(cpu.getFlag(cpu6502::C) == 1) {
        cpu.cycles++;
        cpu.absolute_adress = cpu.program_counter + cpu.relative_address;
        //ako je promijenjen broj stranice povećavamo broj ciklusa za 1
        if((cpu.absolute_adress & 0xFF00u) != (cpu.program_counter & 0xFF00u)) cpu.cycles++;
        cpu.program_counter = cpu.absolute_adress;
    }
    return 0;
}

uint8_t Operation::BEQ(cpu6502 &cpu) {
    if(cpu.getFlag(cpu6502::Z) == 1) {
        cpu.cycles++;
        cpu.absolute_adress = cpu.program_counter + cpu.relative_address;
        //ako pc i apsolutna adresa nisu na istoj stranici povecavamo broj ciklusa za 1
        if((cpu.absolute_adress & 0xFF00u) != (cpu.program_counter & 0xFF00u)) cpu.cycles++;
        cpu.program_counter = cpu.absolute_adress;
    }
    return 0;
}

uint8_t Operation::BIT(cpu6502 &cpu) {
    cpu.getMemoryContent();
    uint8_t result = cpu.accumulator & cpu.memory_content;
    cpu.setFlag(cpu6502::V, cpu.memory_content & (1u << 6u));
    cpu.setFlag(cpu6502::N, cpu.memory_content & (1u << 7u));
    cpu.setFlag(cpu6502::Z, result == 0x00);
    return 0;
}

uint8_t Operation::BMI(cpu6502 &cpu) {
    if(cpu.getFlag(cpu6502::N) == 1) {
        cpu.cycles++;
        cpu.absolute_adress = cpu.program_counter + cpu.relative_address;
        //provjera da li su novi pc i stari pc na istoj stranici u memoriji
        if((cpu.absolute_adress & 0xFF00u) != (cpu.program_counter & 0xFF00u)) cpu.cycles++;
        cpu.program_counter = cpu.absolute_adress;
    }
    return 0;
}

uint8_t Operation::BNE(cpu6502 &cpu) {
    if(cpu.getFlag(cpu6502::Z) == 0) {
        cpu.cycles++;
        cpu.absolute_adress = cpu.program_counter + cpu.relative_address;
        //provjera koji je broj stranica novog pc-a i starog pc-a
        if((cpu.absolute_adress & 0xFF00u) != (cpu.program_counter & 0xFF00u)) cpu.cycles++;
        cpu.program_counter = cpu.absolute_adress;
    }
    return 0;
}

uint8_t Operation::BPL(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::BVC(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::BVS(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::CLC(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::CLD(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::CLI(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::CLV(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::CMP(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::CPX(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::CPY(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::DEC(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::DEX(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::DEY(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::EOR(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::INC(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::INX(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::INY(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::JMP(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::JSR(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::LDA(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::LDX(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::LDY(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::LSR(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::NOP(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::ORA(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::PHA(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::PHP(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::PLA(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::PLP(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::ROL(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::ROR(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::RTI(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::RTS(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::SBC(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::SEC(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::SED(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::SEI(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::STA(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::STX(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::STY(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::TAX(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::TAY(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::TSX(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::TXA(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::TXS(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::TYA(cpu6502 &cpu) {
    return 0;
}

uint8_t Operation::XXX(cpu6502 &cpu) {
    return 0;
}
