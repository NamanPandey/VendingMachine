#pragma once

#include "Utils.hpp"

#ifdef linux
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

void sleepFunction(int seconds) {
    #ifdef linux
    sleep(seconds);
    #elif _WIN32
    Sleep(seconds * 1000);
    #endif
}

