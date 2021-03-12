#include <stdio.h>

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

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

void set_cursor(int offset)
{
    //memory offset is double cursor offset
    offset/= 2;
    //data from out register 0x3d4 on port 0x0e
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    //data from register 0x3d5 port right shfited one byte             
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
    //data from register 0x3d4 on port 0x0f
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    //data from register 0c3d5 set to 0b110000
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

int get_cursor()
{
    //data from register 0x3d4 on port 0x0e
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    //sets data IN register 0x3d5 left shifter by one byte
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    //data FROM register 0x3d4 to 0x0f
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    //adds data FROM register 0x3d5 to offset which is data from same register but left shifter a byte
    offset += port_byte_in(VGA_DATA_REGISTER);
    //cursor offset is half of memeory offset
    return offset * 2;
}