//
// Created by deniss on 7/11/20.
//

#ifndef NES_SPRITERENDERER_H
#define NES_SPRITERENDERER_H


#include <array>
#include "Sprite.h"
#include "Shifter.h"

class SpriteRenderer {

    uint8_t spriteCount = 0;
    std::array<Sprite, 8> sprites{};
    std::array<Shifter, 8> lowBytes{};
    std::array<Shifter, 8> highBytes{};
public:
};


#endif //NES_SPRITERENDERER_H
