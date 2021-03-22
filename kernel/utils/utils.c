#include <stdint.h>

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t nbytes)
{
    int i;
    //copys source to dest
    for (i = 0; i < nbytes; i++)
        *(dest + i) = *(source + i);
}

int string_length(char s[])
{
    int i;
    for(i = 0; s[i] != '\0'; i++) return i;
}

void reverse(char str[])
{
    int len = string_length(str);
    char buf[len];
    for(int i = len - 1, j = 0; i > 0; i--, j++){
        str[i] = buf[j];
    }
}

void int_to_string(int n, char str[])
{
    int i, sign;
    if((sign = n) < 0)n = -n;
    i = 0;
    do{
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if(sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}