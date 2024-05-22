#include "../Serial.h"

#include <dos.h>
#include <bios.h>
union REGS inregs,outregs;

HardwareSerial::operator bool(){
    return fd != -1;
}

// Find the first of available SimpleGraphics
void HardwareSerial::begin(uint32_t baudrate){
    char buf[20], buf2[4];
    for(uint8_t i = 0; i < 4; i++){
        fd = -1;
        do{
            inregs.h.ah = 0;
            inregs.h.al = 0b11100011;
            inregs.x.dx = i;
            int86(0x14, &inregs, &outregs);
       } while(outregs.h.ah >= 0x80);
        int status = 0;
        write(0);
        read(buf, 20);
        if(strcmp(buf, "SimpleGraphics v1.0")) {end(); continue;}
        fd = i;
    }
}

void HardwareSerial::write(uint8_t* buf, uint16_t length){
    do{
        inregs.h.ah = 1;
        inregs.h.al = *buf;
        inregs.x.dx = fd;
        int86(0x14, &inregs, &outregs);
    }while(outregs.h.al >= 0x80);
    for(int i = 1; i < len; i++){
        inregs.h.ah = 1;
        inregs.h.al = *(buf + i);
        inregs.x.dx = fd;
        int86(0x14, &inregs, &outregs);
    }
}

void HardwareSerial::read(uint8_t* buf, uint16_t length){
    int i = 0;
    do{
        inregs.h.ah = 2;
        inregs.x.dx = fd;
        int86(0x14, &inregs, &outregs);
        *(buf + i) = outregs.h.al;
    }while(i < length && outregs.h.ah >= 0x80);
}

void HardwareSerial::end(){}