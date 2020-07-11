//
// Created by deniss on 7/11/20.
//

#ifndef NES_SHIFTER_H
#define NES_SHIFTER_H


#include <cstdint>

class Shifter {

    uint16_t lowByte = 0x0000, highByte = 0x0000;
    void packLowByte(uint8_t data);
    void packHighByte(uint8_t data);
public:
    void shift();
    void pack(uint8_t low, uint8_t high);
    void reset();
    void setBytes(uint8_t low, uint8_t high);
    uint16_t getLowByte() const;
    uint16_t getHighByte() const;
};


#endif //NES_SHIFTER_H
