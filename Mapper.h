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
    virtual void reset() = 0;

    //funkcije za mapiranje adresa CPU
    virtual void writeCPUMemory(uint16_t address, uint32_t &mapped_address) = 0;
    virtual void readCPUMemory(uint16_t address, uint32_t &mapped_address) = 0;

    //funkcije za mapiranje adresa PPU
    virtual void writePPUMemory(uint16_t address, uint32_t &mapped_address) = 0;
    virtual void readPPUMemory(uint16_t address, uint32_t &mapped_address) = 0;
};


#endif //NES_MAPPER_H
