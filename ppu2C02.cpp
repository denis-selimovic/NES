//
// Created by denis on 24/02/2020.
//

#include "ppu2C02.h"

uint8_t ppu2C02::readCPUMemory(uint16_t address) {
    return 0;
}

void ppu2C02::writeCPUMemory(uint16_t address, uint8_t data) {

}

uint8_t ppu2C02::readPPUMemory(uint16_t address) {
    //mapiramo adresu na opseg 0x0000 do 0x3fff
    address &= 0x3FFFu;
    if(address >= 0x0000 && address <= 0x1FFF) return pattern_table[(address & 0x1000u) >> 12u][address & 0x0FFFu];
    else if(address >= 0x2000 && address <= 0x3EFF) {
        address &= 0xFFFu;
        //ako se koristi vertikalni mirroring prvi i treći nametable su isti, kao i drugi i četvrti
        //ako se koristi horizontalni mirroring prvi i drugi nametable su isti i mapiraju se u isti adresni prostor, kao i treći i četvrti
        if(gamepak->mirroring == GamePak::MIRRORING::VERTICAL) {
            if((address >= 0x0000 && address <= 0x03FF) || (address >= 0x0800 && address <= 0x0BFF)) return nametable[0][address & 0x03FFu];
            else if ((address >= 0x0400 && address <= 0x07FF) || (address >= 0x0C00 && address <= 0x0FFF))return nametable[1][address & 0x03FFu];
        }
        else if(gamepak->mirroring == GamePak::MIRRORING::HORIZONTAL) {
            if(address >= 0x0000 && address <= 0x07FF) return nametable[0][address & 0x03FFu];
            else if(address >= 0x0800 && address <= 0x0FFF) return nametable[1][address & 0x03FFu];
        }
    }
    else if(address >= 0x3F00 && address <= 0x3FFF) {
        //adrese od 0x3F00 do 0x3F1F se preslikavaju u ostale adrese
        //adresu svodimo na opseg od 0x0000 do 0x001F da bi pristupili nizu
        address &= 0x001Fu;
        //parovi koji se preslikavaju su 0x3f10->0x3f00, 0x3f14->0x3f04, 0x3f18->0x3f08, 0x3f1c->0x3f0c
        if (address == 0x0010) address = 0x0000;
        else if (address == 0x0014) address = 0x0004;
        else if (address == 0x0018) address = 0x0008;
        else if (address == 0x001C) address = 0x000C;
        return pallete[address] & unsigned(ppumask.bits.grayscale ? 0x30 : 0x3F);
    }
    return 0;
}

void ppu2C02::writePPUMemory(uint16_t address, uint8_t data) {
    //mapiramo adresu na opseg 0x0000 do 0x3fff
    address &= 0x3FFFu;
    if(address >= 0x0000 && address <= 0x1FFF) pattern_table[(address & 0x1000u) >> 12u][address & 0x0FFFu] = data;
    else if(address >= 0x2000 && address <= 0x3EFF) {
        address &= 0xFFFu;
        //ako se koristi vertikalni mirroring prvi i treći nametable su isti, kao i drugi i četvrti
        //ako se koristi horizontalni mirroring prvi i drugi nametable su isti i mapiraju se u isti adresni prostor, kao i treći i četvrti
        if(gamepak->mirroring == GamePak::MIRRORING::VERTICAL) {
            if((address >= 0x0000 && address <= 0x03FF) || (address >= 0x0800 && address <= 0x0BFF)) nametable[0][address & 0x03FFu] = data;
            else if ((address >= 0x0400 && address <= 0x07FF) || (address >= 0x0C00 && address <= 0x0FFF)) nametable[1][address & 0x03FFu] = data;
        }
        else if(gamepak->mirroring == GamePak::MIRRORING::HORIZONTAL) {
            if(address >= 0x0000 && address <= 0x07FF) nametable[0][address & 0x03FFu] = data;
            else if(address >= 0x0800 && address <= 0x0FFF) nametable[1][address & 0x03FFu] = data;
        }
    }
    else if(address >= 0x3F00 && address <= 0x3FFF) {
        //adrese od 0x3F00 do 0x3F1F se preslikavaju u ostale adrese
        //adresu svodimo na opseg od 0x0000 do 0x001F da bi pristupili nizu
        address &= 0x001Fu;
        //parovi koji se preslikavaju su 0x3f10->0x3f00, 0x3f14->0x3f04, 0x3f18->0x3f08, 0x3f1c->0x3f0c
        if (address == 0x0010) address = 0x0000;
        else if (address == 0x0014) address = 0x0004;
        else if (address == 0x0018) address = 0x0008;
        else if (address == 0x001C) address = 0x000C;
        pallete[address] = data;
    }
}

void ppu2C02::connectGamePak(GamePak *g){
    this->gamepak = g;
}
