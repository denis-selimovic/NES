//
// Created by denis on 28/02/2020.
//

#ifndef NES_DEBUGGER_H
#define NES_DEBUGGER_H


#include <iostream>
#include "Bus.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Debugger {

    struct Color {
        uint8_t r, g, b, a;
    };

    struct Rect {
        int x, y, w, h;
    };


    //dimenzije ekrana
    const static uint32_t WINDOW_WIDTH = 3400;
    const static uint32_t WINDOW_HEIGHT = 2000;
    const static uint32_t FONT_SIZE = 10;

    //NES
    cpu6502 cpu;
    ppu2C02 ppu;
    GamePak *gamePak = nullptr;
    Bus *bus = nullptr;

    //init functions
    void initNES(const std::string &nes);
    void initSDL(const std::string &font);

    //free memory functions
    void freeNES();
    void freeSDL();

    //SDL
    void logError(std::ostream &os, const std::string &error);
    void throwError(const std::string &error);
    void createWindow();
    void createRenderer();
    void initFont();

    //oslobađanje memorije
    template <typename T, typename... Args> void cleanup(T *, Args&&... args);
    void cleanup(SDL_Window *w);
    void cleanup(SDL_Renderer *r);
    void cleanup(SDL_Surface *s);
    void cleanup(SDL_Texture *t);
    void cleanup(TTF_Font *f);

    //crtanje stanja emulatora
    void render();
    void drawRAM(int start = 0);
    void drawAllRegisters();
    void drawRegister(const std::string &reg, int value, const Rect &r, const Color &c = {255, 255, 255, 255});
    void drawStatus();
    void drawText(const std::string &text, const Rect &rect, const Color &c = {255, 255, 255, 255});
    static SDL_Rect drawRect(Rect rect);

    //rad emulatora
    void clock();

    //atributi
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    //pomoćni atributi
    bool running = false;
    bool RAM_bank = false;
public:
    Debugger(const std::string &test = "../nes/nestest.nes", const std::string & font_path = "../fonts/sans.ttf");
    ~Debugger();
    Debugger(const Debugger &debugger) = delete;
    Debugger(Debugger &&debugger) = delete;

    void run();
};


#endif //NES_DEBUGGER_H
