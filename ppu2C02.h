//
// Created by denis on 24/02/2020.
//

#ifndef NES_PPU2C02_H
#define NES_PPU2C02_H


#include <cstdint>
#include "GamePak.h"

class ppu2C02 {

    //broj ciklusa sata
    uint8_t cycles = 0;

    //Memorijska mapa ppu
    //ppu ima svoj memorijski prostor odvojen od cpu
    //2KB RAM-a je odvojeno za PPU na NES platformi

    //na adresama 0x0000 do 0x1FFF je za dvije tabele paterna
    uint8_t pattern_table[2][4096];
    //na adresama 0x2000 do 0x3EFF se nalaze 4 tabele imena
    //pošto NES ima samo 2KB VRAM-a 4 tabele se koristeći mirroring pamte kao dvije tabele
    uint8_t nametable[2][1024];
    //na adresama 0x3F00 do 0x3FFF su palete
    //i ovdje se koristi mirroring
    uint8_t pallete[32];


    //PPU registri

    //prvi registar je PPUCTRL koji sadrži 8 bita
    //koristimo union da bi mogli individualno da postavljamo bite
    //i da vratimo sadržaj čitavog registra odjednom
    union PPUCTRL {
        struct {
            uint8_t nametable_select_1 : 1;
            uint8_t nametable_select_2 : 1;
            uint8_t increment_mode : 1;
            uint8_t sprite_tile_select : 1;
            uint8_t background_tile_select : 1;
            uint8_t sprite_height : 1;
            uint8_t ppu_master_slave : 1;
            uint8_t nmi_enable : 1;
        };
        uint8_t reg;
    } ppuctrl;

    //drugi registar je PPUMASK
    union PPUMASK {
        struct {
            uint8_t grayscale : 1;
            uint8_t background_left_column_enable : 1;
            uint8_t sprite_left_column_enable : 1;
            uint8_t background_enable : 1;
            uint8_t sprite_enable : 1;
            uint8_t red_emphasis : 1;
            uint8_t green_emphasis : 1;
            uint8_t blue_emphasis : 1;
        };
        uint8_t reg;
    } ppumask;

    //treći registar je PPUSTATUS
    union PPUSTATUS {
        struct {
            uint8_t U : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vblank : 1;
        };
        uint8_t reg;
    } ppustatus;

    //četvrti registar čuva adresu pomoću koje se pristupa OAM-u
    uint8_t OAM_address;
    //OAM je object attribute memory koja čuva informacije o spirte-ovima
    //interna memorija koja čuva do 64 spirte-a svaki sprite se sastoji od 4 bajta
    //peti registar neće biti implementiran
    //njemu pristupamo preko adrese koja je u OAM_address
    struct Sprite{
        uint8_t y_position;
        uint8_t tile_index;
        uint8_t attributes;
        uint8_t index;
    } OAM[64];


    //šesti registar je PPUSCROLL, a sedmi je adresa u VRAM-u
    //osmi registar neće biti implementiran jer njemu pristupamo preko adrese u VRAM-u
    //osmi registar je sličan petom registru
    //template za šesti i sedmi registar je dat ispod
    union PPUADDRESS {
        struct {
            uint16_t coarse_x : 5;
            uint16_t coarse_y : 5;
            uint16_t nametable_select_x : 1;
            uint16_t nametable_select_y : 1;
            uint16_t fine_y : 3;
            uint16_t unused : 1;
        };
        uint16_t reg;
    };

    //vram address predstavlja sadržaj sedmog registra
    //t_address je pomoćna varijabla preko koje vršimo upis u šesti i sedmi registar
    //ova dva registra koriste isti buffer za upis podataka
    //zbog ovoga prvo moramo pisati u VRAM pa onda mijenjati PPUSCROLL
    PPUADDRESS vram_address, t_address;

    //tri bita za PPUSCROLL
    uint8_t fine_x = 0x00;

    //oba registra koriste isti latch za upis
    //upis u ova dva registra se odvija u dvije faze
    //služi kao latch za upis u PPUSCROLL i VRAMADDRESS
    uint8_t toggle = 0x00;

    //buffer za čitanje osmog registra PPU
    //čitanje iz ovog registra kasni jedan ciklus ako je adresa < 0x3F00
    //zbog toga prethodno pročitanu vrijednost čuvamo u bufferu
    //ako je adresa >= 0x3F00 buffer nije potreban
    uint8_t ppudata_buffer = 0x00;

    GamePak *gamepak;

private:
    // pomoćne varijable za background rendering
    uint8_t tile_id = 0x00;
    uint8_t tile_attribute = 0x00;
    uint8_t tile_lsb = 0x00;
    uint8_t tile_msb = 0x00;

private:
    // pomoćne varijable koje modeliraju shift registre
    uint16_t shifter_attribute_low = 0x0000;
    uint16_t shifter_attribute_high = 0x0000;
    uint16_t shifter_pattern_low = 0x0000;
    uint16_t shifter_pattern_high = 0x0000;

private:
    // struktura za identifikaciju piksela i palete
    struct Palette {
        uint8_t pixel_id, palette_id;
    };
    Palette getComposition();

private:
    void scrollingHorizontal();
    void scrollingVertical();
    void transferHorizontal();
    void transferVertical();
    void loadPixel();
    void updateShiftRegister();
    void fetchNextTile(uint8_t selector);

public:
    bool interrupt = false;
    int scanline = -1;
    uint8_t *oam_memory = (uint8_t*)OAM;

public:
    uint8_t readCPUMemory(uint16_t address);
    void writeCPUMemory(uint16_t address, uint8_t data);
    uint8_t readPPUMemory(uint16_t address);
    void writePPUMemory(uint16_t address, uint8_t data);

    void clock();
    void reset();
    void connectGamePak(GamePak *gamePak);
};


#endif //NES_PPU2C02_H
