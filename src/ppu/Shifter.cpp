//
// Created by deniss on 7/11/20.
//

#include "../../include/ppu/Shifter.h"

void Shifter::shift() {
    lowByte <<= 1u;
    highByte <<= 1u;
}

void Shifter::packLowByte(uint8_t data) {
    lowByte = (lowByte & 0xFF00u) | data;
}

void Shifter::packHighByte(uint8_t data) {
    highByte = (highByte & 0xFF00u) | data;
}

void Shifter::pack(uint8_t low, uint8_t high) {
    packLowByte(low);
    packHighByte(high);
}

uint16_t Shifter::getLowByte() const {
    return lowByte;
}

uint16_t Shifter::getHighByte() const {
    return highByte;
}

void Shifter::reset() {
    lowByte = highByte = 0x0000u;
}
