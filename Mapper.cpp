//
// Created by denis on 26/02/2020.
//

#include "Mapper.h"

Mapper::Mapper(uint8_t prg, uint8_t chr) {
    PRG_banks = prg;
    CHR_banks = chr;
}

Mapper::~Mapper() {

}
