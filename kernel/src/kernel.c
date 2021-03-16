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
void set_cursor(int offset);
int get_cursor();
void set_char_at_video_memory(char character, int offset);
void print_string(char * string);
int get_row_from_offset(int offset);
int move_offset_to_new_line(int offset);
void memory_copy(char *source, char *dest, int nbytes);
int scroll_ln(int offset);
void clear_screen();

int main()
{
    char string[6] = "hello\n";
   clear_screen();
  // write_string(WHITE_ON_BLACK, string);
   print_string("Hello World!\n");
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
    //data from out register 0x3d4 on port 0x0e 15 0b1111
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    //data from register 0x3d5 port right shfited one byte             
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
    //data from register 0x3d4 on port 0x0f 16 0b10000
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

void set_char_at_video_memory(char character, int offset)
{
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

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

//newline characters
int get_row_from_offset(int offset)
{
    return offset / (2 * MAX_COLS);
}

int get_offset(int col, int row)
{
    return 2 * (row * MAX_COLS + col);
}

int move_offset_to_new_line(int offset)
{
    return get_offset(0, get_row_from_offset(offset) + 1);
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

void clear_screen() {
    for(int i = 0; i < MAX_COLS * MAX_ROWS; i++){
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

