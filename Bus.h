//
// Created by denis on 22/02/2020.
//

#ifndef NES_BUS_H
#define NES_BUS_H

#include <cstdint>
#include <array>
#include "ppu2C02.h"
#include "CPU.h"
#include "Key.h"

class Bus {

    std::array<uint8_t, 2048> RAM{};
    std::array<uint8_t , 2> joystick{};

    ppu2C02 &ppu;
    CPU &cpu;

    uint32_t cycles = 0;

    struct DirectMemoryAccess {
        uint8_t page = 0x00, address = 0x00, data = 0x00;
        bool transfer = false, disable = true;
    } DMA;

    void startDMA();
    void readDMA();
    void writeDMA();

    GamePak *gamePak = nullptr;

    friend class Debugger;
    friend class Renderer;

public:
    Key X,Z,A,S,UP,DOWN,LEFT,RIGHT;
    std::array<uint8_t, 2> joystickBuffer{};

public:
     Bus(CPU &cpu, ppu2C02 &ppu);
     ~Bus();
     Bus(const Bus &bus) = delete;
     Bus(Bus &&bus) = delete;

     uint8_t readCPUMemory(uint16_t address);
     void writeCPUMemory(uint16_t address, uint8_t data);
     void clock();
     void reset();
     void connectGamePak(GamePak *g);
};


#endif //NES_BUS_H
