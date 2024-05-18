#pragma once
#include <ltdc.h>
#include <stdint.h>

typedef struct{
    uint16_t height, width;
    uint16_t HSync, VSync;
    uint16_t HBack, VBack;
}Resolution;
void setResolution(Resolution);

extern const Resolution r480x272;
extern const Resolution r640x480;
extern const Resolution r800x480;
extern const Resolution r800x600;
extern const Resolution r1024x600;
extern const Resolution r1024x768;
extern const Resolution r1280x720;
extern const Resolution r1920x1080;

extern Resolution* resolutions[8];