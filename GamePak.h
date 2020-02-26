//
// Created by denis on 24/02/2020.
//

#ifndef NES_GAMEPAK_H
#define NES_GAMEPAK_H

#include <string>

class GamePak {
public:
    enum MIRRORING {HORIZONTAL, VERTICAL} mirroring = HORIZONTAL;

public:
    GamePak(const std::string &game);
};


#endif //NES_GAMEPAK_H
