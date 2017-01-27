﻿#ifndef __MD_STM32F4xx_DAC_H
#define __MD_STM32F4xx_DAC_H

#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "md_stm32f4_dac.h"

/** @addtogroup DAC_SignalGeneration
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* 
 * DM00031020.pdf 14.5.15 DAC register map.
 */
#define DAC_DHR12R1_ADDRESS    0x40007408
#define DAC_DHR12R2_ADDRESS    0x40007414
#define DAC_DHR8R1_ADDRESS     0x40007410
#define DAC_DHR8R2_ADDRESS     0x4000741C


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_InitTypeDef     DAC_InitStructure;
DMA_InitTypeDef     DMA_InitStructure;


void DAC_DMA_SineWaveConfig(MD_DAC_Channel_t DACx,
                            FunctionalState NewStateDAC, 
                            FunctionalState NewStateDMA);

void DAC_Chx_NoiseConfig(MD_DAC_Channel_t DACx);
void DAC_TriangleConfig(MD_DAC_Channel_t DACx);
void DAC_Chx (MD_DAC_Channel_t DACx);

#endif
