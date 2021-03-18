/*-Constraints: r is all general purpose registes and =r write only*/

#include <stdio.h>

int main() 
{
	int val = 7;
	int result = 0;
	
	__asm__(
		"movl %1, %%ebx;"
		"addl %%ebx, %%ebx;"
		"movl %%ebx, %0;"
		: "=r" (result)
		: "r" (val)
		: "%ebx"
	);	
	printf("Result = %d\n", result);
	return 0;
}
