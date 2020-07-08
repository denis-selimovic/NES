//
// Created by deniss on 7/8/20.
//

#ifndef NES_KEY_H
#define NES_KEY_H


class Key {

public:
    bool oldState = false;
    bool newState = false;
    bool isHeld();
};


#endif //NES_KEY_H
