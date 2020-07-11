//
// Created by deniss on 7/11/20.
//

#include "../../include/ppu/SpriteRenderer.h"

void SpriteRenderer::resetShifters() {
    for(auto &shifter: shifters) shifter.reset();
}

void SpriteRenderer::resetSprites() {
    for(auto &spirte: sprites) spirte.reset();
    spriteCount = 0;
}

void SpriteRenderer::reset() {
    resetSprites();
    resetShifters();
}

uint8_t SpriteRenderer::getSpriteCount() const {
    return spriteCount;
}

bool SpriteRenderer::findSprites(Sprite *oam, int scanLine, uint8_t spriteHeight) {
    bool spriteZeroEnabled = false;
    for(int i = 0; i < 64 && spriteCount <= 8; ++i) {
        int32_t diff = int16_t(scanLine) - int16_t(oam[i].yPosition);
        if (diff >= 0 && diff < (spriteHeight ? 16 : 8)) {
            if(spriteCount < 8) {
                if(i == 0) spriteZeroEnabled = true;
                sprites[spriteCount] = oam[i];
            }
            spriteCount++;
        }
    }
    return spriteZeroEnabled;
}

uint16_t SpriteRenderer::sprite8x8(uint8_t i, int scanLine, uint8_t tileSelect) {
    return (tileSelect << 12u) | (sprites[i].tileIndex << 4u) | (scanLine - sprites[i].yPosition);
}

uint16_t SpriteRenderer::sprite8x8Flipped(uint8_t i, int scanLine, uint8_t tileSelect) {
    return (tileSelect << 12u) | (sprites[i].tileIndex << 4u) | (7 - (scanLine - sprites[i].yPosition));
}

uint16_t SpriteRenderer::sprite8x16(uint8_t i, int scanLine) {
    return (scanLine - sprites[i].yPosition < 8) ? sprite8x16Helper(i, 0) : sprite8x16Helper(i, 1);
}

uint16_t SpriteRenderer::sprite8x16Flipped(uint8_t i, int scanLine) {
    return (scanLine - sprites[i].yPosition < 8) ? sprite8x16FlippedHelper(i, 1) : sprite8x16FlippedHelper(i, 0);
}

uint16_t SpriteRenderer::sprite8x16Helper(uint8_t i, uint8_t temp, int scanLine) {
    return ((sprites[i].tileIndex & 0x01) << 12u) | (((sprites[i].tileIndex & 0xFEu) + temp) << 4u) | ((scanLine - sprites[i].yPosition) & 0x07u);
}

uint16_t SpriteRenderer::sprite8x16FlippedHelper(uint8_t i, uint8_t temp, int scanLine) {
    return ((sprites[i].tileIndex & 0x01) << 12u) | (((sprites[i].tileIndex & 0xFEu) + temp) << 4u) | ((7 - (scanLine - sprites[i].yPosition)) & 0x07u);
}

uint8_t SpriteRenderer::flipByte(uint8_t byte) {
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

void SpriteRenderer::shift() {
    for(int i = 0; i < spriteCount; ++i) {
        if(sprites[i].index > 0) sprites[i].index--;
        else shifters[i].shift();
    }
}

