/*Sources:
 * - https://github.com/cfenollosa/os-tutorial
 * - https://en.wikipedia.org/wiki/Programmable_interrupt_controller
 * */

#include "../../drivers/video.h"
#include "../../drivers/port.h"
#include "../../cpu/idt.h"
#include "../../cpu/isr.h"
#include "../../cpu/timer.h"
#include "../../drivers/keyboard.h"

int start_kernel()
{
    clear_screen();
	print_string("Installing interrupt service routines (ISRs).\n");
	isr_install();

	print_string("Enabling external interrupts.\n");
	asm volatile ("sti");

	print_string("Initializing Keyboard (IRQ 1).\n");
	init_keyboard();

	print_string("********************************************************************************\n");
	print_string("**********                          SNOW OS                           **********\n");
	print_string("********************************************************************************\n");
	print_nl();
    return 0;
}

/*
	to debug:
	break main
	break line# of return function
	c
	n
	print when you get to line you want to print have to type out whole line*/
