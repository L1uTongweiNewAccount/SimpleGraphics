#pragma once
#include <stdint.h>
#include <string.h>
struct HardwareSerial{
    #if SYSTEM == linux || SYSTEM == dos
        int fd = -1;
    #elif SYSTEM == win
        #include <windows.h>
        #include <WinSock2.h>
        HANDLE fd = (HANDLE)-1;
    #endif
    operator bool();
    void begin(uint32_t);
    void write(uint8_t c){write(&c, 1)};
    void write(uint8_t*, uint16_t);
    void read(uint8_t*, uint16_t);
    void end();
};

extern HardwareSerial Serial;