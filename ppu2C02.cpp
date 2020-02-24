//
// Created by denis on 24/02/2020.
//

#include "ppu2C02.h"

uint8_t ppu2C02::readCPUMemory(uint16_t address) {
    return 0;
}

void ppu2C02::writeCPUMemory(uint16_t address, uint8_t data) {

}

uint8_t ppu2C02::readPPUMemory(uint16_t address) {
    //mapiramo adresu na opseg 0x0000 do 0x3fff
    address &= 0x3FFFu;
    if(address >= 0x0000 && address <= 0x1FFF) return pattern_table[(address & 0x1000u) >> 12u][address & 0x0FFFu];
    else if(address >= 0x2000 && address <= 0x3EFF) {
        address &= 0xFFFu;

    }
    return 0;
}

void ppu2C02::writePPUMemory(uint16_t address, uint8_t data) {

}
