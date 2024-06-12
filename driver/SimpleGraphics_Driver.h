#pragma once
#include <stdint.h>
#include <string.h>
#include "../Src/User/MemoryRegion.h"
#include "../Src/User/Resolution.h"
#if !defined SYSTEM
    #define SYSTEM none
#endif
#if SYSTEM == avr
    #include "arch/avr/cores/Arduino.h"
    #include "arch/avr/libraries/SoftwareSerial/src/SoftwareSerial.h"
    #ifndef SimpleGraphics_Serial
        #define SimpleGraphics_Serial Serial
    #endif
#elif SYSTEM == linux || SYSTEM == win || SYSTEM == dos
    #include "arch/Serial.h"
#elif SYSTEM == custom
#else
    #pragma message("Available system: avr linux win dos custom(define your own serial.cpp and driver code for your project)")
    #pragma error("Please set the macro SYSTEM with your target's system.")
#endif

#define SimpleGraphics_SUCCESS 0
#define SimpleGraphics_FAILURE 1

void __attribute__((weak)) setResolution(Resolution){};
void __attribute__((weak)) setLTDCClock(Resolution){};

struct SimpleGraphics{
    uint8_t(*init)(void);
    uint8_t(*modeset)(Resolution);
    uint8_t(*write)(void*, uint32_t, uint16_t);
    uint8_t(*read)(void*, uint32_t, uint16_t);
    uint8_t (*copy)(uint32_t, uint32_t, uint16_t);
    uint8_t (*fillVideo)(uint32_t, uint32_t, uint16_t, uint16_t, uint32_t);
    uint8_t (*copyVideo)(uint32_t, uint32_t, uint16_t, uint16_t, uint16_t, uint32_t);
    uint8_t (*mixVideo)(uint32_t, uint32_t, uint32_t, uint16_t, uint16_t, uint16_t, uint32_t, uint32_t);
    uint8_t (*addThread)(uint32_t, uint32_t);
    uint8_t (*removeThread)(uint32_t);
};
extern struct SimpleGraphics SimpleGraphicsFunc;

static uint8_t SimpleGraphics_init(void);
static uint8_t SimpleGraphics_modeset(Resolution);
static uint8_t SimpleGraphics_write(void*, uint32_t, uint16_t);
static uint8_t SimpleGraphics_read(void*, uint32_t, uint16_t);
static uint8_t SimpleGraphics_copy(uint32_t, uint32_t, uint16_t);
static uint8_t SimpleGraphics_fillVideo(uint32_t, uint32_t, uint16_t, uint16_t, uint32_t);
static uint8_t SimpleGraphics_copyVideo(uint32_t, uint32_t, uint16_t, uint16_t, uint16_t, uint32_t);
static uint8_t SimpleGraphics_mixVideo(uint32_t, uint32_t, uint32_t, uint16_t, uint16_t, uint16_t, uint32_t, uint32_t);
static uint8_t SimpleGraphics_addThread(uint32_t, uint32_t);
static uint8_t SimpleGraphics_removeThread(uint32_t);