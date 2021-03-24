#include "string.h"
#include "../drivers/video.h"

void execute_command(char *input)
{
    if(compare_string(input, "EXIT") == 0){
        clear_screen();
        print_string("Stopping the CPU. Fuck Off\n");
        asm volatile("hlt");
    } else if(compare_string(input, "WHOAMI")==0){
        print_string("User: Keith\n");
        print_string(">");
    }else{
        print_string("Unkown Command: ");
        print_string(input);
        print_string("\n>");
    }

}