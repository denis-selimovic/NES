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
    std::array<Shifter, 8> shifters;
public:
    void resetShifters();
    void resetSprites();
    void reset();
    bool findSprites(Sprite *oam, int scanLine, uint8_t spriteHeight);
    uint8_t getSpriteCount() const;
};


#endif //NES_SPRITERENDERER_H
