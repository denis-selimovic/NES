//
// Created by deniss on 7/12/20.
//

#ifndef NES_SPRITESHIFTER_H
#define NES_SPRITESHIFTER_H


#include <cstdint>

class SpriteShifter {

    uint8_t lowByte = 0x00u, highByte = 0x00u;

public:
    void reset();
    void shift();
    void setBytes(uint8_t low, uint8_t high);
    uint8_t getHighByte() const;
    uint8_t getLowByte() const;
};


#endif //NES_SPRITESHIFTER_H
