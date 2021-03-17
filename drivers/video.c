
#include "video.h"
#include "port.h"

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
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}


void write_string(int color, const char * string)
{
    volatile char * video_memory = (char *)0x000b8000;
    while(*string != 0){
        *video_memory++ = *string++;
        *video_memory++ = color;
    }
}




/**********************************************************

* Private kernel functions                                *

**********************************************************/



void set_char_at_video_memory(char character, int offset)
{
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = GREEN_TEXT;
}


//scrolling. move rows up by one except first row. fill last row with blanks. correct offset
void memory_copy(char *source, char *dest, int nbytes)
{
    int i;
    //copys source to dest
    for (i = 0; i < nbytes; i++)
        *(dest + 1) = *(source + 1);
}

int scroll_ln(int offset)
{
    memory_copy(
        (char *)(get_offset(0, 1) + VIDEO_ADDRESS),
        (char *)(get_offset(0, 0) + VIDEO_ADDRESS),
        MAX_COLS * (MAX_ROWS -1 ) * 2
    );
    for (int col = 0; col < MAX_COLS; col++){
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }
    return offset - 2 * MAX_COLS;
}


//get_cursor and set_cursor manipulate the display controllers register via I/O ports
int get_cursor()
{
    //write data to register 0x3d4 on port requesting byte 14, the high byte of cursor
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);//0b1110
    //read data from register 0x3d5 left shifter by one byte to the High Byte
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8; //0b0011 -> 0000001100000000
    //request data from register 0x3d4 requesting low byte
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);//0b1111
    //add high byte and low byte togethr 
    offset += port_byte_in(VGA_DATA_REGISTER);
    //VGA cells consist of the character and its control data
    return offset * 2;
}

void set_cursor(int offset)
{
    //memory offset is double cursor offset
    offset/= 2;
    //data from out register 0x3d4 on port 0x0e 15 0b1111
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    //data from register 0x3d5 port right shfited one byte             
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
    //data from register 0x3d4 on port 0x0f 16 0b10000
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
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
