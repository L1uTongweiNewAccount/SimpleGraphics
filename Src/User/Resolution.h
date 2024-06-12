#pragma once
#include <ltdc.h>
#include <stdint.h>

typedef struct{
    uint16_t height, width;
    uint16_t HSync, VSync;
    uint16_t HBack, VBack;
    uint16_t PLL3N, PLL3R;
}Resolution;
void setResolution(Resolution);

const __attribute__((weak)) Resolution r640x480_60 = {640, 480, 95, 1, 63, 42, 129, 128};
const __attribute__((weak)) Resolution r640x480_75 = {640, 480, 63, 2, 135, 16, 63, 50};
const __attribute__((weak)) Resolution r800x600_60 = {800, 600, 127, 3, 127, 23, 8, 5};
const __attribute__((weak)) Resolution r800x600_72 = {800, 600, 119, 5, 119, 59, 4, 2};
const __attribute__((weak)) Resolution r800x600_75 = {800, 600, 79, 2, 175, 21, 99, 50};
const __attribute__((weak)) Resolution r1024x768_60 = {1024, 768, 135, 5, 183, 31, 13, 5};
const __attribute__((weak)) Resolution r1024x768_75 = {1024, 768, 175, 2, 191, 28, 394, 125};
const __attribute__((weak)) Resolution r1280x800_60 = {1280, 800, 135, 2, 263, 24, 333, 100};
const __attribute__((weak)) Resolution r1280x960_60 = {1280, 960, 111, 2, 407, 36, 108, 25};
const __attribute__((weak)) Resolution r1280x1024_60 = {1280, 1024, 111, 2, 295, 38, 108, 25};
const __attribute__((weak)) Resolution r1440x900_60 = {1440, 900, 151, 2, 311, 28, 213, 50};
const __attribute__((weak)) Resolution r1920x1080_30 = {1920, 1080, 44, 5, 280, 45, 148, 50};
const __attribute__((weak)) Resolution r1920x1080_60 = {1920, 1080, 11, 3, 67, 20, 26, 5};
