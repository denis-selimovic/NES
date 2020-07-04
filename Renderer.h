//
// Created by denis on 25/02/2020.
//

#ifndef NES_RENDERER_H
#define NES_RENDERER_H

#include <SDL2/SDL.h>
#include <string>
#include <ostream>

class Renderer {

    // window size
    const static uint32_t WINDOW_WIDTH = 256;
    const static uint32_t WINDOW_HEIGHT = 240;

    // SDL utility
    void initSDL();
    void freeSDL();
    void createWindow();
    void createRenderer();
    void logError(std::ostream &os, const std::string &error);
    void throwError(const std::string &error);

    // SDL atributi
    SDL_Window  *window;
    SDL_Renderer *renderer;

    // Oslobađanje memorije
    template <typename T, typename... Args> void cleanup(T *, Args&&... args);
    void cleanup(SDL_Window *w);
    void cleanup(SDL_Renderer *r);
public:
    Renderer();
    ~Renderer();
    Renderer(const Renderer &renderer) = delete;
    Renderer(Renderer &&renderer) = delete;
    void drawPixel(uint x, uint y, int r, int g, int b);
};


#endif //NES_RENDERER_H
