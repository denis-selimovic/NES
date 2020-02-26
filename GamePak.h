//
// Created by denis on 24/02/2020.
//

#ifndef NES_GAMEPAK_H
#define NES_GAMEPAK_H

#include <string>
#include <vector>
#include "Mapper.h"

class GamePak {
public:
    enum MIRRORING {HORIZONTAL, VERTICAL} mirroring = HORIZONTAL;


    //GamePak se sastoji PRG (programske memorije PRG-ROM) i CHR(memorija sprite-ova CHR-ROM)
    //njih učitamo iz .nes datoteke
    std::vector<uint8_t> PRG;
    std::vector<uint8_t> CHR;


    //pomoćna struktura za učitavanje header-a .nes fajla
    //na osnovu ovoga saznajemo informacije o vrsti .nes fajla
    //možemo odrediti kapacitet PRG i CHR memorije
    struct NESHeader {
        uint8_t header[4];
        uint8_t prg_rom_size;
        uint8_t chr_rom_size;
        uint8_t flags_mirroring_trainer;
        uint8_t flags_playchoice;
        uint8_t flags_prg_ram_size;
        uint8_t tv_system;
        uint8_t prg_ram_presence;
        uint8_t unused[5];
    };

    //pokazivač na mapper koji se koristi za mapiranje adresa
    Mapper *mapper = nullptr;

    //funkcija za određivanje koji mapper se koristi
    Mapper *setMapper(uint8_t mapperID, uint8_t prg_banks, uint8_t chr_banks);
public:
    GamePak(const std::string &game);
};


#endif //NES_GAMEPAK_H
