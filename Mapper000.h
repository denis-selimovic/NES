//
// Created by denis on 26/02/2020.
//

#ifndef NES_MAPPER000_H
#define NES_MAPPER000_H


#include "Mapper.h"

class Mapper000: public Mapper{

public:
    Mapper000(uint8_t pgr_banks, uint8_t chr_banks);
};


#endif //NES_MAPPER000_H
