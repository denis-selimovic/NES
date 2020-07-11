//
// Created by deniss on 7/8/20.
//

#include "../include/Key.h"

bool Key::isHeld() {
    oldState = newState;
    return newState;
}
