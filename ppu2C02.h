//
// Created by denis on 24/02/2020.
//

#ifndef NES_PPU2C02_H
#define NES_PPU2C02_H


#include <cstdint>

class ppu2C02 {
public:

    uint8_t readCPUMemory(uint16_t address);
    void writeCPUMemory(uint16_t address, uint8_t data);
};


#endif //NES_PPU2C02_H
