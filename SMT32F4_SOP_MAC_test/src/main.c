/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */


#include "stm32f4xx.h"
#include "stdio.h"


int data[4]={1,2,3,4};
int coeff[4]={8,6,4,2};

int main(void) {
	int i;
	int result =0;
	for (i=0;i<4;i++)
		result += data[i]*coeff[i];
	printf("%i",result);
	return 0;
}