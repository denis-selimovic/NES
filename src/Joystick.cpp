//
// Created by deniss on 7/8/20.
//

#include "../include/Joystick.h"

Joystick::Joystick() {
    keyboard[SDLK_x] = {0x80u, false};
    keyboard[SDLK_z] = {0x40u, false};
    keyboard[SDLK_a] = {0x20u, false};
    keyboard[SDLK_s] = {0x10u, false};
    keyboard[SDLK_UP] = {0x08u, false};
    keyboard[SDLK_DOWN] = {0x04u, false};
    keyboard[SDLK_LEFT] = {0x02u, false};
    keyboard[SDLK_RIGHT] = {0x01u, false};
}

void Joystick::writeState(SDL_Keycode code, bool state) {
    if(keyboard.find(code) == keyboard.end()) return;
    Key pressedKey = keyboard[code];
    keyboard[code] = pressedKey.changeState(state);
}

void Joystick::updateState(uint8_t index) {
    if(index < 0 || index > 1) return;
    buffer[index] = 0x00;
    for(auto &it : keyboard) buffer[index] |= (it.second.pressed) ? it.second.mask : 0x00u;
}


