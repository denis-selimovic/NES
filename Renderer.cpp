//
// Created by denis on 25/02/2020.
//

#include <iostream>
#include "Renderer.h"

void Renderer::drawPixel(int x, int y, int r, int g, int b) {
    if(x >= 0 && y >= 0) {
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, r, g, b, 0);
        SDL_RenderDrawPoint(renderer, x, y);
        SDL_RenderPresent(renderer);
        /*pixels[WINDOW_WIDTH * y + x] = 0xFF000000u | (b << 4u) | (g << 2u) | r;
        SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH * 4);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);*/
    }

}

void Renderer::initSDL() {
    createWindow();
    createRenderer();
}

void Renderer::freeSDL() {
    cleanup(window, renderer);
    delete[] pixels;
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
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, WINDOW_WIDTH , WINDOW_HEIGHT);
    pixels = new unsigned int[WINDOW_WIDTH * WINDOW_HEIGHT * 4];
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
    bus = new Bus(cpu, ppu);
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
        while(!bus->ppu.rendered) bus->clock();
        std::cout<<"Rendering done!\n";
        bus->ppu.rendered = false;
        //drawPixel(bus->currentPixel.x, bus->currentPixel.y, bus->currentPixel.r, bus->currentPixel.g, bus->currentPixel.b);
        render();
        bus->ppu.info.resize(0);
     }
}

void Renderer::render() {
    SDL_RenderClear(renderer);
    for(auto pixel: bus->ppu.info) {
        SDL_SetRenderDrawColor(renderer, pixel.r, pixel.g, pixel.b, 255);
        SDL_RenderDrawPoint(renderer, pixel.y, pixel.x);
    }
    SDL_RenderPresent(renderer);
}

