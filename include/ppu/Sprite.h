//
// Created by deniss on 7/11/20.
//

#ifndef NES_SPRITE_H
#define NES_SPRITE_H


#include <cstdint>

class Sprite {

public:
    uint8_t yPosition = 0xFFu, tileIndex = 0xFFu, attributes = 0xFFu, index = 0xFFu;
    void reset();
};


#endif //NES_SPRITE_H
