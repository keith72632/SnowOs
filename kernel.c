#include <stdio.h>

void write_string(int color, const char * string)
{
    volatile char * video_memory = (char *)0x000b8000;
    while(*string != 0){
        *video_memory++ = *string++;
        *video_memory++ = color;
    }
}

int main()
{
    char string[6] = "hello\0";
    write_string(0x1f, string);   
//   *((char *)0x000b8002) = 'K';
    return 0;
}
