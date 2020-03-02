//
// Created by denis on 26/02/2020.
//

#include "Mapper000.h"

Mapper000::Mapper000(uint8_t pgr_banks, uint8_t chr_banks) : Mapper(pgr_banks, chr_banks) {}

void Mapper000::reset() {
    // ovaj mapper ne radi ništa u reset funkciji
}

bool Mapper000::writeCPUMemory(uint16_t address, uint32_t &mapped_address) {
    // ova funkcija mapira adresu sa CPU sabirnice u adresu koja postoji na GamePak-u
    // na sabirnici za CPU adrese od 0x8000 do 0xFFFF su rezervisane za pristup GamePak-u
    // te adrese se onda preko mapera mapiraju u opseg adresa koje obuhvata PRG-ROM
    // ako je broj bankova 1 adrese iz opsega CPU se mapiraju u 0x0000-0x3FFF
    // ako je broj bankova veći od 1 adrese se mapiraju u 0x0000-0x7FFFF opseg PRG-ROM
    if(address >= 0x8000 && address <= 0xFFFF) {
        mapped_address = address & unsigned(PRG_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper000::readCPUMemory(uint16_t address, uint32_t &mapped_address) {
    // ova funkcija mapira adresu sa CPU sabirnice u adresu koja postoji na GamePak-u
    // na sabirnici za CPU adrese od 0x8000 do 0xFFFF su rezervisane za pristup GamePak-u
    // te adrese se onda preko mapera mapiraju u opseg adresa koje obuhvata PRG-ROM
    // ako je broj bankova 1 adrese iz opsega CPU se mapiraju u 0x0000-0x3FFF
    // ako je broj bankova veći od 1 adrese se mapiraju u 0x0000-0x7FFFF opseg PRG-ROM
    if(address >= 0x8000 && address <= 0xFFFF) {
        mapped_address = address & unsigned(PRG_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper000::writePPUMemory(uint16_t address, uint32_t &mapped_address) {
    // što se tiče ppu sabirnice adresa je što se ovog mappera tiče samo proslijeđena tj nema mapiranja
    // opseg sa ppu sabirnice od 0x0000 do 0x1FFF se mapira u isti taj opseg
    // pošto se radi o CHR-ROM upis nije moguć pa nema ni mapiranja
    // mapiranje je jedino moguće ako je to ustvari CHR-RAM tj kad je broj bankova 0
    if(address >= 0x0000 && address <= 0x1FFF && CHR_banks == 0) {
        mapped_address = address;
        return true;
    }
    return false;
}

bool Mapper000::readPPUMemory(uint16_t address, uint32_t &mapped_address) {
    // što se tiče ppu sabirnice adresa je što se ovog mappera tiče samo proslijeđena tj nema mapiranja
    // opseg sa ppu sabirnice od 0x0000 do 0x1FFF se mapira u isti taj opseg
    if(address >= 0x0000 && address <= 0x1FFF) {
        mapped_address = address;
        return true;
    }
    return false;
}
