
#include "video.h"
#include "port.h"
#include "../kernel/utils/utils.h"
#include <stdint.h>

void write_string(int color, const char * string);
void set_cursor(int offset);
int get_cursor();
int get_offset(int col, int row);
void set_char_at_video_memory(char character, int offset);
int get_row_from_offset(int offset);
int move_offset_to_new_line(int offset);
void memory_copy(char *source, char *dest, int nbytes);
int scroll_ln(int offset);
void clear_screen();
/**********************************************************

* Public Kernel Functions                                 *

**********************************************************/


void print_string(char * string)
{
    int offset = get_cursor();
    int i = 0;
    while(string[i] != 0){
        if(offset >= MAX_ROWS * MAX_COLS * 2){
            offset = scroll_ln(offset);
        }
        if(string[i] == '\n'){
            offset = move_offset_to_new_line(offset);
        }else{
            set_char_at_video_memory(string[i], offset);
            //move offset 2 bytes to next character cell
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}


/**********************************************************

* Private kernel functions                                *

**********************************************************/

void print_nl() 
{
    int offset = move_offset_to_new_line(get_cursor());
    if(offset >= MAX_ROWS * MAX_COLS * 2){
        offset = scroll_ln(offset);
    }
    set_cursor(offset);
}


void set_char_at_video_memory(char character, int offset)
{
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = 0b00000010;
}


//scrolling. move rows up by one except first row. fill last row with blanks. correct offset


int scroll_ln(int offset)
{
    memory_copy(
        (uint8_t *)(get_offset(0, 1) + VIDEO_ADDRESS),
        (uint8_t *)(get_offset(0, 0) + VIDEO_ADDRESS),
        MAX_COLS * (MAX_ROWS -1 ) * 2
    );
    for (int col = 0; col < MAX_COLS; col++){
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }
    return offset - 2 * MAX_COLS;
}


/*
get_cursor and set_cursor manipulate the display controllers register via I/O ports
cursor postion 16 bit. data register(0x3d5) will hold low byte if control register(0x3d4) is set to 15(0x0f),
and the high byte if the value is set to 14 (0x0e). cursor offset represent video offset/2
*/
int get_cursor()
{
    //write data to register 0x3d4 on port requesting byte 14, the high byte of cursor
    port_byte_out(VGA_CTRL_REGISTER, 0b1110);//14 == 0b1110
    //read data from register 0x3d5 left shifter by one byte to the High Byte
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8; //0b0011 -> 0000001100000000
    //request data from register 0x3d4 requesting low byte (15)
    port_byte_out(VGA_CTRL_REGISTER, 0b1111);//0b1111
    //add high byte and low byte togethr 
    offset += port_byte_in(VGA_DATA_REGISTER);//(786)0b1100000000 + (112)0b0001110000 = 0b1101110000
    //VGA cells consist of the character and its control data
    return offset * 2;
}

void set_cursor(int offset)
{
    //memory offset is double cursor offset
    offset/= 2;
    //request data from register 0x3d4 on port 0x0e 14 0b1110
    port_byte_out(VGA_CTRL_REGISTER, 0b1110);
    //request data from register 0x3d5 port right shfited one byte             
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
    //request data from register 0x3d4 on port 0x0f 15 0b1111
    port_byte_out(VGA_CTRL_REGISTER, 0b1111);
    //data from register 0c3d5 set to 0b110000
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    for(int i = 0; i < MAX_COLS * MAX_ROWS; i++){
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}


//map row and column coordinates to memory offset of particular display char
//each cell holds two bytes
int get_offset(int col, int row)
{
    return 2 * (row * MAX_COLS + col);
}

//newline characters
int get_row_from_offset(int offset)
{
    return offset / (2 * MAX_COLS);
}


int move_offset_to_new_line(int offset)
{
    return get_offset(0, get_row_from_offset(offset) + 1);
}
