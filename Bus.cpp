//
// Created by denis on 22/02/2020.
//

#include "Bus.h"

uint8_t Bus::readCPUMemory(uint16_t address) {
    if(address >= 0x0000 && address <= 0x1FFF) return RAM[address & 0x07FFu];
    else if(address >= 0x2000 && address <= 0x3FFF) return ppu->readCPUMemory(address & 0x0007u);
    return 0;
}

void Bus::writeCPUMemory(uint16_t address, uint8_t data) {
    if(address >= 0x0000 && address <= 0x01FF) RAM[address & 0x07FFu] = data;
    else if (address >= 0x2000 && address <= 0x3FFF) ppu->writeCPUMemory(address & 0x0007u, data);
}

void Bus::connectPPU(ppu2C02 *ppu2C02) {
    this->ppu = ppu2C02;
}
