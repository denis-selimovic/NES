//
// Created by denis on 25/02/2020.
//

#include <iostream>
#include "Renderer.h"

void Renderer::drawPixel(uint x, uint y, int r, int g, int b) {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawPoint(renderer, x, y);
    SDL_RenderPresent(renderer);
}

void Renderer::initSDL() {
    createWindow();
    createRenderer();
}

void Renderer::freeSDL() {
    cleanup(window, renderer);
    SDL_Quit();
}

void Renderer::createWindow() {
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

void Renderer::createRenderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr) {
        logError(std::cout, "SDL_CreateRenderer");
        cleanup(window);
        SDL_Quit();
        throwError("SDL_CreateRenderer");
    }
}

void Renderer::logError(std::ostream &os, const std::string &error) {
    os << error << " Error:" << SDL_GetError() << std::endl;
}

void Renderer::throwError(const std::string &error) {
    throw std::logic_error(error);
}

template<typename T, typename... Args>
void Renderer::cleanup(T *t, Args &&... args) {
    cleanup(t);
    cleanup(std::forward<Args>(args)...);
}

void Renderer::cleanup(SDL_Window *w) {
    if(!w) return;
    SDL_DestroyWindow(w);
}

void Renderer::cleanup(SDL_Renderer *r) {
    if(!r) return;
    SDL_DestroyRenderer(r);
}

Renderer::Renderer(const std::string &nes) {
    initNES(nes);
    initSDL();
}

Renderer::~Renderer() {
    freeNES();
    freeSDL();
}

void Renderer::initNES(const std::string &nes) {
    bus = new Bus(cpu6502, ppu2C02);
    gamePak = new GamePak(nes);
    bus->connectGamepak(gamePak);
}

void Renderer::freeNES() {
    delete gamePak;
    delete bus;
}

void Renderer::run() {
    running = true;
    while(running) {
        bus->clock();
        drawPixel(bus->currentPixel.x, bus->currentPixel.y, bus->currentPixel.r, bus->currentPixel.g, bus->currentPixel.b);
    }
}

