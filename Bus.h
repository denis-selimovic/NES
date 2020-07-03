//
// Created by denis on 22/02/2020.
//

#ifndef NES_BUS_H
#define NES_BUS_H

#include <cstdint>
#include <array>
#include "ppu2C02.h"
#include "cpu6502.h"

class Bus {

    //povezani na sabirnicu su RAM, ppu i cpu
    std::array<uint8_t, 2048> RAM{};
    ppu2C02 &ppu;
    cpu6502 &cpu;

    //broj ciklusa sabirnice
    uint32_t cycles = 0;

    struct DirectMemoryAccess {
        uint8_t page = 0x00, address = 0x00, data = 0x00;
        bool transfer = false, disable = true;
    } DMA;
    void startDMA();
    void readDMA();
    void writeDMA();

    //pokazivač na gamepak
    GamePak *gamePak = nullptr;
    friend class Debugger;
public:
     Bus(cpu6502 &cpu, ppu2C02 &ppu);
     uint8_t readCPUMemory(uint16_t address);
     void writeCPUMemory(uint16_t address, uint8_t data);
     void clock();
     void reset();
     void connectGamepak(GamePak *g);
};


#endif //NES_BUS_H
