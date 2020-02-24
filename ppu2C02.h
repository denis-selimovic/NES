//
// Created by denis on 24/02/2020.
//

#ifndef NES_PPU2C02_H
#define NES_PPU2C02_H


#include <cstdint>
#include "GamePak.h"

class ppu2C02 {

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
        } bits;
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
        } bits;
        uint8_t reg;
    } ppumask;

    //treći registar je PPUSTATUS
    union PPUSTATUS {
        struct {
            uint8_t U : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vblank : 1;
        } bits;
        uint8_t reg;
    } ppustatus;

    //četvrti registar čuva adresu pomoću koje se pristupa OAM-u
    uint8_t OAM_address;
    //OAM je object attribute memory koja čuva informacije o spirte-ovima
    //interna memorija koja čuva do 64 spirte-a svaki sprite se sastoji od 4 bajta
    struct Sprite{
        uint8_t y_position;
        uint8_t tile_index;
        uint8_t attributes;
        uint8_t index;
    } OAM[64];


    GamePak *gamepak;
public:

    uint8_t readCPUMemory(uint16_t address);
    void writeCPUMemory(uint16_t address, uint8_t data);
    uint8_t readPPUMemory(uint16_t address);
    void writePPUMemory(uint16_t address, uint8_t data);
    void connectGamePak(GamePak *gamepak);
};


#endif //NES_PPU2C02_H
