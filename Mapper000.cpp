//
// Created by denis on 26/02/2020.
//

#include "Mapper000.h"

Mapper000::Mapper000(uint8_t pgr_banks, uint8_t chr_banks) : Mapper(pgr_banks, chr_banks) {}

void Mapper000::reset() {

}

bool Mapper000::writeCPUMemory(uint16_t address, uint32_t &mapped_address) {
    return false;
}

bool Mapper000::readCPUMemory(uint16_t address, uint32_t &mapped_address) {
    return false;
}

bool Mapper000::writePPUMemory(uint16_t address, uint32_t &mapped_address) {
    return false;
}

bool Mapper000::readPPUMemory(uint16_t address, uint32_t &mapped_address) {
    return false;
}
