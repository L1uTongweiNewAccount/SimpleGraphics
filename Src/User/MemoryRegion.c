#include "MemoryRegion.h"

// USART Recieve Buffer
uint8_t USARTRecieveBuffer[65536];
uint8_t USARTTranmitBuffer[32768];

// External SDRAM Address
uint32_t* ExternalSDRAM = (uint32_t*)0xC0000000;
uint32_t ExternalSDRAMLength = 0x200000;