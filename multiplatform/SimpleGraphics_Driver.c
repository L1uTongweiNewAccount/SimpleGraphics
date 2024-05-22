#include "SimpleGraphics_Driver.h"

const Resolution r480x272 = {480, 272, 41, 10, 4, 4};
const Resolution r640x480 = {640, 480, 96, 2, 64, 2};
const Resolution r800x480 = {800, 480, 128, 2, 128, 43};
const Resolution r800x600 = {800, 600, 128, 4, 128, 24};
const Resolution r1024x600 = {1024, 600, 136, 4, 184, 24};
const Resolution r1024x768 = {1024, 768, 136, 6, 184, 32};
const Resolution r1280x720 = {1280, 720, 40, 5, 330, 25};
const Resolution r1920x1080 = {1920, 1080, 44, 5, 236, 40};

struct SimpleGraphics SimpleGraphicsFunc = {
    .setLayer = SimpleGraphics_setLayer,
    .addThread = SimpleGraphics_addThread,
    .init = SimpleGraphics_init,
    .copy = SimpleGraphics_copy,
    .copyVideo = SimpleGraphics_copyVideo,
    .fillVideo = SimpleGraphics_fillVideo,
    .mixVideo = SimpleGraphics_mixVideo,
    .modeset = SimpleGraphics_modeset,
    .read = SimpleGraphics_read,
    .removeThread = SimpleGraphics_removeThread,
    .write = SimpleGraphics_write
};

static uint8_t SimpleGraphics_init(void){
    char buf[20];
    SimpleGraphics_Serial.begin(115200);
    if(!Serial) return SimpleGraphics_FAILURE;
    SimpleGraphics_Serial.write(1);
    SimpleGraphics_Serial.write(0);
    SimpleGraphics_Serial.read(buf, 20);
    return !strcmp(buf, "SimpleGraphics v1.0") ? SimpleGraphics_SUCCESS : SimpleGraphics_FAILURE;
}

static uint8_t SimpleGraphics_modeset(Resolution r){
    SimpleGraphics_Serial.write(6);
    SimpleGraphics_Serial.write(3);
    SimpleGraphics_Serial.write((uint8_t*)&r, sizeof(Resolution));
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_write(void* buf, uint32_t addr, uint16_t length){
    SimpleGraphics_Serial.write(length + 7);
    SimpleGraphics_Serial.write(3);
    SimpleGraphics_Serial.write(addr);
    SimpleGraphics_Serial.write(length);
    SimpleGraphics_Serial.write((uint8_t*)buf, length);
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_read(void* buf, uint32_t addr, uint16_t length){
    SimpleGraphics_Serial.write(7);
    SimpleGraphics_Serial.write(4);
    SimpleGraphics_Serial.write(addr);
    SimpleGraphics_Serial.write(length);
    SimpleGraphics_Serial.read((uint8_t*)buf, length);
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_copy(uint32_t dstAddr, uint32_t srcAddr, uint16_t lentgh){
    SimpleGraphics_Serial.write(11);
    SimpleGraphics_Serial.write(5);
    SimpleGraphics_Serial.write(srcAddr);
    SimpleGraphics_Serial.write(dstAddr);
    SimpleGraphics_Serial.write(length);
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_setLayer(uint32_t id, uint32_t addr, uint16_t X0, uint16_t X1, uint16_t Y0, uint16_t Y1, uint8_t BackR, uint8_t BackG, uint8_t BackB, uint8_t Alpha){
    SimpleGraphics_Serial.write(25);
    SimpleGraphics_Serial.write(6);
    SimpleGraphics_Serial.write(X0);
    SimpleGraphics_Serial.write(X1);
    SimpleGraphics_Serial.write(Y0);
    SimpleGraphics_Serial.write(Y1);
    SimpleGraphics_Serial.write(addr);
    SimpleGraphics_Serial.write(Alpha);
    SimpleGraphics_Serial.write(BackR);
    SimpleGraphics_Serial.write(BackG);
    SimpleGraphics_Serial.write(BackB);
    SimpleGraphics_Serial.write(id);
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_fillVideo(uint32_t addr, uint32_t color, uint16_t x, uint16_t y, uint16_t screen_x){
    SimpleGraphics_Serial.write(15);
    SimpleGraphics_Serial.write(7);
    SimpleGraphics_Serial.write(color);
    SimpleGraphics_Serial.write(addr);
    SimpleGraphics_Serial.write(screen_x - x);
    SimpleGraphics_Serial.write(x);
    SimpleGraphics_Serial.write(y);
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_copyVideo(uint32_t dstAddr, uint32_t srcAddr, uint16_t x, uint16_t y, uint16_t screen_x, uint32_t excolor){
    SimpleGraphics_Serial.write(19);
    SimpleGraphics_Serial.write(8);
    SimpleGraphics_Serial.write(srcAddr);
    SimpleGraphics_Serial.write(dstAddr);
    SimpleGraphics_Serial.write(excolor);
    SimpleGraphics_Serial.write(screen_x - x);
    SimpleGraphics_Serial.write(x);
    SimpleGraphics_Serial.write(y);
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_mixVideo(uint32_t addrA, uint32_t addrB, uint32_t dstAddr, uint16_t x , uint16_t y, uint16_t screen_x, uint32_t colorA, uint32_t colorB){
    SimpleGraphics_Serial.write(27);
    SimpleGraphics_Serial.write(9);
    SimpleGraphics_Serial.write(addrA);
    SimpleGraphics_Serial.write(addrB);
    SimpleGraphics_Serial.write(dstAddr);
    SimpleGraphics_Serial.write(colorA);
    SimpleGraphics_Serial.write(colorB);
    SimpleGraphics_Serial.write(screen_x - x);
    SimpleGraphics_Serial.write(x);
    SimpleGraphics_Serial.write(y);
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_addThread(uint32_t id, uint32_t addr){
    SimpleGraphics_Serial.write(9);
    SimpleGraphics_Serial.write(10);
    SimpleGraphics_Serial.write(id);
    SimpleGraphics_Serial.write(addr);
    return SimpleGraphics_SUCCESS;
}

static uint8_t SimpleGraphics_removeThread(uint32_t id){
    SimpleGraphics_Serial.write(5);
    SimpleGraphics_Serial.write(11);
    SimpleGraphics_Serial.write(id);
    return SimpleGraphics_SUCCESS;
}