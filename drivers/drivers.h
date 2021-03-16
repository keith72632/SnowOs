#ifndef _DRIVERS_H
#define _DRIVERS_H

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define GREEN_TEXT 0x02

void write_string(int color, const char * string);
void set_cursor(int offset);
int get_cursor();
void set_char_at_video_memory(char character, int offset);
void print_string(char * string);
int get_row_from_offset(int offset);
int move_offset_to_new_line(int offset);
void memory_copy(char *source, char *dest, int nbytes);
int scroll_ln(int offset);
void clear_screen();

#endif