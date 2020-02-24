//
// Created by denis on 21/02/2020.
//

#include "Operation.h"

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
    if(cpu.getFlag(cpu6502::N) == 0) {
        cpu.cycles++;
        cpu.absolute_adress = cpu.program_counter + cpu.relative_address;
        //provjera stranice
        if((cpu.absolute_adress & 0xFF00u) != (cpu.program_counter & 0xFF00u)) cpu.cycles++;
        cpu.program_counter = cpu.absolute_adress;
    }
    return 0;
}

uint8_t Operation::BRK(cpu6502 &cpu) {
    // BRK ima dvobajtni opcode
    // zbog se sljedeća vrijednost pc-a ignoriše nakon što je pročitan prvi bajt opcode-a za BRK -> inkrementirano pc
    cpu.program_counter++;
    //postavimo I flag na 1
    cpu.setFlag(cpu6502::I, true);
    //programski brojač stavimo na stack
    //na stack prvo ide prvih 8 bita pc-a (najznačajnijih)
    cpu.write(0x0100 + cpu.stack_pointer, (cpu.program_counter  >> 8u) & 0x00FFu);
    cpu.stack_pointer--;
    //zatim na stack ide 8 najmanje značajnih bita
    cpu.write(0x0100 + cpu.stack_pointer, cpu.program_counter & 0x00FFu);
    cpu.stack_pointer--;

    //nakon ovog na stack se stavlja sadržaj statusnog registra sa B flagom postavljenim na 1
    //B flag se nakon toga vraća na 0
    cpu.setFlag(cpu6502::B, true);
    cpu.write(0x0100 + cpu.stack_pointer, cpu.status_register);
    cpu.setFlag(cpu6502::B, false);
    cpu.stack_pointer--;

    //adresa interrupt rutine se dobija sa lokacija 0xFFFF (high byte) i 0xFFFE (low byte)
    cpu.program_counter = uint16_t(cpu.read(0xFFFF) << 8u) | uint16_t(cpu.read(0xFFFE));
    return 0;
}

uint8_t Operation::BVC(cpu6502 &cpu) {
    if(cpu.getFlag(cpu6502::V) == 0) {
        cpu.cycles++;
        cpu.absolute_adress = cpu.program_counter + cpu.relative_address;
        //ako pc i novi pc nisu na istoj stranici broj ciklusa se povecava
        if((cpu.absolute_adress & 0xFF00u) != (cpu.program_counter & 0xFF00u)) cpu.cycles++;
        cpu.program_counter = cpu.program_counter;
    }
    return 0;
}

uint8_t Operation::BVS(cpu6502 &cpu) {
    if(cpu.getFlag(cpu6502::V) == 1) {
        cpu.cycles++;
        cpu.absolute_adress = cpu.program_counter + cpu.relative_address;
        //ako pc i novi pc nisu na istoj stranici broj ciklusa se povecava
        if((cpu.absolute_adress & 0xFF00u) != (cpu.program_counter & 0xFF00u)) cpu.cycles++;
        cpu.program_counter = cpu.program_counter;
    }
    return 0;
}

uint8_t Operation::CLC(cpu6502 &cpu) {
    cpu.setFlag(cpu6502::C, false);
    return 0;
}

uint8_t Operation::CLD(cpu6502 &cpu) {
    cpu.setFlag(cpu6502::D, false);
    return 0;
}

uint8_t Operation::CLI(cpu6502 &cpu) {
    cpu.setFlag(cpu6502::I, false);
    return 0;
}

uint8_t Operation::CLV(cpu6502 &cpu) {
    cpu.setFlag(cpu6502::V, false);
    return 0;
}

uint8_t Operation::CMP(cpu6502 &cpu) {
    cpu.getMemoryContent();
    uint16_t result = uint16_t(cpu.accumulator) - uint16_t(cpu.memory_content);
    cpu.setFlag(cpu6502::Z, (result & 0x00FFu) == 0x0000);
    cpu.setFlag(cpu6502::N, result & 0x0080u);
    //C flag se postavlja ako je accumulator >= memory
    cpu.setFlag(cpu6502::C, cpu.accumulator >= cpu.memory_content);
    return 1;
}

uint8_t Operation::CPX(cpu6502 &cpu) {
    cpu.getMemoryContent();
    uint16_t result = uint16_t(cpu.x_register) - uint16_t(cpu.memory_content);
    cpu.setFlag(cpu6502::Z, (result & 0x00FFu) == 0x0000);
    cpu.setFlag(cpu6502::N, result & 0x0080u);
    //C flag se postavlja ako x >= memory
    cpu.setFlag(cpu6502::C, cpu.x_register >= cpu.memory_content);
    return 0;
}

uint8_t Operation::CPY(cpu6502 &cpu) {
    cpu.getMemoryContent();
    uint16_t result = uint16_t(cpu.y_register) - uint16_t(cpu.memory_content);
    cpu.setFlag(cpu6502::Z, (result & 0x00FFu) == 0x0000);
    cpu.setFlag(cpu6502::N, result & 0x0080u);
    //C flag se postavlja ako je y >= memory
    cpu.setFlag(cpu6502::C, cpu.y_register >= cpu.memory_content);
    return 0;
}

uint8_t Operation::DEC(cpu6502 &cpu) {
    cpu.getMemoryContent();
    uint16_t result = cpu.memory_content - 1;
    cpu.write(cpu.absolute_adress, result & 0x00FFu);
    cpu.setFlag(cpu6502::N, result & 0x0080u);
    cpu.setFlag(cpu6502::Z, (result & 0x00FFu) == 0x0000);
    return 0;
}

uint8_t Operation::DEX(cpu6502 &cpu) {
    cpu.x_register--;
    cpu.setFlag(cpu6502::N, cpu.x_register & 0x80u);
    cpu.setFlag(cpu6502::Z, cpu.x_register == 0x00);
    return 0;
}

uint8_t Operation::DEY(cpu6502 &cpu) {
    cpu.y_register--;
    cpu.setFlag(cpu6502::N, cpu.y_register & 0x80u);
    cpu.setFlag(cpu6502::Z, cpu.y_register == 0x00);
    return 0;
}

uint8_t Operation::EOR(cpu6502 &cpu) {
    cpu.getMemoryContent();
    cpu.accumulator ^= cpu.memory_content;
    cpu.setFlag(cpu6502::N, cpu.accumulator & 0x80u);
    cpu.setFlag(cpu6502::Z, cpu.accumulator == 0x00);
    return 1;
}

uint8_t Operation::INC(cpu6502 &cpu) {
    cpu.getMemoryContent();
    uint16_t result = cpu.memory_content + 1;
    cpu.write(cpu.absolute_adress, result & 0x00FFu);
    cpu.setFlag(cpu6502::N, result & 0x0080u);
    cpu.setFlag(cpu6502::Z, (result & 0x00FFu) == 0x0000);
    return 0;
}

uint8_t Operation::INX(cpu6502 &cpu) {
    cpu.x_register++;
    cpu.setFlag(cpu6502::N, cpu.x_register & 0x80u);
    cpu.setFlag(cpu6502::Z, cpu.x_register == 0x00);
    return 0;
}

uint8_t Operation::INY(cpu6502 &cpu) {
    cpu.y_register++;
    cpu.setFlag(cpu6502::N, cpu.y_register & 0x80u);
    cpu.setFlag(cpu6502::Z, cpu.y_register == 0x00);
    return 0;
}

uint8_t Operation::JMP(cpu6502 &cpu) {
    cpu.program_counter = cpu.relative_address;
    return 0;
}

uint8_t Operation::JSR(cpu6502 &cpu) {
    //cuvamo mjesto povratka tj pc--
    cpu.program_counter--;

    //stavimo pc na stek
    cpu.write(0x0100 + cpu.stack_pointer, (cpu.program_counter >> 8u) & 0x00FFu);
    cpu.stack_pointer--;
    cpu.write(0x0100 + cpu.stack_pointer, cpu.program_counter & 0x00FFu);
    cpu.stack_pointer--;

    //mijenjamo pc
    cpu.program_counter = cpu.absolute_adress;
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
