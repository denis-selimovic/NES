//
// Created by deniss on 7/11/20.
//

#include "../../include/ppu/Tile.h"

void Tile::reset() {
    id = attribute = lsb = msb = 0x00;
}

uint8_t Tile::getPackingByte(uint8_t mask) const {
    return (attribute & mask) ? 0xFFu : 0x00u;
}
