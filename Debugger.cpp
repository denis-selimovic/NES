//
// Created by denis on 28/02/2020.
//

#include "Debugger.h"

Debugger::Debugger(const std::string &test) {
    gamePak = new GamePak(test);
    bus = new Bus(cpu, ppu);
    bus->connectGamepak(gamePak);
    createWindow();
    createRenderer();
    initFont();
    font = TTF_OpenFont("../fonts/sans.ttf", FONT_SIZE);
}

Debugger::~Debugger() {
    delete bus;
    delete gamePak;
    cleanup(window, renderer);
    TTF_CloseFont(font);
    SDL_Quit();
}

void Debugger::logError(std::ostream &os, const std::string &error) {
    os << error << " Error:" << SDL_GetError() <<std::endl;
}

void Debugger::createWindow() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logError(std::cout, "SDL_INIT");
        throwError("SDL_INIT");
    }
    window = SDL_CreateWindow("NES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr) {
        logError(std::cout, "SDL_CreateWindow");
        SDL_Quit();
        throwError("SDL_CreateWindow");
    }
}

void Debugger::createRenderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr) {
        logError(std::cout, "SDL_CreateRenderer");
        cleanup(window);
        SDL_Quit();
        throwError("SDL_CreateRenderer");
    }
}

void Debugger::initFont() {
    if(TTF_Init() != 0) {
        throwError("ERROR");
    }
}

void Debugger::throwError(const std::string &error) {
    throw std::logic_error(error);
}

template<typename T, typename... Args>
void Debugger::cleanup(T *t, Args &&... args) {
    cleanup(t);
    cleanup(std::forward<Args>(args)...);
}

void Debugger::cleanup(SDL_Window *w) {
    if(!w) return;
    SDL_DestroyWindow(w);
}

void Debugger::cleanup(SDL_Renderer *r) {
    if(!r) return;
    SDL_DestroyRenderer(r);
}

void Debugger::run() {
    running = true;
    drawStatus();
    SDL_Event e;
    while(running) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) running = false;
        }
    }
}

void Debugger::drawText(std::string text) {
    SDL_Color white = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, "C  Z  I  D  B  U  V  N", white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect messageRect = drawRect(0, 0, 150, 50);
    SDL_RenderCopy(renderer, texture, nullptr, &messageRect);
    SDL_RenderPresent(renderer);
}

SDL_Rect Debugger::drawRect(int x, int y, int w, int h) {
    return SDL_Rect{x, y, w, h};
}








