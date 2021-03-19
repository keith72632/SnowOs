/*Interrupt Service Routines(ISRs)||Interrupt Handlers for 8259
 * - first 32 ISR interrut handlers are resevered for CPU specific instruction like exceptions and faults
 * - isr_handler extracts all info from the interrupt
 * - https://en.wikipedia.org/wiki/Programmable_interrupt_controller
 * - https://wiki.osdev.org/PIC
 * - https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C
 * */
#include "isr.h"
#include "idt.h"
#include "../drivers/video.h"
#include "../drivers/port.h"
#include "../kernel/utils/utils.h"

/*Array of structure registers_t;
int isr.h: typedef void (*isr_t)(registers_t *);*/
isr_t interrupt_handlers[256];

void isr_install() {
    /*First 32 ISR interrupts
     *idt[index] represents each gate
     *These two helper fucntions will take 32 bit memory addess(IDT offset) and split it
     *#define low_16(address) (uint16_t)((address) & 0xFFFF)  //0b1111111111111111
     *#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

     *void set_idt_gate(int n, uint32_t handler) {
     *idt[n].low_offset = low_16(handler);
     *idt[n].sel = KERNEL_CS; //0x08 OR 0b1000
     *idt[n].always0 = 0;
     *idt[n].flags = 0x8E;
     *idt[n].high_offset = high_16(handler);
}
     *This x86 system, the 8259 Programmable Interrupt Controller is responsibles for hardware interrupts
     *  */
    set_idt_gate(0, (uint32_t) isr0);
    set_idt_gate(1, (uint32_t) isr1);
    set_idt_gate(2, (uint32_t) isr2);
    set_idt_gate(3, (uint32_t) isr3);
    set_idt_gate(4, (uint32_t) isr4);
    set_idt_gate(5, (uint32_t) isr5);
    set_idt_gate(6, (uint32_t) isr6);
    set_idt_gate(7, (uint32_t) isr7);
    set_idt_gate(8, (uint32_t) isr8);
    set_idt_gate(9, (uint32_t) isr9);
    set_idt_gate(10, (uint32_t) isr10);
    set_idt_gate(11, (uint32_t) isr11);
    set_idt_gate(12, (uint32_t) isr12);
    set_idt_gate(13, (uint32_t) isr13);
    set_idt_gate(14, (uint32_t) isr14);
    set_idt_gate(15, (uint32_t) isr15);
    set_idt_gate(16, (uint32_t) isr16);
    set_idt_gate(17, (uint32_t) isr17);
    set_idt_gate(18, (uint32_t) isr18);
    set_idt_gate(19, (uint32_t) isr19);
    set_idt_gate(20, (uint32_t) isr20);
    set_idt_gate(21, (uint32_t) isr21);
    set_idt_gate(22, (uint32_t) isr22);
    set_idt_gate(23, (uint32_t) isr23);
    set_idt_gate(24, (uint32_t) isr24);
    set_idt_gate(25, (uint32_t) isr25);
    set_idt_gate(26, (uint32_t) isr26);
    set_idt_gate(27, (uint32_t) isr27);
    set_idt_gate(28, (uint32_t) isr28);
    set_idt_gate(29, (uint32_t) isr29);
    set_idt_gate(30, (uint32_t) isr30);
    set_idt_gate(31, (uint32_t) isr31);

    /* Remap the PIC 
     *Primary PIC: port 0x20 is command and 0x21 is data
     *Secondary PIC  uses ports 0xA0(command) and 0xA1(data)
     *This programming happens by sending four command words (ICWs)
     *Then wait for the followign three inputs on the data ports:
     * -ICW2(IDT offset) will be set to 0x20 (32) for primary and 0x28 for secondary
     * -ICW3(wiring between PICs) tels primary PIC to accept IRQs from from secondary PIC on IRQ2 (0x04, 0b00000100)
     *      -secondary PIC will be marked as secodar by setting 0x02 = 0b00000010
     * -ICW4(mode) set 0x01 = 0b00000001 in order to enebale 8086 mode
     * -Finally, send (OCW1)0x00 = 0b0000000000 to enable all IRQs (no masking)
     */

    //ICW1(tells PIC to wait for three other input words. Mandatory)
    port_byte_out(0x20, 0x11); //starts intitalization in cascade mode
    port_byte_out(0xA0, 0x11);
    //ICW2 (vector offset)
    port_byte_out(0x21, 0x20);//ICW2 Master PIC offset
    port_byte_out(0xA1, 0x28);//ICW2 Slace PIC offset
    //ICW3 (tell PIC how its wired to master/slaves)
    port_byte_out(0x21, 0x04); //Tell master there is slave PIC at IRQ2
    port_byte_out(0xA1, 0x02); //Tell slave PIC its cascade identity
    //ICW4 (Gives additional info about environment)
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    //OCW1 (operational command word) enable all IRQs
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);
    /*To disable PIC, write 0xff to both master and slave*/

    // Install the IRQs (interrupt requests)Conncects secondary chip to primary
    //Extension of the IDT by adding gates for IRQ 0-15
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);
    /*Then is interrupt.asm, push these IRQs number as weill as the interrupt to stack before calling irq_common_stub
     *Then call irq_handler*/
    load_idt(); // Load with ASM
}

/* To print the message which defines every exception */
char *exception_messages[] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",

        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",

        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",

        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
};
/*prints info for the interrupt, registers_t in isr.h for deets*/
/*isr_handler essentially prints message from exception messages relative 
 *to the int_no in registers_t *r. This function is called in interrupt.asm
 *arguments to C function need to be on stack before calling, so each gate 
 *needs handler. See interrupt.asm for details
 */
void isr_handler(registers_t *r) {
    print_string("received interrupt: ", GREEN_TEXT);
    char s[3];
    int_to_string(r->int_no, s);
    print_string(s, GREEN_TEXT);
    print_nl();
    print_string(exception_messages[r->int_no], 0x04);
    print_nl();
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

/*This will be called in interrupt.asm
 *retrieve resective hadnler from the array based on the interrupt number invoked with the given registers_t
 *PIC protocol stats we must send an end of interrupt(EOI) command to the invovled PICs(only primary for IRQ 0-7, both for IRQ 8-15)
 required for the PIC to know that the interrupt is handled and can send further interrupts*/
void irq_handler(registers_t *r) {
    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }

    /*PIC has two interrupt status registers. In-Service Register (ISR) and the Interrupt Request Register(IRR) 
     * - ISR tells which interrupts are being serviced, meaning IRQs sent to the CPU
     * - The IRR tells us which interruts have been rasied
     * - IRR send interrupts to CPU, then marked in the ISR
     * - ISR and IRR can be read via the OCW3 command word sent to either Master(0x20) or Slave(0xA0)
     * -read ISR or IRR, write command to COMMAND port, not the data port
     * - read IRR, write 0x0a
     * - read ISR, write 0x0b*/

    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r->int_no >= 40) {
        port_byte_out(0xA0, 0x20); /* follower */
    }
    port_byte_out(0x20, 0x20); /* leader */
}