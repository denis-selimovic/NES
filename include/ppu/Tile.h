//
// Created by deniss on 7/11/20.
//

#ifndef NES_TILE_H
#define NES_TILE_H


#include <cstdint>

class Tile {

public:
    uint8_t id = 0x00, attribute = 0x00, lsb = 0x00, msb = 0x00;
    void reset();
};


#endif //NES_TILE_H
