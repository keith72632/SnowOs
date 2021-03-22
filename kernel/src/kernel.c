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
	short temp;
	temp = 32;
    clear_screen();
	print_string("Installing interrupt service routines (ISRs).\n", WHITE_ON_BLACK);
	isr_install();

	print_string("Enabling external interrupts.\n", WHITE_ON_BLACK);
	asm volatile ("sti");

	print_string("Initializing Keyboard (IRQ 1).\n", WHITE_ON_BLACK);
	init_keyboard();

	print_string("********************************************************************************\n", GREEN_TEXT);
	print_string("**********                          SNOW OS                           **********\n", GREEN_TEXT);
	print_string("********************************************************************************\n", GREEN_TEXT);
	print_nl();
	print_string(">", GREEN_TEXT);
    return 0;
}
/*isr_install->set_idt_gates(index of gate, pointer to function implememnted in asm(isr_n)->sets a couple bytes in assembly then jumps to top of file and calls isr_handler->
 *same for irq_handler, except it */
/*
	to debug:
	break main
	break line# of return function
	c
	n
	print when you get to line you want to print have to type out whole line*/
