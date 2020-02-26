//
// Created by denis on 24/02/2020.
//

#include "ppu2C02.h"

uint8_t ppu2C02::readCPUMemory(uint16_t address) {
    uint8_t data = 0x00;
    switch(address) {
        case 0x0000:
        case 0x0001:
            //PPUCTRL i PPUMASK su write-only registri
            break;
        case 0x0002:
            //čitanje iz ovog registra utiče i na ostale registre
            //postavlja se toggle na 0
            //takođe vblank se postavlja na 0

            //samo najznačanija tri bita su validna
            data = ppustatus.reg & 0xE0u;
            toggle = false;
            ppustatus.vblank = 0;
            break;
        case 0x0003:
            //OAM_ADDRESS nije readable
            break;
        case 0x0004:
            //pristup oam memoriji je moguć uvijek
            data = oam_memory[address];
            break;
        case 0x0005:
        case 0x0006:
            //PPUSCROLL i VRAM_ADDRESS su write-only
            break;
        case 0x0007:
            //koristimo buffer od ranije da vratimo podatke koji su pročitani u prethodnom ciklusu
            data = ppudata_buffer;
            //sada počinje sljedeći ciklus čitanja
            ppudata_buffer = readPPUMemory(vram_address.reg);

            //ako je adresa >= 0x3F00 onda se radi o čitanju paleta
            //ovo je moguće uraditi bez čekanja i u tom slučaju se podaci vraćaju odmah
            if(vram_address.reg >= 0x3F00) data = ppudata_buffer;

            //na kraju inkrementiramo adresu
            vram_address.reg += (ppuctrl.increment_mode ? 32 : 1);
            break;
        default:
            break;
    }
    return data;
}

void ppu2C02::writeCPUMemory(uint16_t address, uint8_t data) {
    switch(address) {
        case 0x0000:
            //upis u ovaj registar je moguć
            //upis u ovaj registar vrši upis i u PPUSCROLL registar - t_address pomoćna varijabla
            ppuctrl.reg = data;
            t_address.nametable_select_x = ppuctrl.nametable_select_1;
            t_address.nametable_select_y = ppuctrl.nametable_select_2;
            break;
        case 0x0001:
            //upis u mask registar je isto moguć
            ppumask.reg = data;
            break;
        case 0x0002:
            //upis u ovaj registar nije moguć
            break;
        case 0x0003:
            //upis adrese za OAM
            OAM_address = data;
            break;
        case 0x0004:
            //upis u OAM preko sadržaja četvrtog registra
            //ua upis koristimo pokazivač na tu memoriju
            oam_memory[OAM_address] = data;
            break;
        case 0x0005:
            //upis u PPUSCROLL ide u dvije faze
            //prvo vršimo upis u X scroll, pa onda u Y scroll
            //ovo se određuje na osnovu latch varijable - toggle
            if(!toggle) {
                //prvo postavimo vrijednost fine_x i coarse_x
                fine_x = data & 0x07u;
                t_address.coarse_x = data >> 3u;
                // postavimo toggle na 1;
                toggle = true;
            }
            else {
                //postavimo fine_y i coarse_y
                //vratimo toggle na 0
                t_address.fine_y = data & 0x07u;
                t_address.coarse_y = data >> 3u;
                toggle = false;
            }
            break;
        case 0x0006:
            //t_address koritimo kao buffer za vram_address
            //upis se vrši iz dva dijela
            //kad je latch u nuli upišemo high byte
            //nakon toga upišemo low byte
            if(!toggle) {
                //adresu svodimo na opseg 0x3F i generišemo high byte
                t_address.reg = (uint16_t)(((data & 0x3Fu) << 8u) | (t_address.reg & 0x00FFu));
                toggle = true;
            }
            else {
                //sada dodajemo low byte
                t_address.reg = (t_address.reg & 0xFF00u) | data;
                vram_address = t_address;
                toggle = false;
            }
            break;
        case 0x0007:
            //upis u VRAM
            //koristmo writePPUMemory funkciju
            //inkrementiramo adresu
            writePPUMemory(vram_address.reg, data);
            //adresu inkrementiramo na osnovu PPUCTRL regista za 1 ili 32
            vram_address.reg += (ppuctrl.increment_mode ? 32 : 1);
            break;
        default:
            break;
    }
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
        return pallete[address] & unsigned(ppumask.grayscale ? 0x30 : 0x3F);
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

void ppu2C02::clock() {

}

void ppu2C02::reset() {

}
