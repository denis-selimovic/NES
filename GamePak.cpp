//
// Created by denis on 24/02/2020.
//

#include <fstream>
#include "GamePak.h"

GamePak::GamePak(const std::string &game) {
    // .nes fajl ima sljedeću strukturu
    // header of 16 bajta koji ćemo učitati koristeći strukturu header
    // sljedećih 512 bajta možda okupira trainer što možemo provjeriti
    // nakon ovog na red dolazi PRG-ROM koji zauzima prg_rom_size x 16KB
    // na kraju dolazimo do CHR-ROm koji zauzima chr_rom_size x 8KB


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

        // učitamo PRG-ROM
        // bajt 4 iz headera nam govori kolika je veličina ove memorije u 16KB
        PRG.resize(header.prg_rom_size * 16384);
        nes.read((char*)PRG.data(), PRG.size());

        nes.close();
    }
}
