//
// Created by deniss on 7/12/20.
//

#include "../../include/ppu/SpriteShifter.h"

void SpriteShifter::reset() {
    lowByte = highByte = 0x00;
}

void SpriteShifter::shift() {
    lowByte <<= 1u;
    highByte <<= 1u;
}

void SpriteShifter::setBytes(uint8_t low, uint8_t high) {
    lowByte = low;
    highByte = high;
}

uint8_t SpriteShifter::getHighByte() const {
    return highByte;
}

uint8_t SpriteShifter::getLowByte() const {
    return lowByte;
}
