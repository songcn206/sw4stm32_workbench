﻿#include "md_stm32f4_gpio.h"
//#include "stm32f4xx.h"

/* Private function */
static uint16_t GPIO_UsedPins[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Private functions */
void MD_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
				  MD_GPIO_Mode_t GPIO_Mode, MD_GPIO_OType_t GPIO_OType,
				  MD_GPIO_PuPd_t GPIO_PuPd, MD_GPIO_Speed_t GPIO_Speed) {

	if (GPIO_Pin == 0x00) {
		return;
	}

	/* Enable clock for GPIO */
	MD_GPIO_INT_EnableClock(GPIOx);

	/* Do initialization */
	MD_GPIO_INT_Init(GPIOx, GPIO_Pin, GPIO_Mode, GPIO_OType, GPIO_PuPd, GPIO_Speed);
}

void MD_GPIO_InitAlternate(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
						   MD_GPIO_OType_t GPIO_OType, MD_GPIO_PuPd_t GPIO_PuPd,
						   MD_GPIO_Speed_t GPIO_Speed,
						   uint8_t Alternate) {

	/* Local variable for shift operations */
	uint32_t pinpos;

	/* Return if pin mask validates to zero */
	if (GPIO_Pin == 0x00)
		return;

	/* Enable GPIOx clock */
	MD_GPIO_INT_EnableClock(GPIOx);

	/* Set alternate functions for all pins */
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		/* Check pin */
		if ((GPIO_Pin & (1 << pinpos)) == 0) {
			continue;
		}

		/* Set alternate function */
		GPIOx->AFR[pinpos >> 0x03] =
		   (GPIOx->AFR[pinpos >> 0x03] & ~(0x0F << (4 * (pinpos & 0x07)))) |
		   (Alternate << (4 * (pinpos & 0x07)));
	}

	/* Do initialization */
	MD_GPIO_INT_Init(GPIOx, GPIO_Pin, MD_GPIO_Mode_ALT, GPIO_OType, GPIO_PuPd,
					 GPIO_Speed);
}

void MD_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;
	uint8_t ptr = MD_GPIO_GetPortSource(GPIOx);

	/* Go through all pins */
	for (i = 0x00; i < 0x10; i++) {
		/* Pin is set */
		if (GPIO_Pin & (1 << i)) {
			/* Set 11 bits combination for analog mode */
			GPIOx->MODER |= (0x03 << (2 * i));

			/* Pin is not used */
			GPIO_UsedPins[ptr] &= ~(1 << i);
		}
	}
}

void MD_GPIO_SetPinAsInput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;

	/* Go through all pins */
	for (i = 0x00; i < 0x10; i++) {
		/* Pin is set */
		if (GPIO_Pin & (1 << i)) {
			/* Set 00 bits combination for input */
			GPIOx->MODER &= ~(0x03 << (2 * i));
		}
	}
}

void MD_GPIO_SetPinAsOutput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;

	/* Go through all pins */
	for (i = 0x00; i < 0x10; i++) {
		/* Pin is set */
		if (GPIO_Pin & (1 << i)) {
			/* Set 01 bits combination for output */
			GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x01 << (2 * i));
		}
	}
}

void MD_GPIO_SetPinAsAnalog(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;

	/* Go through all pins */
	for (i = 0x00; i < 0x10; i++) {
		/* Pin is set */
		if (GPIO_Pin & (1 << i)) {
			/* Set 11 bits combination for analog mode */
			GPIOx->MODER |= (0x03 << (2 * i));
		}
	}
}

void MD_GPIO_SetPinAsAlternate(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;

	/* Set alternate functions for all pins */
	for (i = 0; i < 0x10; i++) {
		/* Check pin */
		if ((GPIO_Pin & (1 << i)) == 0) {
			continue;
		}

		/* Set alternate mode */
		GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x02 << (2 * i));
	}
}

void MD_GPIO_SetPullResistor(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
							 MD_GPIO_PuPd_t GPIO_PuPd) {
	uint8_t pinpos;

	/* Go through all pins */
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		/* Check if pin available */
		if ((GPIO_Pin & (1 << pinpos)) == 0) {
			continue;
		}

		/* Set GPIO PUPD register */
		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(
			  GPIO_PuPd <<
			  (2 * pinpos)));
	}
}

void MD_GPIO_Lock(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	uint32_t d;

	/* Set GPIO pin with 16th bit set to 1 */
	d = 0x00010000 | GPIO_Pin;

	/* Write to LCKR register */
	GPIOx->LCKR = d;
	GPIOx->LCKR = GPIO_Pin;
	GPIOx->LCKR = d;

	/* Read twice */
	(void)GPIOx->LCKR;
	(void)GPIOx->LCKR;
}

uint16_t MD_GPIO_GetPinSource(uint16_t GPIO_Pin) {
	uint16_t pinsource = 0;

	/* Get pinsource */
	while (GPIO_Pin > 1) {
		/* Increase pinsource */
		pinsource++;
		/* Shift right */
		GPIO_Pin >>= 1;
	}

	/* Return source */
	return pinsource;
}

uint16_t MD_GPIO_GetPortSource(GPIO_TypeDef *GPIOx) {
	/* Get port source number */
	/* Offset from GPIOA                       Difference between 2 GPIO addresses */
	return ((uint32_t)GPIOx - (GPIOA_BASE)) / ((GPIOB_BASE) - (GPIOA_BASE));
}

/* Private functions */
void MD_GPIO_INT_EnableClock(GPIO_TypeDef *GPIOx) {
	/* Set bit according to the 1 << portsourcenumber */
	RCC->AHB1ENR |= (1 << MD_GPIO_GetPortSource(GPIOx));
}

void MD_GPIO_INT_DisableClock(GPIO_TypeDef *GPIOx) {
	/* Clear bit according to the 1 << portsourcenumber */
	RCC->AHB1ENR &= ~(1 << MD_GPIO_GetPortSource(GPIOx));
}

void MD_GPIO_INT_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, MD_GPIO_Mode_t GPIO_Mode,
		MD_GPIO_OType_t GPIO_OType, MD_GPIO_PuPd_t GPIO_PuPd, MD_GPIO_Speed_t GPIO_Speed) {
	uint8_t pinpos;
	uint8_t ptr = MD_GPIO_GetPortSource(GPIOx);

	/* Go through all pins */
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		/* Check if pin available */
		if ((GPIO_Pin & (1 << pinpos)) == 0) {
			continue;
		}

		/* Pin is used */
		GPIO_UsedPins[ptr] |= 1 << pinpos;

		/* Set GPIO PUPD register */
		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(
			  GPIO_PuPd <<
			  (2 * pinpos)));

		/* Set GPIO MODE register */
		GPIOx->MODER = (GPIOx->MODER & ~((uint32_t)(0x03 << (2 * pinpos)))) | ((
						  uint32_t)(
						  GPIO_Mode << (2 * pinpos)));

		/* Set only if output or alternate functions */
		if (GPIO_Mode == MD_GPIO_Mode_OUT || GPIO_Mode == MD_GPIO_Mode_ALT) {
			/* Set GPIO OTYPE register */
			GPIOx->OTYPER = (GPIOx->OTYPER & ~(uint16_t)(0x01 << pinpos)) | ((uint16_t)(
							   GPIO_OType << pinpos));

			/* Set GPIO OSPEED register */
			GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~((uint32_t)(0x03 << (2 * pinpos)))) | ((
								uint32_t)(
								GPIO_Speed << (2 * pinpos)));
		}
	}
}

uint16_t MD_GPIO_GetUsedPins(GPIO_TypeDef *GPIOx) {
	/* Return used */
	return GPIO_UsedPins[MD_GPIO_GetPortSource(GPIOx)];
}

uint16_t MD_GPIO_GetFreePins(GPIO_TypeDef *GPIOx) {
	/* Return free pins */
	return ~GPIO_UsedPins[MD_GPIO_GetPortSource(GPIOx)];
}
