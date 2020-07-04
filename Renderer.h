//
// Created by denis on 25/02/2020.
//

#ifndef NES_RENDERER_H
#define NES_RENDERER_H

#include <SDL2/SDL.h>

class Renderer {

public:
    void drawPixel(uint x, uint y, uint pixel, uint palette);
};


#endif //NES_RENDERER_H
