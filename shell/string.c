#include <stdbool.h>
#include "../drivers/video.h"
#include "../kernel/utils/utils.h"


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

int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void execute_command(char *input)
{
    if(compare_string(input, "E") == 0){
        print_string("Stopping the CPU. Bye\n", GREEN_TEXT);
        asm volatile("hlt");
        print_string("this should be haulting\n", GREEN_TEXT);
    }
    print_string("Unkown Command", GREEN_TEXT);
    print_string(input, GREEN_TEXT);
    print_string("\n>", GREEN_TEXT);
}