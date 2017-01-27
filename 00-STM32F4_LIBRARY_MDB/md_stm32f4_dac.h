﻿/**
 * @file        md_stm32f4_dac.h
 *
 * @date        26 Jan 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         System Workbench ac6 (eclipse stm32)
 * @stdperiph   STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 * @brief       DESCRIPTION
 *
 * @verbatim

	------------------------------------------------------------------------

	Copyright (C) 2016	Manuel Del Basso

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	------------------------------------------------------------------------

 * @endverbatim
 *
 */
#ifndef __MD_STM32F4_DAC_H_
#define __MD_STM32F4_DAC_H_


/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * @addtogroup MD_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup MD_DAC
 * @brief    DAC library for STM32F4xx - http://stm32f4-discovery.com/2014/04/library-07-da-converter-on-stm32f4xx/
 * @{
 *
 * This library provides 12-bit digital to analog output, values from 0 to 4095
 *
 * @par Pinout
 *
@verbatim
DAC	channel		STM32F4xx

DAC1			PA4
DAC2			PA5
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - March 12, 2015
  - Support for my new GPIO library

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx DAC
 - defines.h
 - TM GPIO
@endverbatim
 */

/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "defines.h"
//#include "tm_stm32f4_gpio.h"

/**
 * @defgroup MD_DAC_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Select which DAC channel
 */
typedef enum {
	MD_DAC1, /*!< DAC channel 1 */
	MD_DAC2  /*!< DAC channel 2 */
} MD_DAC_Channel_t;

/**
 * @}
 */

/**
 * @defgroup MD_DAC_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes DAC channel and it's pin
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref MD_DAC_Channel_t enumeration
 * @retval None
 */
void MD_DAC_Init(MD_DAC_Channel_t DACx);

/**
 * @brief  Sets analog value to ADCx
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref MD_DAC_Channel_t enumeration
 * @param  value: 12-bit unsigned value for 12-bit DAC
 * @retval None
 */
void MD_DAC_SetValue(MD_DAC_Channel_t DACx, uint16_t value);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

///* C++ detection */
//#ifdef __cplusplus
//}
//#endif


#endif /* MD_STM32F4_DAC_H_ */
