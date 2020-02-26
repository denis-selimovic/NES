//
// Created by denis on 26/02/2020.
//

#ifndef NES_MAPPER000_H
#define NES_MAPPER000_H


#include "Mapper.h"

class Mapper000: public Mapper{

public:
    Mapper000(uint8_t pgr_banks, uint8_t chr_banks);

    void reset() override;
    bool writeCPUMemory(uint16_t address, uint32_t &mapped_address) override;
    bool readCPUMemory(uint16_t address, uint32_t &mapped_address) override;
    bool writePPUMemory(uint16_t address, uint32_t &mapped_address) override;
    bool readPPUMemory(uint16_t address, uint32_t &mapped_address) override;
};


#endif //NES_MAPPER000_H
