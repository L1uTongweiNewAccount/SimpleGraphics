#include "Resolution.h"
#include "MemoryRegion.h"

void setResolution(Resolution r){ //Re-init LTDC
    LTDC_HandleTypeDef hltdc;
    hltdc.Instance = LTDC;
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AH;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AH;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AH;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Init.HorizontalSync = r.HSync;
    hltdc.Init.VerticalSync = r.VSync;
    hltdc.Init.AccumulatedHBP = r.HBack;
    hltdc.Init.AccumulatedVBP = r.VBack;
    hltdc.Init.AccumulatedActiveW = r.width;
    hltdc.Init.AccumulatedActiveH = r.height;
    hltdc.Init.TotalWidth = r.HSync + r.HBack + r.width - 1;
    hltdc.Init.TotalHeigh = r.VSync + r.VBack + r.height - 1;
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;
    if (HAL_LTDC_Init(&hltdc) != HAL_OK) Error_Handler();
    // LTDC Clock = 25Mhz x PLL3N / PLL3R
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 1;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    PeriphClkInitStruct.PLL3.PLL3N = r.PLL3N;
    PeriphClkInitStruct.PLL3.PLL3R = r.PLL3R;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) Error_Handler();
    LTDC_LayerCfgTypeDef pLayerCfg = {0};
    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = r.width - 1;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = r.height - 1;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
    pLayerCfg.Alpha = 0;
    pLayerCfg.Alpha0 = 0;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = (uint32_t)ExternalSDRAM;
    pLayerCfg.ImageWidth = r.width;
    pLayerCfg.ImageHeight = r.height;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;
    if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK) Error_Handler();
}