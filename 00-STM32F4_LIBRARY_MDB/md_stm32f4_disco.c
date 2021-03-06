﻿/**
 * @file        md_stm32f4_disco.c
 *
 * @date        24 Jan 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         System Workbench ac6 (eclipse stm32)
 * @stdperiph   STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 *
 *
   @verbatim

	------------------------------------------------------------------------

	Copyright (C) 2016  Manuel Del Basso

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

   @endverbatim
 *
 */
#include "md_stm32f4_disco.h"
//#include "md_stm32f4_gpio.h"
#include "stm32f4xx.h"
#include "md_stm32f4_gpio.h"


/* Button pressed status for onpressed/onreleased events */
static volatile uint8_t MD_INT_DISCO_ButtonPressed = 0;

void MD_DISCO_LedInit(void) {
	/* Set pins as output */
	MD_GPIO_Init(MD_DISCO_LED_PORT, LED_ALL, MD_GPIO_Mode_OUT, MD_GPIO_OType_PP, MD_GPIO_PuPd_NOPULL, MD_GPIO_Speed_High);
	
	/* Turn leds off */
	MD_DISCO_LedOff(LED_ALL);
}

void MD_DISCO_ButtonInit(void) {
	/* Set pin as input */
	MD_GPIO_Init(MD_DISCO_BUTTON_PORT, MD_DISCO_BUTTON_PIN, MD_GPIO_Mode_IN,
			MD_GPIO_OType_PP, MD_DISCO_BUTTON_PULL, MD_GPIO_Speed_Low);
}

uint8_t MD_DISCO_ButtonOnPressed(void) {
	/* If button is now pressed, but was not already pressed */
	if (MD_DISCO_ButtonPressed()) {

		if (!MD_INT_DISCO_ButtonPressed) {
			/* Set flag */
			MD_INT_DISCO_ButtonPressed = 1;

			/* Return button onpressed */
			return 1;
		}
	} else {
		/* Clear flag */
		MD_INT_DISCO_ButtonPressed = 0;
	}
	
	/* Button is not pressed or it was already pressed before */
	return 0;
}

uint8_t MD_DISCO_ButtonOnReleased(void) {
	/* If button is now released, but was not already released */
	if (!MD_DISCO_ButtonPressed()) {
		if (MD_INT_DISCO_ButtonPressed) {
			/* Set flag */
			MD_INT_DISCO_ButtonPressed = 0;
			
			/* Return button onreleased */
			return 1;
		}
	} else {
		/* Set flag */
		MD_INT_DISCO_ButtonPressed = 1;
	}
	
	/* Button is not released or it was already released before */
	return 0;
}
