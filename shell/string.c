#include <stdbool.h>
#include "../drivers/video.h"
#include "../kernel/utils/utils.h"


void append(char s[], char n, short *index)
{
    s[*index] = n;
    s[*index + 1] = '\0';
    *index+=1;
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
    char character = get_char_from_video_memory(newCursor);
    if(character != '>'){
        set_char_at_video_memory(' ', newCursor, GREEN_TEXT);
        set_cursor(newCursor);
    }
}

int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

