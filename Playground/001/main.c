#include <stdio.h>

typedef int TYPE_t(int num);

int times_two(int num);

int main()
{
    TYPE_t *ptr = NULL;
    ptr = times_two;

    int num = (*ptr)(8);
    printf("The num is: %d\n", num);
    return 0;
}

int times_two(int num)
{
    return num * 2;
}