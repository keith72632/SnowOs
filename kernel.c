#include <stdio.h>

void write_string(int color, const char * string);
unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

int main()
{
    char string[6] = "hello\0";
    write_string(0x1f, string);   
//   *((char *)0x000b8002) = 'K';
    return 0;
}

void write_string(int color, const char * string)
{
    volatile char * video_memory = (char *)0x000b8000;
    while(*string != 0){
        *video_memory++ = *string++;
        *video_memory++ = color;
    }
}

unsigned char port_byte_in(unsigned short port)
{
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}
