/*
 * struct_array_test.cpp
 *
 *  Created on: 27.02.2011
 *      Author: r0oland
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long DWORD;

struct my_time
{
	DWORD raw_value;
	DWORD days;
	DWORD hours;
	DWORD minutes;
	DWORD seconds;
	int changed;
};

void struct_printer (my_time *array);

int main ()
{
	my_time struct_array[5];
	printf("sizeof DWORD: %d\n", sizeof(DWORD));
	printf("array created!\n");

	for (int i=0;i<5;i++)
	{
		struct_array[i].raw_value = i+1;
		struct_array[i].days = i+2;
		struct_array[i].hours = i+3;
		struct_array[i].minutes = i+4;
		struct_array[i].seconds = i+5;
		struct_array[i].changed = 1;
	}
	printf("array initialzed!\n");

	struct_printer(struct_array);

	return(0);
}

void struct_printer (my_time *array)
{
	for (int i=0;i<5;i++)
		{
			printf("struct_array[%d].raw_value = %lu \n",
					i, array[i].raw_value);
			printf("struct_array[%d].days = %lu \n",
					i, array[i].days);
			printf("struct_array[%d].hours = %lu \n",
					i, array[i].hours);
			printf("struct_array[%d].minutes = %lu \n",
					i, array[i].minutes);
			printf("struct_array[%d].seconds = %lu \n",
					i, array[i].seconds);
		}
		printf("array printed!\n");
}
