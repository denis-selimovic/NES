//
// Created by denis on 24/02/2020.
//

#ifndef NES_PPU2C02_H
#define NES_PPU2C02_H


#include <cstdint>

class ppu2C02 {

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

public:

    uint8_t readCPUMemory(uint16_t address);
    void writeCPUMemory(uint16_t address, uint8_t data);
};


#endif //NES_PPU2C02_H
