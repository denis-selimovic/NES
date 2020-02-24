//
// Created by denis on 22/02/2020.
//

#include "Bus.h"

uint8_t Bus::readCpuRAM(uint16_t address) {
    if(address >= 0x0000 && address <= 0xFFFF) return RAM[address];
    return 0;
}

void Bus::writeCpuRAM(uint16_t address, uint8_t data) {
    if(address >= 0x0000 && address <= 0xFFFF) RAM[address] = data;
}
