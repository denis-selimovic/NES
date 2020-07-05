//
// Created by denis on 21/02/2020.
//

#ifndef NES_CPU6502_H
#define NES_CPU6502_H

#include <cstdint>
#include <vector>
#include "Instruction.h"
#include "AddressingMode.h"
#include "Operation.h"
#include "Bus.h"
#include "Disassembler.h"


class Instruction;
class Bus;
class Disassembler;
class Renderer;

class cpu6502 {

public:
    //maske za postavljanje bita u statusnom registru
    //redom CARRY, ZERO, INTERRUPT, DECIMAL, BREAK, UNUSED, OVERFLOW, NEGATIVE
    enum FLAGS {C = (1 << 0), Z = (1 << 1), I = (1 << 2), D = (1 << 3), B = (1 << 4), U = (1 << 5), V = (1 << 6), N = (1 << 7)};
    enum MODE {NORMAL, DEBUG};
private:
    //cpu ima 6 registara opce namjene
    uint8_t accumulator = 0x00;
    uint8_t x_register = 0x00;
    uint8_t y_register = 0x00;
    uint8_t stack_pointer = 0x00;
    uint8_t status_register = 0x00;
    uint16_t program_counter = 0x0000;


    //pokazivac na trenutnu instrukciju
    Instruction *instruction = nullptr;

    //pokazivac na sabirnicu
    Bus *bus = nullptr;

    void setFlag(FLAGS flag, bool value);

    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address);

    //tabela instrukcija
    //indeksira sa pomoću opcode-a
    std::vector<Instruction> lookup;

    //pomoćne varijable pri izvršavanju instrukcija
    uint8_t cycles = 0;
    uint8_t memory_content = 0x00;
    uint16_t relative_address = 0x00;
    uint16_t absolute_address = 0x0000;
    uint16_t debugAddress = 0x0000;

    uint8_t getMemoryContent();

    //mod emulatora
    MODE mode;

    //disassembler
    Disassembler *disassembler = nullptr;

    friend class Operation;
    friend class AddressingMode;
    friend class Debugger;
    friend class Renderer;
public:
    cpu6502(MODE mode = NORMAL);
    void connectToBus(Bus *bus);
    void setDisassembler(Disassembler *disassembler);
    void clock();
    void reset();
    void interruptRequest();
    void nonmaskableInterrupt();
    void testMode();
    std::string getInstructionName();
    uint8_t getFlag(FLAGS flag);
};


#endif //NES_CPU6502_H
