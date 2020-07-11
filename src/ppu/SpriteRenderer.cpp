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


