//
// Created by denis on 26/02/2020.
//

#ifndef NES_MAPPER_H
#define NES_MAPPER_H


#include <cstdint>

class Mapper {

protected:
    uint8_t PRG_banks;
    uint8_t CHR_banks;

public:
    Mapper(uint8_t prg, uint8_t chr);
    virtual ~Mapper();
};


#endif //NES_MAPPER_H
