/**
 *	Keil project for XY-GalvoScanner
 *  29-04-2015
 *
 *
 *	@author		Manuel Del Basso
 *	@email		Manuel.DelBasso@googlemail.com  
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
 
#ifndef ADC_DAC_DMA_H_
#define ADC_DAC_DMA_H_  100

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dac.h"
#include "defines.h"

#include "tm_stm32f4_dac.h"

// ==============================================================
//                          20-05-2015
// ==============================================================

// Setpoint (external) 
// Auf prototyp Signal v1.3 wurde dazu einer der
// I_Sens_X/Y Kan�le benutzt 
/*
 CHAN   ADC1    ADC2    ADC3
 ----------------------------
    0    PA0    PA0    PA0
    1    PA1    PA1    PA1
    2    PA2    PA2    PA2
    3    PA3    PA3    PA3
    4    PA4    PA4    PF6
    5    PA5    PA5    PF7
    6    PA6    PA6    PF8
    7    PA7    PA7    PF9
    8    PB0    PB0    PF10
    9    PB1    PB1    PF3
    10   PC0    PC0    PC0
    11   PC1    PC1    PC1
    12   PC2    PC2    PC2
    13   PC3    PC3    PC3
    14   PC4    PC4    PF4
    15   PC5    PC5    PF5

    Signal v1.3 proto
    ---------------
    PC3     Isens_X
    PA3     Isens_Y
    PA6     pos_X
----P-A-7---p-o-s_-Y--
    PB0     pos_Y   

    Bei ADC dual-mode:
    ADC1_CH06    pos_X  (PA6)
    ADC2_CH07    pos_Y  (PB0)

    Zum testen, triplemode (prototyp v1.3)
    ADC3_CH03    setpoint=Isens_X  (PA3)

    Zum testen, dualmode (prototyp v1.3)
    ADC2_CH03    setpoint=Isens_X  (PA3)

*/
// ==============================================================
//	                    ADC things
// ==============================================================
#define ADC_X_CHAN      ADC_Channel_6   // process y output (ADC)
#define ADC_Y_CHAN      ADC_Channel_8   // process y output (ADC)
#define ADC_Ix_CHAN     ADC_Channel_13  // I_sens_x
#define ADC_Iy_CHAN     ADC_Channel_3   // I_sens_y
#define ADC_W_CHAN      ADC_Channel_3   // Setpoint (external)

#define ADC_CCR_ADDRESS    ((uint32_t)0x40012308)
// ==============================================================


// ==============================================================
// Warum define geändert? 01-06-2015 @@@MDB
// ?????????????????????????????????????????
// ==============================================================
//#define DAC_Align_12b_R  CHAN_Align_12b_R
// ==============================================================



extern struct autoSaveSystem ass;

/* Sets the ADC scan channels count 
 * This MUST be used in ADC init function, DMA init function and at the point 
 * where a complete buffer copy was taken */    
#define ADC_N_REGULAR_CHANNELS  2

/**< enumerate return types
 * Declare enumerations used as return type of function pointer 
 * DAC_SecureSetDualChanSigned(). Polling the return argument of 
 * DAC_SecureSetDualChanSigned() could be used to determine, where the
 * the write pointer points to!. */
typedef enum {
    DEFAULT_WRITE_DAC,      ///< return by default function which is used for write access to DAC hardware
    TRIPPED_WRITE_DAC      ///< return by error handling function if ass is in tripped state
} DAC_WP_t;


void NVIC_Configuration(void);  
void TIM2_DMA_triggerConfiguration(FunctionalState TimerRun, \
                                        FunctionalState IntOn, \
                                        uint16_t peri);

void DMA_Configuration( __IO int16_t *MultiConvBuff, \
                                             uint8_t memSize);
void AnalogWatchdog_Configuration (void);
void ADC_Configuration(void);
void RCC_Configuration(void);

void ADC_DMA_DualModeConfig(__IO int16_t *MultiConvBuff);
void ADC_ContScanMode_w_DMA (__IO int16_t *MultiConvBuff);
void ADC_ContScanMode_w_DMA_timeTrigd (__IO int16_t *MultiConvBuff, uint8_t memSize);
void DAC_SetSignedValue(TM_DAC_Channel_t DACx, int16_t val);

/* extern defined function pointer */
extern  DAC_WP_t (*DAC_SecureSetDualChanSigned) (int16_t, int16_t);
/* Primary function that (*DAC_SecureSetDualChanSigned) is pointing to */
        DAC_WP_t DAC_SetDualChanSigned_Tripped  (int16_t Data2, int16_t Data1);
/* Error condition function that (*DAC_SecureSetDualChanSigned) is pointing to */
        DAC_WP_t DAC_SetDualChanSigned          (int16_t Data2, int16_t Data1);

#endif
