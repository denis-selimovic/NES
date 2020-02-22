//
// Created by denis on 22/02/2020.
//

#ifndef NES_BUS_H
#define NES_BUS_H

#include <cstdint>
#include <array>

class Bus {

    std::array<uint8_t, 64 * 1024> RAM;
public:
     uint8_t readMemory(uint16_t address);
     void writeMemory(uint16_t address, uint8_t data);
};


#endif //NES_BUS_H
