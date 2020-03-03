//
// Created by denis on 28/02/2020.
//

#include <sstream>
#include "Debugger.h"

Debugger::Debugger(const std::string &test, const std::string &font_path) {
    initNES(test);
    initSDL(font_path);
}

Debugger::~Debugger() {
    freeNES();
    freeSDL();
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

void Debugger::cleanup(SDL_Surface *s) {
    if(!s) return;
    SDL_FreeSurface(s);
}

void Debugger::cleanup(SDL_Texture *t) {
    if(!t) return;
    SDL_DestroyTexture(t);
}

void Debugger::cleanup(TTF_Font *f) {
    if(!f) return;
    TTF_CloseFont(f);
}

void Debugger::run() {
    running = true;
    SDL_Event e;
    while(running) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) running = false;
            else if(e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        clock();
                        render();
                        break;
                    case SDLK_r:
                        RAM_bank = !RAM_bank;
                        render();
                        break;
                }
            }
        }
    }
}

void Debugger::drawText(const std::string &text, const Rect &rect, const Color &c) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), {c.r, c.g, c.b, c.a});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect messageRect = drawRect(rect);
    SDL_RenderCopy(renderer, texture, nullptr, &messageRect);
    cleanup(surface);
}

SDL_Rect Debugger::drawRect(Rect r) {
    return SDL_Rect{r.x, r.y, r.w, r.h};
}

void Debugger::drawStatus() {
    std::stringstream stream;
    uint8_t status = bus->cpu.status_register;
    for(int i = 0; i < 8; ++i) {
        ((status & 0x80u) > 0) ? stream << 1 : stream << 0;
        status <<= 1u;
        if(i != 7) stream << "   ";
    }
    drawText("N   V   U   B   D   I   Z   C", {10, 10, 300, 50}, {255, 255, 255, 255});
    drawText(stream.str(), {10, 70, 300, 50}, {255, 255, 255, 255});
}

void Debugger::drawRegister(const std::string &reg, int value, const Rect &r, const Color &c) {
    std::stringstream ss;
    ss << reg << " : 0x " << std::hex << value;
    drawText(ss.str(), {r.x, r.y, r.w, r.h}, {c.r, c.g, c.b, c.a});
}

void Debugger::drawAllRegisters() {
    drawRegister("PC", bus->cpu.program_counter, {450, 10, 300, 50});
    drawRegister("X", bus->cpu.x_register, {950, 10, 300, 50});
    drawRegister("Y", bus->cpu.y_register, {1450, 10, 300, 50});
    drawRegister("SP", bus->cpu.stack_pointer, {450, 70, 300, 50});
    drawRegister("A", bus->cpu.accumulator, {950, 70, 300, 50});
}

void Debugger::drawRAM(int start) {
    std::stringstream ss;
    for(int i = 0; i < 32; ++i) {
        for(int j = 0; j < 32; ++j) {
            int value = bus->RAM[(i + start) * 32 + j];
            ss << std::hex << value;
            drawText(ss.str(), {10 + 80 * i, 130 + 50 * j, 50, 40});
            ss.str(std::string());
        }
    }
}

void Debugger::render() {
    SDL_RenderClear(renderer);
    drawStatus();
    drawAllRegisters();
    (!RAM_bank) ? drawRAM() : drawRAM(32);
    SDL_RenderPresent(renderer);
}

void Debugger::initNES(const std::string &test) {
    disassembler = new Disassembler();
    cpu = cpu6502(cpu6502::DEBUG);
    cpu.setDisassembler(disassembler);
    gamePak = new GamePak(test);
    bus = new Bus(cpu, ppu);
    bus->connectGamepak(gamePak);
    bus->cpu.testMode();
}

void Debugger::initSDL(const std::string &font_path) {
    createWindow();
    createRenderer();
    initFont();
    font = TTF_OpenFont(font_path.c_str(), FONT_SIZE);
}

void Debugger::freeNES() {
    delete bus;
    delete gamePak;
    delete disassembler;
}

void Debugger::freeSDL() {
    cleanup(window, renderer, font);
    SDL_Quit();
    TTF_Quit();
}

void Debugger::clock() {
    do {
        bus->clock();
    }
    while(bus->cpu.cycles != 0 || bus->cycles % 3 != 0);
}













