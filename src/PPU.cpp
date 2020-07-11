//
// Created by denis on 24/02/2020.
//

#include <cstring>
#include "../include/PPU.h"

uint8_t PPU::readCPUMemory(uint16_t address) {
    uint8_t data = 0x00;
    switch(address) {
        case 0x0000:
            break;
        case 0x0001:
            //PPUCTRL i PPUMASK su write-only registri
            break;
        case 0x0002:
            //čitanje iz ovog registra utiče i na ostale registre
            //postavlja se toggle na 0
            //takođe vblank se postavlja na 0

            //samo najznačanija tri bita su validna
            data = (ppustatus.reg & 0xE0u) | (ppudata_buffer & 0x1Fu);
            toggle = 0;
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
            break;
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

void PPU::writeCPUMemory(uint16_t address, uint8_t data) {
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
                t_address.coarse_x = (data >> 3u);
                // postavimo toggle na 1;
                toggle = 1;
            }
            else {
                //postavimo fine_y i coarse_y
                //vratimo toggle na 0
                t_address.fine_y = data & 0x07u;
                t_address.coarse_y = (data >> 3u);
                toggle = 0;
            }
            break;
        case 0x0006:
            //t_address koritimo kao buffer za vram_address
            //upis se vrši iz dva dijela
            //kad je latch u nuli upišemo high byte
            //nakon toga upišemo low byte
            if(!toggle) {
                //adresu svodimo na opseg 0x3F i generišemo high byte
                t_address.reg = (uint16_t)((data & 0x3Fu) << 8u) | (t_address.reg & 0x00FFu);
                toggle = 1;
            }
            else {
                //sada dodajemo low byte
                t_address.reg = (t_address.reg & 0xFF00u) | data;
                vram_address = t_address;
                toggle = 0;
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

uint8_t PPU::readPPUMemory(uint16_t address) {
    //mapiramo adresu na opseg 0x0000 do 0x3fff
    uint8_t data = 0x00;
    address &= 0x3FFFu;
    if(gamePak->readPPUMemory(address, data)) {}
    else if(address >= 0x0000 && address <= 0x1FFF) return patternTable[(address & 0x1000u) >> 12u][address & 0x0FFFu];
    else if(address >= 0x2000 && address <= 0x3EFF) {
        address &= 0x0FFFu;
        //ako se koristi vertikalni mirroring prvi i treći nametable su isti, kao i drugi i četvrti
        //ako se koristi horizontalni mirroring prvi i drugi nametable su isti i mapiraju se u isti adresni prostor, kao i treći i četvrti
        if(gamePak->mirroring == GamePak::MIRRORING::VERTICAL) {
            if((address >= 0x0000 && address <= 0x03FF) || (address >= 0x0800 && address <= 0x0BFF)) return nameTable[0][address & 0x03FFu];
            else if ((address >= 0x0400 && address <= 0x07FF) || (address >= 0x0C00 && address <= 0x0FFF))return nameTable[1][address & 0x03FFu];
        }
        else if(gamePak->mirroring == GamePak::MIRRORING::HORIZONTAL) {
            if(address >= 0x0000 && address <= 0x07FF) return nameTable[0][address & 0x03FFu];
            else if(address >= 0x0800 && address <= 0x0FFF) return nameTable[1][address & 0x03FFu];
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
        return palette[address] & (ppumask.grayscale ? 0x30 : 0x3F);
    }
    return data;
}

void PPU::writePPUMemory(uint16_t address, uint8_t data) {
    //mapiramo adresu na opseg 0x0000 do 0x3fff
    address &= 0x3FFFu;
    if(gamePak->writePPUMemory(address, data)) {}
    else if(address >= 0x0000 && address <= 0x1FFF) patternTable[(address & 0x1000u) >> 12u][address & 0x0FFFu] = data;
    else if(address >= 0x2000 && address <= 0x3EFF) {
        address &= 0x0FFFu;
        //ako se koristi vertikalni mirroring prvi i treći nametable su isti, kao i drugi i četvrti
        //ako se koristi horizontalni mirroring prvi i drugi nametable su isti i mapiraju se u isti adresni prostor, kao i treći i četvrti
        if(gamePak->mirroring == GamePak::MIRRORING::VERTICAL) {
            if((address >= 0x0000 && address <= 0x03FF) || (address >= 0x0800 && address <= 0x0BFF)) nameTable[0][address & 0x03FFu] = data;
            else if ((address >= 0x0400 && address <= 0x07FF) || (address >= 0x0C00 && address <= 0x0FFF)) nameTable[1][address & 0x03FFu] = data;
        }
        else if(gamePak->mirroring == GamePak::MIRRORING::HORIZONTAL) {
            if(address >= 0x0000 && address <= 0x07FF) nameTable[0][address & 0x03FFu] = data;
            else if(address >= 0x0800 && address <= 0x0FFF) nameTable[1][address & 0x03FFu] = data;
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
        palette[address] = data;
    }
}

void PPU::connectGamePak(GamePak *gamePak){
    this->gamePak = gamePak;
}

void PPU::clock() {
    if(scanline >= -1 && scanline < 240) {
        if(scanline == 0 && cycles == 0) cycles = 1;
        if(scanline == -1 && cycles == 1) {
            ppustatus.vblank = 0;
            ppustatus.sprite_overflow = 0;
            ppustatus.sprite_zero_hit = 0;
            std::fill_n(sprite_low, 8, 0);
            std::fill_n(sprite_high, 8, 0);
        }
        if((cycles >= 2 && cycles < 258) || (cycles >= 321 && cycles < 338)) {
            updateShiftRegister();
            uint8_t tile_selector = (cycles - 1) % 8;
            fetchNextTile(tile_selector);
        }
        if(cycles == 256) scrollingVertical();
        if(cycles == 257) {
            loadPixel();
            transferHorizontal();
        }
        if(cycles == 338 || cycles == 340) tile.id = readPPUMemory(0x2000u | (vram_address.reg && 0x0FFFu));
        if(scanline == -1 && cycles >= 280 && cycles < 305) transferVertical();
        if(cycles == 257 && scanline >= 0) {
            std::memset(foundSprites, 0xFF, 8 * sizeof(Sprite));
            sprite_count = 0;
            std::fill_n(sprite_low, 8, 0);
            std::fill_n(sprite_high, 8, 0);
            findSprites();
        }
        if(cycles == 340) {
            for(uint i = 0; i < sprite_count; ++i) {
                uint16_t sprite_address_low, sprite_address_high;
                if(ppuctrl.sprite_height) {
                    if(!(foundSprites[i].attributes & 0x80u)) sprite_address_low = sprite8x16(i);
                    else sprite_address_low = sprite8x16Flipped(i);
                }
                else {
                    if(!(foundSprites[i].attributes & 0x80u)) sprite_address_low = sprite8x8(i);
                    else sprite_address_low = sprite8x8Flipped(i);
                }
                sprite_address_high = sprite_address_low + 8;
                uint8_t sprite_low_bytes = readPPUMemory(sprite_address_low), sprite_high_bytes = readPPUMemory(sprite_address_high);
                if(foundSprites[i].attributes & 0x40u) {
                    sprite_low_bytes = flipBytes(sprite_low_bytes);
                    sprite_high_bytes = flipBytes(sprite_high_bytes);
                }
                sprite_low[i] = sprite_low_bytes;
                sprite_high[i] = sprite_high_bytes;
            }

        }
    }
    if(scanline >= 241 && scanline < 261) {
        if(scanline == 241 && cycles == 1) {
            ppustatus.vblank = 1;
            if(ppuctrl.nmi_enable) interrupt = true;
        }
    }

    Palette palette{};
    SpritePalette spritePalette{};
    if(ppumask.background_enable && (ppumask.background_left_column_enable || cycles >= 9)) palette = getComposition();
    if(ppumask.sprite_enable && (ppumask.sprite_left_column_enable || cycles >= 9)) spritePalette = getSpriteComposition();
    Pixel pixel = getColor(getFinalComposition(palette, spritePalette));
    if(256 * scanline + cycles - 1 >= 0 && 256 * scanline + cycles - 1 < 256 * 240) pixels[256 * scanline + (cycles - 1)] = getColorCode(pixel);

    cycles++;
    if(cycles >= 341) {
        cycles = 0;
        scanline++;
        if(scanline >= 261) {
            rendered = true;
            scanline = -1;
        }
    }
}

PPU::~PPU() {
    delete[] pixels;
}

void PPU::reset() {
    ppustatus.reg = 0x00;
    ppumask.reg = 0x00;
    ppuctrl.reg = 0x00;
    vram_address.reg = 0x0000;
    t_address.reg = 0x0000;
    ppudata_buffer = 0x00;
    toggle = false;
    fine_x = 0x00;
    cycles = 0;
    tile.reset();
    pattern.reset();
    attribute.reset();
}

void PPU::scrollingHorizontal() {
    // Ako su biti za rendering pozadine ili sprite-a postavljeni može se izvršiti scroll horizontalno
    if(ppumask.background_enable || ppumask.sprite_enable) {
        // prilikom skrolanja možemo preći u sljedeći nametable
        // ukoliko se to desi moramo ažurirati bit koji označava sljedeći nametable
        // u suprotnom samo povećamo adresu sa koje se čita sljedeći bajt
        // jedan nametable ima 32x30 pločica, nakon što naiđemo na 31. pločicu u redu prelazimo na sljedeći nametable
        vram_address.nametable_select_x = (vram_address.coarse_x != 31) ? vram_address.nametable_select_x : ~vram_address.nametable_select_x;
        vram_address.coarse_x = (vram_address.coarse_x + 1) % 32;
    }
}

void PPU::scrollingVertical() {
    // scroll vertikalno je isto moguć samo kad su biti za rendering omogućeni
    if(ppumask.background_enable || ppumask.sprite_enable) {
        // scroll vertikalno ne ide na svakih 8 piksela (kolika je visina jedne pločice)
        // moguć je scroll za određeni broj piksela, zato koristimo fine_y varijablu
        // pored toga, kako nametable ima 32x32 pločica, a vidljivo je samo 32x30 posljednja dva reda traže poseban tretman
        if(vram_address.fine_y < 7) {
            vram_address.fine_y++;
        }
        else {
            vram_address.fine_y = 0;
            if(vram_address.coarse_y == 29 || vram_address.coarse_y == 31) {
                vram_address.coarse_y = 0;
                if(vram_address.coarse_y == 29) vram_address.nametable_select_y = ~vram_address.nametable_select_y;
            }
            else {
                vram_address.coarse_y++;
            }
        }
    }
}

void PPU::transferHorizontal() {
    if(ppumask.sprite_enable || ppumask.background_enable) {
        vram_address.coarse_x = t_address.coarse_x;
        vram_address.nametable_select_x = t_address.nametable_select_x;
    }

}

void PPU::transferVertical() {
    if(ppumask.sprite_enable || ppumask.background_enable) {
        vram_address.coarse_y = t_address.coarse_y;
        vram_address.nametable_select_y = t_address.nametable_select_y;
        vram_address.fine_y = t_address.fine_y;
    }
}

void PPU::loadPixel() {
    pattern.pack(tile.lsb, tile.msb);
    attribute.pack(tile.getPackingByte(0b01u), tile.getPackingByte(0b10u));
}

void PPU::updateShiftRegister() {
    if(ppumask.background_enable) {
        pattern.shift();
        attribute.shift();
    }
    if(ppumask.sprite_enable && cycles >= 1 && cycles < 258) {
        for(int i = 0; i < sprite_count; ++i) {
            if(foundSprites[i].index > 0) foundSprites[i].index--;
            else {
                sprite_low[i] <<= 1u;
                sprite_high[i] <<= 1u;
            }
        }
    }
}

void PPU::fetchNextTile(uint8_t selector) {
    uint16_t address = 0x0000, lsb_address = 0x0000, msb_address = 0x0000;
    switch (selector) {
        case 0:
            loadPixel();
            tile.id = readPPUMemory(0x2000u | (vram_address.reg & 0x0FFFu));
            break;
        case 2:
            address = 0x23C0u | (vram_address.nametable_select_y << 11u) | (vram_address.nametable_select_x << 10u)
                    | ((vram_address.coarse_y >> 2u) << 3u) | (vram_address.coarse_x >> 2u);
            tile.attribute = readPPUMemory(address);
            if(vram_address.coarse_y & 0x02u) tile.attribute >>= 4u;
            if(vram_address.coarse_x & 0x02u) tile.attribute >>= 2u;
            tile.attribute &= 0x03u;
            break;
        case 4:
            lsb_address = (ppuctrl.background_tile_select << 12u) + (uint16_t(tile.id) << 4u) + vram_address.fine_y;
            tile.lsb = readPPUMemory(lsb_address);
            break;
        case 6:
            msb_address = (ppuctrl.background_tile_select << 12u) + (uint16_t(tile.id) << 4u) + vram_address.fine_y + 8;
            tile.msb = readPPUMemory(msb_address);
            break;
        case 7:
            scrollingHorizontal();
            break;
        default:
            break;
    }
}

PPU::Palette PPU::getComposition() {
    uint16_t selector = (0x8000u >> fine_x);
    uint8_t pixel_id = (((pattern.getHighByte() & selector) > 0) << 1u) | ((pattern.getLowByte() & selector) > 0);
    uint8_t palette_id = (((attribute.getHighByte() & selector) > 0) << 1u) | ((attribute.getLowByte() & selector) > 0);
    return {pixel_id, palette_id};
}

PPU::SpritePalette PPU::getSpriteComposition() {
    spriteZero.rendered = false;
    SpritePalette spritePalette{};
    for(uint i = 0; i < sprite_count; ++i) {
        if(foundSprites[i].index == 0) {
            spritePalette.pixel_id = (((sprite_high[i] & 0x80u) > 0) << 1u) | ((sprite_low[i] & 0x80u) > 0);
            spritePalette.palette_id = (foundSprites[i].attributes & 0x03u) + 0x04;
            spritePalette.priority = ((foundSprites[i].attributes & 0x20u) == 0);
            if(spritePalette.pixel_id != 0) {
                if(i == 0) spriteZero.rendered = true;
                break;
            }
        }
    }
    return spritePalette;
}

PPU::FinalPalette PPU::getFinalComposition(PPU::Palette palette, PPU::SpritePalette spritePalette) {
    FinalPalette finalPalette{};
    if(palette.pixel_id == 0 && spritePalette.pixel_id == 0) finalPalette = {0x00, 0x00};
    else if(palette.pixel_id > 0 && spritePalette.pixel_id == 0) finalPalette = {palette.pixel_id, palette.palette_id};
    else if(palette.pixel_id == 0 && spritePalette.pixel_id > 0) finalPalette = {spritePalette.pixel_id, spritePalette.palette_id};
    else if (palette.pixel_id > 0 && spritePalette.pixel_id > 0) {
        if(spritePalette.priority) finalPalette = {spritePalette.pixel_id, spritePalette.palette_id};
        else finalPalette = {palette.pixel_id, palette.palette_id};
        if(spriteZero.enabled && spriteZero.rendered && (ppumask.sprite_enable & ppumask.background_enable)) {
            if(~(ppumask.background_left_column_enable | ppumask.sprite_left_column_enable)) {
                if(cycles >= 9 && cycles < 258) ppustatus.sprite_zero_hit = 1;
            }
            else {
                if(cycles >= 1 && cycles < 258) ppustatus.sprite_zero_hit = 1;
            }
        }
    }
    return finalPalette;
}

void PPU::findSprites() {
    spriteZero.enabled = false;
    for(int i = 0; i < 64 && sprite_count <= 8; ++i) {
        int32_t diff = int16_t(scanline) - int16_t(OAM[i].yPosition);
        if (diff >= 0 && diff < (ppuctrl.sprite_height ? 16 : 8)) {
            if(sprite_count < 8) {
                if(i == 0) spriteZero.enabled = true;
                //foundSprites[sprite_count] = OAM[i];
                memcpy(&foundSprites[sprite_count], &OAM[i], sizeof(Sprite));
                sprite_count++;
            }
        }
    }
    ppustatus.sprite_overflow = (sprite_count >= 8);
}

uint16_t PPU::sprite8x8(uint8_t i) {
    return (ppuctrl.sprite_tile_select << 12u) | (foundSprites[i].tileIndex << 4u) | (scanline - foundSprites[i].yPosition);
}

uint16_t PPU::sprite8x8Flipped(uint8_t i) {
    return (ppuctrl.sprite_tile_select << 12u) | (foundSprites[i].tileIndex << 4u) |  (7 - (scanline - foundSprites[i].yPosition));
}

uint16_t PPU::sprite8x16(uint8_t i) {
    return (scanline - foundSprites[i].yPosition < 8) ? sprite8x16Helper(i, 0) : sprite8x16Helper(i, 1);
}

uint16_t PPU::sprite8x16Flipped(uint8_t i) {
    return (scanline - foundSprites[i].yPosition < 8) ? sprite8x16FlippedHelper(i, 1) : sprite8x16FlippedHelper(i, 0);
}

uint16_t PPU::sprite8x16Helper(uint8_t i, uint8_t temp) {
    return ((foundSprites[i].tileIndex & 0x01) << 12u) | (((foundSprites[i].tileIndex & 0xFEu) + temp) << 4u) | ((scanline - foundSprites[i].yPosition) & 0x07u);
}

uint16_t PPU::sprite8x16FlippedHelper(uint8_t i, uint8_t temp) {
    return ((foundSprites[i].tileIndex & 0x01) << 12u) | (((foundSprites[i].tileIndex & 0xFEu) + temp) << 4u) |  ((7 - (scanline - foundSprites[i].yPosition)) & 0x07u);
}

uint8_t PPU::flipBytes(uint8_t bytes) {
    bytes = (bytes & 0xF0) >> 4 | (bytes & 0x0F) << 4;
    bytes = (bytes & 0xCC) >> 2 | (bytes & 0x33) << 2;
    bytes = (bytes & 0xAA) >> 1 | (bytes & 0x55) << 1;
    return bytes;
}

PPU::PPU() {
    ppuPalette.push_back({84,84, 84});
    ppuPalette.push_back({0, 30, 116});
    ppuPalette.push_back({8, 16, 144});
    ppuPalette.push_back({48, 0, 136});
    ppuPalette.push_back({68, 0, 100});
    ppuPalette.push_back({92, 0, 48});
    ppuPalette.push_back({84, 4, 0});
    ppuPalette.push_back({60, 24, 0});
    ppuPalette.push_back({32, 42, 0});
    ppuPalette.push_back({8, 58, 0});
    ppuPalette.push_back({0, 64, 0});
    ppuPalette.push_back({0, 60, 0});
    ppuPalette.push_back({0, 50, 60});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({152, 150, 152});
    ppuPalette.push_back({8, 76, 196});
    ppuPalette.push_back({48, 50, 236});
    ppuPalette.push_back({92, 30, 228});
    ppuPalette.push_back({136, 20, 176});
    ppuPalette.push_back({160, 20, 100});
    ppuPalette.push_back({152, 34, 32});
    ppuPalette.push_back({120, 60, 0});
    ppuPalette.push_back({84, 90, 0});
    ppuPalette.push_back({40, 114, 0});
    ppuPalette.push_back({8, 124, 0});
    ppuPalette.push_back({0, 118, 40});
    ppuPalette.push_back({0, 102, 120});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({236, 238, 236});
    ppuPalette.push_back({76, 154, 236});
    ppuPalette.push_back({120, 124, 236});
    ppuPalette.push_back({176, 98, 236});
    ppuPalette.push_back({228, 84, 236});
    ppuPalette.push_back({236, 88, 180});
    ppuPalette.push_back({236, 106, 100});
    ppuPalette.push_back({212, 136, 32});
    ppuPalette.push_back({160, 170, 0});
    ppuPalette.push_back({116, 196, 0});
    ppuPalette.push_back({76, 208, 32});
    ppuPalette.push_back({56, 204, 108});
    ppuPalette.push_back({56, 180, 204});
    ppuPalette.push_back({60, 60, 60});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({236, 238, 236});
    ppuPalette.push_back({168, 204, 236});
    ppuPalette.push_back({188, 188, 236});
    ppuPalette.push_back({212, 178, 236});
    ppuPalette.push_back({236, 174, 236});
    ppuPalette.push_back({236, 174, 212});
    ppuPalette.push_back({236, 180, 176});
    ppuPalette.push_back({228, 196, 144});
    ppuPalette.push_back({204, 210, 120});
    ppuPalette.push_back({180, 222, 120});
    ppuPalette.push_back({168, 226, 144});
    ppuPalette.push_back({152, 226, 180});
    ppuPalette.push_back({160, 214, 228});
    ppuPalette.push_back({160, 162, 160});
    ppuPalette.push_back({0, 0, 0});
    ppuPalette.push_back({0, 0, 0});
}

PPU::Pixel PPU::getColor(PPU::FinalPalette palette) {
    return ppuPalette[readPPUMemory(0x3F00u + (palette.palette_id << 2u) + palette.pixel_id) & 0x3Fu];
}

unsigned int PPU::getColorCode(PPU::Pixel pixel) {
    return (0xff << 24u) | ((pixel.r & 0xffu) << 16u) | ((pixel.g & 0xffu) << 8u) | (pixel.b & 0xffu);
}



