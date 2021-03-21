#include <stdio.h>
#include <stdlib.h>
int add(int first, int second){return first + second;}
int sub(int first, int second){return first - second;}
int mult(int first, int second){return first * second;}
int divide(int first, int second){return first / second;}

int array_1[] = {7, 8, 9, 0, 8, 7, 6, 4, 3};
int array_2[] = {4, 3, 7, 8, 5, 9, 2, 3, 5};

typedef int math_t(int first, int second);

int main()
{
	int result, input, i;
	math_t *ptr = NULL;
	printf("Enter corresponding integer with command:\n");
	printf("1. Add\n");
	printf("2. Sub\n");
	printf("3. Mult\n");
	printf("4. Div\n");
	scanf("%d", &input);

	printf("Choice is: %d\n", input);
	switch(input){
		case 1:
			ptr = add;
			break;
		case 2:
			ptr = sub;
			break;
		case 3:
			ptr = mult;
			break;
		case 4:
			ptr = divide;
			break;
		default:
			printf("This program is retarded\n");
			break;
	}
	for(i = 0; i < 7; ++i){
		result = (*ptr)(array_1[i], array_2[i]);
		printf("Result: %d\n", result); 
	}
	
	return 0;	
}
