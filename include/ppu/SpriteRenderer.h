//
// Created by deniss on 7/11/20.
//

#ifndef NES_SPRITERENDERER_H
#define NES_SPRITERENDERER_H


#include <array>
#include <functional>
#include <tuple>
#include "Sprite.h"
#include "Shifter.h"

class SpriteRenderer {

    uint8_t spriteCount = 0;
    std::array<Sprite, 8> sprites{};
    std::array<Shifter, 8> shifters{};

    uint16_t sprite8x8(uint8_t i, int scanLine, uint8_t tileSelect);
    uint16_t sprite8x8Flipped(uint8_t i, int scanLine, uint8_t tileSelect);
    uint16_t sprite8x16(uint8_t i, int scanLine);
    uint16_t sprite8x16Flipped(uint8_t i, int scanLine);
    uint16_t sprite8x16Helper(uint8_t i, uint8_t temp, int scanLine);
    uint16_t sprite8x16FlippedHelper(uint8_t i, uint8_t temp, int scanLine);
    uint8_t flipByte(uint8_t byte);
public:
    void resetShifters();
    void resetSprites();
    void reset();
    void shift();
    void getSpriteToRender(uint8_t spriteHeight, uint8_t tileSelect, int scanLine, const std::function<uint8_t(uint16_t)>& func);
    bool findSprites(int scanLine, uint8_t spriteHeight, const std::function<int16_t(uint8_t)>& yPosFunc, const std::function<Sprite(uint8_t)>& spriteFunc);
    std::tuple<uint8_t, uint8_t, uint8_t, bool> findNextSprite();
    uint8_t getSpriteCount() const;
};


#endif //NES_SPRITERENDERER_H
