#include <picotk.h>
#include <stdlib.h>
#include "main.h"
#include "MemoryRegion.h"
#include "Resolution.h"
#include "stm32h7xx_hal_tim.h"

struct LinkedSheet{
    uint32_t id;
    void(*task)(void);
    uint8_t *memory, *stackPtr;
    struct LinkedSheet *next;
}*threadsRoot = NULL, *ptr = NULL;

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_memtomem_dma1_stream2;
extern DMA_HandleTypeDef hdma_memtomem_dma1_stream3;
extern DMA_HandleTypeDef hdma_memtomem_dma1_stream4;
TIM_HandleTypeDef htim3;
uint16_t instructionSize = 0;
uint8_t IsPre = 1;

void setup(){
    // Erase NAND Flash
    __builtin_memset(ExternalNAND, 0, ExternalNANDLength * 4);
    // Recieve Pack that 2 bytes large & n bytes large
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)&instructionSize, 2);
    // Init Timer3
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 1000;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if(HAL_TIM_Base_Init(&htim3) != HAL_OK) Error_Handler();
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if(HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) Error_Handler();
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if(HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) Error_Handler();
    __HAL_RCC_TIM3_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
void TIM3_IRQHandler(void) {
    if(ptr == NULL) return;
    asm volatile("mov %0, sp" :"=r"(ptr->stackPtr));
    if(ptr->next == NULL) ptr = threadsRoot;
    else ptr = ptr->next;
    asm volatile("mov sp, %0" ::"r"(ptr->stackPtr));
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(!IsPre) HAL_UART_Receive_DMA(&huart1, USARTRecieveBuffer, instructionSize);
    else{
        switch(USARTRecieveBuffer[0]){ // switch the type
            case 0: // Card Info
                HAL_UART_Transmit_DMA(&huart1, "SimpleGraphics v1.0", 20);
                break;
            // case 1: // switch the resolution (in fixed mode)
            //     // NOTE: Expected size 2
            //     if(USARTRecieveBuffer[1] > 7) break;
            //     setResolution(*resolutions[USARTRecieveBuffer[1]]);
            //     break;
            case 2:{ // switch the resolution (customed)
                //NOTE: Expected size 7
                Resolution *r = (Resolution*)&USARTRecieveBuffer[1];
                setResolution(*r);
                break;
            }
            case 3: // normal write memory
                //NOTE: expected size > 7
                HAL_DMA_Start(
                    &hdma_memtomem_dma1_stream2, 
                    (uint32_t)(USARTRecieveBuffer + 7),
                    *(uint32_t*)&USARTRecieveBuffer[1],
                    *(uint16_t*)&USARTRecieveBuffer[5]
                );
                break;
            case 4: // normal read memory
                //NOTE: expected size 7
                HAL_DMA_Start(
                    &hdma_memtomem_dma1_stream3, 
                    *(uint32_t*)&USARTRecieveBuffer[1],
                    (uint32_t)USARTTranmitBuffer,
                    *(uint16_t*)&USARTRecieveBuffer[5]
                );
                while(HAL_DMA_GetState(&hdma_memtomem_dma1_stream2) != HAL_DMA_STATE_READY);
                HAL_UART_Transmit_DMA(huart, USARTTranmitBuffer, *(uint16_t*)&USARTRecieveBuffer[5]);
                break;
            case 5: // normal copy memory
                //NOTE: expected size 11
                HAL_DMA_Start(
                    &hdma_memtomem_dma1_stream4, 
                    *(uint32_t*)&USARTRecieveBuffer[1],
                    *(uint32_t*)&USARTRecieveBuffer[5],
                    *(uint16_t*)&USARTRecieveBuffer[9]
                );
                break;
            case 6:{ // apply a layer
                //NOTE: expected size 25
                LTDC_LayerCfgTypeDef pLayerCfg = {};
                pLayerCfg.WindowX0 = *(uint16_t*)&USARTRecieveBuffer[1];
                pLayerCfg.WindowX1 = *(uint16_t*)&USARTRecieveBuffer[3];
                pLayerCfg.WindowY0 = *(uint16_t*)&USARTRecieveBuffer[5];
                pLayerCfg.WindowY1 = *(uint16_t*)&USARTRecieveBuffer[7];
                pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
                pLayerCfg.Alpha = *(uint8_t*)&USARTRecieveBuffer[13];
                pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
                pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
                pLayerCfg.FBStartAdress = *(uint32_t*)&USARTRecieveBuffer[9];
                pLayerCfg.ImageWidth = pLayerCfg.WindowX1 - pLayerCfg.WindowX0;
                pLayerCfg.ImageHeight = pLayerCfg.WindowY1 - pLayerCfg.WindowY0;
                pLayerCfg.Backcolor.Blue = *(uint8_t*)&USARTRecieveBuffer[19];
                pLayerCfg.Backcolor.Green = *(uint8_t*)&USARTRecieveBuffer[17];
                pLayerCfg.Backcolor.Red = *(uint8_t*)&USARTRecieveBuffer[15];
                if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, *(uint32_t*)&USARTRecieveBuffer[21]) != HAL_OK) Error_Handler();
                break;
            }
            case 7: // DMA2D Fill memory
                //NOTE: expected size 15
                while (DMA2D->CR & DMA2D_CR_START);
                DMA2D->CR = 0x00030000UL | (1 << 9);
                DMA2D->OCOLR = *(uint32_t*)&USARTRecieveBuffer[1];
                DMA2D->OMAR = *(uint32_t*)&USARTRecieveBuffer[5];
                DMA2D->OOR = *(uint32_t*)&USARTRecieveBuffer[9];
                DMA2D->OPFCCR = DMA2D_ARGB8888;
                DMA2D->NLR = (uint32_t)((uint32_t)(*(uint16_t*)&USARTRecieveBuffer[11]) << 16) | *(uint16_t*)&USARTRecieveBuffer[13];
                DMA2D->CR |= DMA2D_CR_START; 
                break;
            case 8: // DMA2D Copy memory
                //NOTE: expected size 19
                while (DMA2D->CR & DMA2D_CR_START);
                DMA2D->CR = 0x00000000UL | (1 << 9);
                DMA2D->FGMAR = *(uint32_t*)&USARTRecieveBuffer[1];
                DMA2D->OMAR = *(uint32_t*)&USARTRecieveBuffer[5];
                DMA2D->FGOR = *(uint32_t*)&USARTRecieveBuffer[9];
                DMA2D->OOR = *(uint32_t*)&USARTRecieveBuffer[13];
                DMA2D->FGPFCCR = DMA2D_ARGB8888;
                DMA2D->OPFCCR = DMA2D_ARGB8888;
                DMA2D->NLR = (uint32_t)((uint32_t)(*(uint16_t*)&USARTRecieveBuffer[15]) << 16) | *(uint16_t*)&USARTRecieveBuffer[17];
                DMA2D->CR |= DMA2D_CR_START;
                break;
            case 9: // DMA2D Mix memory
                //NOTE: expected size 27
                while (DMA2D->CR & DMA2D_CR_START);
                DMA2D->CR = 0x00020000UL | (1 << 9);
                DMA2D->FGMAR = *(uint32_t*)&USARTRecieveBuffer[1];
                DMA2D->BGMAR = *(uint32_t*)&USARTRecieveBuffer[5];
                DMA2D->OMAR = *(uint32_t*)&USARTRecieveBuffer[9];
                DMA2D->FGOR = *(uint32_t*)&USARTRecieveBuffer[13];
                DMA2D->BGOR = *(uint32_t*)&USARTRecieveBuffer[17];
                DMA2D->OOR = *(uint32_t*)&USARTRecieveBuffer[21];
                DMA2D->FGPFCCR = DMA2D_ARGB8888;
                DMA2D->BGPFCCR = DMA2D_ARGB8888;
                DMA2D->OPFCCR = DMA2D_ARGB8888;
                DMA2D->NLR = (uint32_t)((uint32_t)(*(uint16_t*)&USARTRecieveBuffer[23]) << 16) | *(uint16_t*)&USARTRecieveBuffer[25];
                DMA2D->CR |= DMA2D_CR_START;
                break;
            case 10:{ //add a thread
                //NOTE: expected size 9
                if(threadsRoot == NULL){
                    threadsRoot = (struct LinkedSheet*)malloc(sizeof(struct LinkedSheet));
                    ptr = threadsRoot;
                }else{
                    struct LinkedSheet* tail = threadsRoot;
                    while(tail->next != NULL) tail = tail->next;
                    asm volatile("mov %0, sp" :"=r"(ptr->stackPtr));
                    ptr = tail->next = (struct LinkedSheet*)malloc(sizeof(struct LinkedSheet));
                }
                ptr->id = *(uint32_t*)&USARTRecieveBuffer[1];
                ptr->task = (void*)(*(uint32_t*)&USARTRecieveBuffer[5]);
                ptr->memory = ptr->stackPtr = (uint8_t*)malloc(0x200);
                ptr->next = NULL;
                asm volatile("mov sp, %0" ::"r"(ptr->stackPtr));
                ptr->task();
                break;
            }
            case 11:{ //delete a thread
                //NOTE: expected size 5
                struct LinkedSheet* target = threadsRoot;
                if(target == NULL) break;
                if(target->id == *(uint32_t*)&USARTRecieveBuffer[1]){
                    if(ptr == target) ptr = ptr->next;
                    threadsRoot = target->next;
                    free(target->memory);
                    free(target);
                    break;
                }
                while(!target->next && target->next->id != *(uint32_t*)&USARTRecieveBuffer[1]){
                    target = target->next;
                }
                if(target == NULL) break;
                struct LinkedSheet* newNext = target->next->next;
                if(ptr == target->next) ptr = ptr->next;
                free(target->next->memory);
                free(target->next);
                target->next = newNext;
                break;
            }
        }
        HAL_UART_Receive_DMA(&huart1, (uint8_t*)&instructionSize, 2);
    }
    IsPre = !IsPre;
}
