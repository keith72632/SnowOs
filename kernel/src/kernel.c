#include "../../drivers/video.h"
#include "../../drivers/port.h"

int main()
{
	int offset;
	port_byte_out(0x3d4, 14); //0b1110

	offset = port_byte_in(0x3d5) << 8; //(3)0b0011 -> (786)0b1100000000

	port_byte_out(0x3d4, 15);//0b1111

	offset += port_byte_in(0x3d5);//(786)0b1100000000 + (112)0b0001110000 = 0b1101110000

	int vga_offset = offset * 2;//==1984 = 0b11111000000

	char * vga = (char *)0xb8000;
	vga[vga_offset] = 'X';
	vga[vga_offset + 1] = 0x02;

	/*
    char string[6] = "hello\n";
    clear_screen();
    print_string("Hello World!\n");

	*/

    return 0;
}

/*
	to debug:
	break main
	break line# of return function
	c
	n
	print when you get to line you want to print have to type out whole line*/
