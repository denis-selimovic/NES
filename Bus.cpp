//
// Created by denis on 22/02/2020.
//

#include "Bus.h"

uint8_t Bus::readCPUMemory(uint16_t address) {
    uint8_t data = 0x00;
    if(gamePak->readCPUMemory(address, data)) {}
    else if(address >= 0x0000 && address <= 0x1FFF) return RAM[address & 0x07FFu];
    else if(address >= 0x2000 && address <= 0x3FFF) return ppu.readCPUMemory(address & 0x0007u);
    return data;
}

void Bus::writeCPUMemory(uint16_t address, uint8_t data) {
    if(gamePak->writeCPUMemory(address, data)) {}
    else if(address >= 0x0000 && address <= 0x01FF) RAM[address & 0x07FFu] = data;
    else if (address >= 0x2000 && address <= 0x3FFF) ppu.writeCPUMemory(address & 0x0007u, data);
}

Bus::Bus(cpu6502 &cpu, ppu2C02 &ppu) : cpu(cpu), ppu(ppu) {
    this->cpu.connectToBus(this);
}

void Bus::clock() {
    //svaki signal sata pozivamo funkciju clock iz klase ppu
    //jedan ciklus sata cpu se dešava na svaka tri ciklusa sata ppu
    ppu.clock();
    if(cycles % 3 == 0) cpu.clock();
    cycles++;
}

void Bus::reset() {
    //resetujemo cpu i ppu
    ppu.reset();
    cpu.reset();
    cycles = 0;
}

void Bus::connectGamepak(GamePak *g) {
    gamePak = g;
}

