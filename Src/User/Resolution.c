#include "Resolution.h"
#include "MemoryRegion.h"

const Resolution r480x272 = {480, 272, 41, 10, 4, 4};
const Resolution r640x480 = {640, 480, 96, 2, 64, 2};
const Resolution r800x480 = {800, 480, 128, 2, 128, 43};
const Resolution r800x600 = {800, 600, 128, 4, 128, 24};
const Resolution r1024x600 = {1024, 600, 136, 4, 184, 24};
const Resolution r1024x768 = {1024, 768, 136, 6, 184, 32};
const Resolution r1280x720 = {1280, 720, 40, 5, 330, 25};
const Resolution r1920x1080 = {1920, 1080, 44, 5, 236, 40};
Resolution* resolutions[8] = {&r480x272, &r640x480, &r800x480, &r800x600, &r1024x600, &r1024x768, &r1280x720, &r1920x1080};

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
    LTDC_LayerCfgTypeDef pLayerCfg = {0};
    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = 0;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = 0;
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