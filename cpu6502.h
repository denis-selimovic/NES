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

class cpu6502 {

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

public:
    void setAddressingMode(AddressingMode * mode);
    void setOperation(Operation *operation);

};


#endif //NES_CPU6502_H
