#include "../../drivers/video.h"
#include "../../drivers/port.h"

int main()
{
    char string[12] = "hello world!";
    //clear_screen();
    print_string(string);

    return 0;
}

/*
	to debug:
	break main
	break line# of return function
	c
	n
	print when you get to line you want to print have to type out whole line*/
