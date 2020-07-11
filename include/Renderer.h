//
// Created by denis on 25/02/2020.
//

#ifndef NES_RENDERER_H
#define NES_RENDERER_H

#include <SDL2/SDL.h>
#include <string>
#include <ostream>
#include "Bus.h"

class CPU;
class PPU;
class Bus;

class Renderer {

    // window size
    const static uint32_t WINDOW_WIDTH = 1536;
    const static uint32_t WINDOW_HEIGHT = 1440;

    // SDL utility
    void initSDL();
    void freeSDL();
    void createWindow();
    void createRenderer();
    void createTexture();
    void logError(std::ostream &os, const std::string &error);
    void throwError(const std::string &error);

    // SDL atributi
    SDL_Window  *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;

    // Oslobađanje memorije
    template <typename T, typename... Args> void cleanup(T *, Args&&... args);
    void cleanup(SDL_Window *w);
    void cleanup(SDL_Renderer *r);
    void cleanup(SDL_Texture *t);

    // povezivanje na emulator
    CPU cpu;
    PPU ppu;
    Bus *bus = nullptr;
    GamePak *gamePak = nullptr;

    // inicijalizacija emulatora
    void initNES(const std::string &nes);
    void freeNES();

    // Funkcije za crtanje na ekran
    void render();

    // rad renderera
    bool running = false;
public:
    explicit Renderer(const std::string &test = "../resources/roms/smb.nes");
    ~Renderer();
    Renderer(const Renderer &renderer) = delete;
    Renderer(Renderer &&renderer) = delete;
    void run();
};


#endif //NES_RENDERER_H
