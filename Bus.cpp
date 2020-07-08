//
// Created by denis on 22/02/2020.
//

#include <iostream>
#include "Bus.h"

uint8_t Bus::readCPUMemory(uint16_t address) {
    uint8_t data = 0x00;
    if(gamePak->readCPUMemory(address, data)) {}
    else if(address >= 0x0000 && address <= 0x1FFF) return RAM[address & 0x07FFu];
    else if(address >= 0x2000 && address <= 0x3FFF) return ppu.readCPUMemory(address & 0x0007u);
    else if (address >= 0x4016 && address <= 0x4017) {
        data = (joystick[address & 0x01u] & 0x80u) > 0;
        joystick[address & 0x01u] <<= 1u;
    }
    return data;
}

void Bus::writeCPUMemory(uint16_t address, uint8_t data) {
    if(gamePak->writeCPUMemory(address, data)) {}
    else if(address >= 0x0000 && address <= 0x1FFF) RAM[address & 0x07FFu] = data;
    else if (address >= 0x2000 && address <= 0x3FFF) ppu.writeCPUMemory(address & 0x0007u, data);
    else if (address == 0x4014) DMA = {data, 0x00, 0x00, true, true};
    else if (address >= 0x4016 && address <= 0x4017) joystick[address & 0x01u] = joystickBuffer[address & 0x01u];
}

Bus::Bus(cpu6502 &cpu, ppu2C02 &ppu) : cpu(cpu), ppu(ppu) {
    this->cpu.connectToBus(this);
}

Bus::~Bus() {
    delete gamePak;
    gamePak = nullptr;
}

void Bus::clock() {
    //svaki signal sata pozivamo funkciju clock iz klase ppu
    //jedan ciklus sata cpu se dešava na svaka tri ciklusa sata ppu
    ppu.clock();
    if(cycles % 3 == 0) {
        if(DMA.transfer) {
            if(DMA.disable && cycles % 2 == 1) DMA.disable = false;
            else if(!DMA.disable) startDMA();
        }
        else cpu.clock();
    }
    if(ppu.interrupt) {
        cpu.nonmaskableInterrupt();
        ppu.interrupt = false;
    }
    cycles++;
}

void Bus::reset() {
    //resetujemo cpu i ppu
    ppu.reset();
    cpu.reset();
    gamePak->reset();
    cycles = 0;
    DMA = {0x00, 0x00, 0x00, false, true};
}

void Bus::connectGamePak(GamePak *g) {
    gamePak = g;
    ppu.connectGamePak(g);
}

void Bus::startDMA() {
    (cycles % 2 == 0) ? readDMA() : writeDMA();
}

void Bus::writeDMA() {
    ppu.oam_memory[DMA.address] = DMA.data;
    DMA.address++;
    if(DMA.address == 0x00) {
        DMA.transfer = false;
        DMA.disable = true;
    }
}

void Bus::readDMA() {
    DMA.data = readCPUMemory((DMA.page << 8u) | DMA.address);
}



