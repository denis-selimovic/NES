//
// Created by denis on 21/02/2020.
//

#ifndef NES_CPU6502_H
#define NES_CPU6502_H

#include <cstdint>
#include "Instruction.h"

class cpu6502 {

    //cpu ima 6 registara opce namjene
    uint8_t accumulator;
    uint8_t x_register;
    uint8_t y_register;
    uint8_t stack_pointer;
    uint8_t status_register;
    uint16_t program_counter;


    //pokazivac na trenutni instrukciju
    Instruction *instruction = nullptr;

public:



};


#endif //NES_CPU6502_H
