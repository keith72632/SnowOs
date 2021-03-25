#ifndef _VIDEO_H
#define _VIDEO_H

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define GREEN_TEXT 0x02
#define COLOR GREEN_TEXT

void print_string(char * string);
void clear_screen();
void print_nl();
int scroll_ln(int offset);
void set_char_at_video_memory(char character, int offset, int color);
int get_cursor();
void set_cursor(int offset);
char get_char_from_video_memory(int offset);




#endif