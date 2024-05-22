#pragma once
#include <stdint.h>

// USART Recieve Buffer
extern uint8_t USARTRecieveBuffer[65536];
extern uint8_t USARTTranmitBuffer[32768];

// External SDRAM Address A
extern uint32_t* ExternalSDRAM;
extern uint32_t ExternalSDRAMLength;

// External SDRAM Address B
extern uint32_t* ExternalSDRAM2;
extern uint32_t ExternalSDRAMLength2;

// External NAND Flash Address
extern uint32_t* ExternalNAND;
extern uint32_t ExternalNANDLength;