//
// Created by denis on 24/02/2020.
//

#include <fstream>
#include <stdexcept>
#include "GamePak.h"

GamePak::GamePak(const std::string &game) {
    // .nes fajl ima sljedeću strukturu
    // header of 16 bajta koji ćemo učitati koristeći strukturu header
    // sljedećih 512 bajta možda okupira trainer što možemo provjeriti
    // nakon ovog na red dolazi PRG-ROM koji zauzima prg_rom_size x 16KB
    // na kraju dolazimo do CHR-ROM koji zauzima chr_rom_size x 8KB


    //otvaramo binarnu datoteku
    std::ifstream nes(game, std::ifstream::binary);
    if(nes.is_open()) {
        // .nes datoteka je uspješno otvorena
        // prvo učitavamo header
        NESHeader header{};
        nes.read((char*)&header, sizeof(NESHeader));

        // u zavisnosti od 3 bita u bajtu 6 headera provjeravamo da li da preskočimo sljedećih 512 bajta
        // ako je ovaj bit postavljen na 1 što saznajemo pomoću maske 0x04 onda možemo preskočiti 512 bajta
        if(header.flags_mirroring_trainer & 0x04u) nes.seekg(512, std::ios::cur);


        // na osnovu prvog bita bajta 6 u header možemo odrediti mirroring
        // 0 - horizontalni, 1 - vertikalni
        (header.flags_mirroring_trainer & 0x01u) ? mirroring = VERTICAL : mirroring = HORIZONTAL;

        // id mappera možemo odrediti na osnovu bita bajta 6 i bajta 7 u headeru
        // na osnovu ovog id-a određujemo koji mapper se koristi
        // gornja četiri bita 6. bajta određuju low nibble id-a
        // gornja četiri bita 7. bajta određuju high nibble id-a
        uint8_t mapper = ((header.flags_mirroring_trainer & 0xF0u) >> 4u) | (header.flags_playchoice & 0xF0u);

        // učitamo PRG-ROM
        // bajt 4 iz headera nam govori kolika je veličina ove memorije u 16KB
        PRG.resize(header.prg_rom_size * 16384);
        nes.read((char*)PRG.data(), PRG.size());

        //ako je broj bankova CHR-ROM 0 onda alociramo CHR-RAM od 8KB
        //ako to nije slučaj alociramo CHR-ROM na isti način kao PRG-ROM
        (header.chr_rom_size == 0) ? CHR.resize(8192) : CHR.resize(header.chr_rom_size * 8192);
        nes.read((char*)CHR.data(), CHR.size());


        //postavljamo mapper na osnovu id-a
        this->mapper = setMapper(mapper, header.prg_rom_size, header.chr_rom_size);

        nes.close();
    }
    else {
        throw std::logic_error("ERROR!");
    }
}

Mapper *GamePak::setMapper(uint8_t mapperID, uint8_t prg_banks, uint8_t chr_banks) {
    return nullptr;
}

bool GamePak::readCPUMemory(uint16_t address, uint8_t &data) {
    uint32_t mapped_address = 0x0000;
    if(mapper->readCPUMemory(address, mapped_address)) {
        data = PRG[mapped_address];
        return true;
    }
    return false;
}

bool GamePak::writeCPUMemory(uint16_t address, uint8_t data) {
    uint32_t mapped_address = 0x0000;
    if(mapper->writeCPUMemory(address, mapped_address)) {
        PRG[mapped_address] = data;
        return true;
    }
    return false;
}

bool GamePak::readPPUMemory(uint16_t address, uint8_t &data) {
    uint32_t mapped_address = 0x0000;
    if(mapper->readPPUMemory(address, mapped_address)) {
        data = CHR[mapped_address];
        return true;
    }
    return false;
}

bool GamePak::writePPUMemory(uint16_t address, uint8_t data) {
    uint32_t mapped_address = 0x0000;
    if(mapper->writePPUMemory(address, mapped_address)) {
        CHR[mapped_address] = data;
        return true;
    }
    return false;
}

