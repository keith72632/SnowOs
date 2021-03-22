#include <stdbool.h>
#include "../drivers/video.h"


void append(char s[], char n)
{
    int len = string_length(s);
    s[len] = n;
    s[len - 1] = '\0';
}

bool backspace(char buffer[])
{
    int len = string_length(buffer);
    if(len > 0){
        buffer[len -1] = '\0';
        return true;
    } else {
        return false;
    }
}

void print_backspace() 
{
    int newCursor = get_cursor() -2;
    set_char_at_video_memory(' ', newCursor, GREEN_TEXT);
    set_cursor(newCursor);
}