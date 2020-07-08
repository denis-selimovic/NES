//
// Created by denis on 25/02/2020.
//

#ifndef NES_RENDERER_H
#define NES_RENDERER_H

#include <SDL2/SDL.h>
#include <string>
#include <ostream>
#include "Bus.h"

class cpu6502;
class ppu2C02;
class Bus;

class Renderer {

    // window size
    const static uint32_t WINDOW_WIDTH = 1024;
    const static uint32_t WINDOW_HEIGHT = 960;

    // SDL utility
    void initSDL();
    void freeSDL();
    void createWindow();
    void createRenderer();
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
    cpu6502 cpu;
    ppu2C02 ppu;
    Bus *bus = nullptr;
    GamePak *gamePak = nullptr;

    // inicijalizacija emulatora
    void initNES(const std::string &nes);
    void freeNES();

    // Funkcije za crtanje na ekran
    void render();

    // rad renderera
    bool running = false;
    double frame = 0;
    double sync = 0;
public:
    explicit Renderer(const std::string &test = "../nes/smb.nes");
    ~Renderer();
    Renderer(const Renderer &renderer) = delete;
    Renderer(Renderer &&renderer) = delete;
    void run();
};


#endif //NES_RENDERER_H
