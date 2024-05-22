#include "../Serial.h"

#include <stdlib.h>
#include <windows.h>

HardwareSerial::operator bool(){
    return fd != (HANDLE)-1;
}

// Find the first of available SimpleGraphics
void HardwareSerial::begin(uint32_t baudrate){
    char buf[20], buf2[4];
    for(uint8_t i = 0; i <= 255; i++){
        fd = NULL;
        strcpy(buf, "COM");
        strcat(buf, itoa(i, buf2, 10));
        fd = createFileA(buf, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
        if(fd == (HANDLE)-1) {end(); continue;}
        if(!SetupComm(hCom, 0, 0)) {end(); continue;}
        DCB p = {};
        p.DCBlength = sizeof(p);
        p.BaudRate = baudrate; // 9600
        p.ByteSize = 8;
        p.Parity = NOPARITY;
        p.StopBits = ONESTOPBIT;
        if(!SetCommState(fd, &p)) {end(); continue;}
        COMMTIMEOUTS TimeOuts;
        TimeOuts.ReadIntervalTimeout = 1000;
        TimeOuts.ReadTotalTimeoutMultiplier = 500;
        TimeOuts.ReadTotalTimeoutConstant = 5000;
        TimeOuts.WriteTotalTimeoutMultiplier = 500;
        TimeOuts.WriteTotalTimeoutConstant = 2000;
        SetCommTimeouts(fd,&TimeOuts);
        PurgeComm(fd,PURGE_TXCLEAR|PURGE_RXCLEAR);
        int status = 0;
        WriteFile(fd, "\0", 1, &status, NULL);
        if(status <= 0) {end(); continue;}
        delay_ms(100);
        ReadFile(fd, buf, 20, &status, NULL);
        if(status <= 0) {end(); continue;}
        if(strcmp(buf, "SimpleGraphics v1.0")) {end(); continue;}
    }
}

void HardwareSerial::write(uint8_t* buf, uint16_t length){
    OVERLAPPED m_osWrite;
    memset(&m_osWrite, 0, sizeof(m_osWrite));
    m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, L"WriteEvent");
    WriteFile(fd, buf, length, &length, &m_osWrite);
    CloseHandle(m_osWrite.hEvent);
}

void HardwareSerial::read(uint8_t* buf, uint16_t length){
    OVERLAPPED m_osRead;
    memset(&m_osRead, 0, sizeof(m_osRead));
    m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, L"ReadEvent");
    ReadFile(hCom, buf, length, &length, &m_osRead);
    CloseHandle(m_osWrite.hEvent);
}

void HardwareSerial::end(){
    CloseHandle(fd);
}