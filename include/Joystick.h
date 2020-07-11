//
// Created by deniss on 7/8/20.
//

#ifndef NES_JOYSTICK_H
#define NES_JOYSTICK_H


#include <array>
#include <SDL2/SDL.h>
#include <map>

class Joystick {

    struct Key {
        uint8_t mask;
        bool pressed;
        Key changeState(bool state) {
            this->pressed = state;
            return *this;
        }
    };

    std::map<SDL_Keycode, Key> keyboard;

public:
    std::array<uint8_t, 2> buffer{};

public:
    Joystick();
    void writeState(SDL_Keycode code, bool state);
    void updateState(uint8_t index);
};


#endif //NES_JOYSTICK_H
