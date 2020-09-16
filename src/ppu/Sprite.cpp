//
// Created by deniss on 7/11/20.
//

#include "../../include/ppu/Sprite.h"

void Sprite::reset() {
    yPosition = tileIndex = attributes = index = 0xFFu;
}
