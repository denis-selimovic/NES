//
// Created by denis on 29/02/2020.
//

#ifndef NES_UTILITY_H
#define NES_UTILITY_H

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <string>

std::string getCurrentDirectory() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}

#endif //NES_UTILITY_H
