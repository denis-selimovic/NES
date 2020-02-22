//
// Created by denis on 21/02/2020.
//

#ifndef NES_CPU6502_H
#define NES_CPU6502_H

#include <cstdint>
#include "Instruction.h"
#include "AddressingMode.h"
#include "Operation.h"

class Instruction;
class AddressingMode;
class Operation;
class Bus;

class cpu6502 {

public:
    //maske za postavljanje bita u statusnom registru
    //redom CARRY, ZERO, INTERRUPT, DECIMAL, BREAK, UNUSED, OVERFLOW, NEGATIVE
    enum FLAGS {C = (1 << 0), Z = (1 << 1), I = (1 << 2), D = (1 << 3), B = (1 << 4), U = (1 << 5), V = (1 << 6), N = (1 << 7)};

private:
    //cpu ima 6 registara opce namjene
    uint8_t accumulator;
    uint8_t x_register;
    uint8_t y_register;
    uint8_t stack_pointer;
    uint8_t status_register;
    uint16_t program_counter;


    //pokazivac na trenutnu instrukciju
    Instruction *instruction = nullptr;

    //pokazivac na mod adresiranja
    AddressingMode *addr_mode = nullptr;

    //pokazivac na implementaciju instrukcije
    Operation *operation = nullptr;

    //pokazivac na sabirnicu
    Bus *bus = nullptr;

    void setFlag(FLAGS flag, bool value);
    uint8_t getFlag(FLAGS flag);

public:

    void setAddressingMode(AddressingMode * mode);
    void setOperation(Operation *operation);
    void connectToBus(Bus *bus);
};


#endif //NES_CPU6502_H
