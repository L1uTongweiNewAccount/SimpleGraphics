#pragma once
#include <stdint.h>

// USART Recieve Buffer
extern uint8_t USARTRecieveBuffer[65536];
extern uint8_t USARTTranmitBuffer[32768];

// External SDRAM Address
extern uint32_t* ExternalSDRAM;
extern uint32_t ExternalSDRAMLength;