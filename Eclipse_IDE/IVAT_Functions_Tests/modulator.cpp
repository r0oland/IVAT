/*
 * modulator.c
 *
 *  Created on: 24.02.2011
 *      Author: r0oland
 */
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main()
{
	while(1)
	{
		int count = 0;
		unsigned long delayTimeTemp[] =
		{ 0, 0, 0, 0, 0};

		printf("Please enter your time in seconds!\n");
		scanf("%lu", &delayTimeTemp);
		printf("You entered: %lu\n", delayTimeTemp[0]);
		modulator(delayTimeTemp);
		while(count++ < 5)
			printf("[%d]=%lu ", (count-1), delayTimeTemp[count-1]);
		printf("\n");
	}

	return(0);
}
