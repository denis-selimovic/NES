//
// Created by denis on 22/02/2020.
//

#ifndef NES_BUS_H
#define NES_BUS_H

#include <cstdint>
#include <array>
#include "ppu2C02.h"

class Bus {

    std::array<uint8_t, 2048> RAM;
    ppu2C02 &ppu;
public:
     uint8_t readCPUMemory(uint16_t address);
     void writeCPUMemory(uint16_t address, uint8_t data);
     void connectPPU(ppu2C02 &ppu);
};


#endif //NES_BUS_H
