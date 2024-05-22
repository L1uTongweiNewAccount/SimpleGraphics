#include "MemoryRegion.h"

// USART Recieve Buffer
uint8_t USARTRecieveBuffer[65536];
uint8_t USARTTranmitBuffer[32768];

// External SDRAM Address A
uint32_t* ExternalSDRAM = (uint32_t*)0xC0000000;
uint32_t ExternalSDRAMLength = 0x200000;

// External SDRAM Address B
uint32_t* ExternalSDRAM2 = (uint32_t*)0xD0000000;
uint32_t ExternalSDRAMLength2 = 0x200000;

// External NAND Flash Address
uint32_t* ExternalNAND = (uint32_t*)0x70000000;
uint32_t ExternalNANDLength = 0x4000000;