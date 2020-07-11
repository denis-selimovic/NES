//
// Created by deniss on 7/11/20.
//

#include "../../include/ppu/SpriteRenderer.h"

void SpriteRenderer::resetShifters() {
    for(auto &shifter: shifters) shifter.reset();
}

void SpriteRenderer::resetSprites() {
    for(auto &sprite: sprites) sprite.reset();
    spriteCount = 0;
}

void SpriteRenderer::reset() {
    resetSprites();
    resetShifters();
}

uint8_t SpriteRenderer::getSpriteCount() const {
    return spriteCount;
}

bool SpriteRenderer::findSprites(int scanLine, uint8_t spriteHeight, const std::function<int16_t(uint8_t)>& yPosFunc, const std::function<Sprite(uint8_t)>& spriteFunc) {
    bool spriteZeroEnabled = false;
    for(int i = 0; i < 64 && spriteCount <= 8; ++i) {
        int32_t diff = int16_t(scanLine) - yPosFunc(i);
        if (diff >= 0 && diff < (spriteHeight ? 16 : 8)) {
            if(spriteCount < 8) {
                if(i == 0) spriteZeroEnabled = true;
                sprites[spriteCount] = spriteFunc(i);
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
    return (scanLine - sprites[i].yPosition < 8) ? sprite8x16Helper(i, 0, scanLine) : sprite8x16Helper(i, 1, scanLine);
}

uint16_t SpriteRenderer::sprite8x16Flipped(uint8_t i, int scanLine) {
    return (scanLine - sprites[i].yPosition < 8) ? sprite8x16FlippedHelper(i, 1, scanLine) : sprite8x16FlippedHelper(i, 0, scanLine);
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

void SpriteRenderer::getSpriteToRender(uint8_t spriteHeight, uint8_t tileSelect, int scanLine, const std::function<uint8_t(uint16_t)>& func) {
    for(uint i = 0; i < spriteCount; ++i) {
        uint16_t spriteAddressLowByte, spriteAddressHighByte;
        if(spriteHeight) {
            if(!(sprites[i].attributes & 0x80u)) spriteAddressLowByte = sprite8x16(i, scanLine);
            else spriteAddressLowByte = sprite8x16Flipped(i, scanLine);
        }
        else {
            if(!(sprites[i].attributes & 0x80u)) spriteAddressLowByte = sprite8x8(i, scanLine, tileSelect);
            else spriteAddressLowByte = sprite8x8Flipped(i, scanLine, tileSelect);
        }
        spriteAddressHighByte = spriteAddressLowByte + 8;
        uint8_t spriteLowByte = func(spriteAddressLowByte), spriteHighByte = func(spriteAddressHighByte);
        if(sprites[i].attributes & 0x40u) {
            spriteLowByte = flipByte(spriteLowByte);
            spriteHighByte = flipByte(spriteHighByte);
        }
        shifters[i].setBytes(spriteLowByte, spriteHighByte);
    }
}

std::tuple<uint8_t, uint8_t, uint8_t, bool> SpriteRenderer::findNextSprite() {
    std::tuple<uint8_t, uint8_t, uint8_t, bool> tuple;
    std::get<3>(tuple) = false;
    for(uint i = 0; i < spriteCount; ++i) {
        if(sprites[i].index == 0) {
            std::get<0>(tuple) = (((shifters[i].getHighByte() & 0x80u) > 0) << 1u) | ((shifters[i].getLowByte() & 0x80u) > 0);
            std::get<1>(tuple) = (sprites[i].attributes & 0x03u) + 0x04;
            std::get<2>(tuple) = ((sprites[i].attributes & 0x20u) == 0);
            if(std::get<0>(tuple) != 0) {
                if(i == 0) std::get<3>(tuple) = true;
                break;
            }
        }
    }
    return tuple;
}

