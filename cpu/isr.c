#include "idt.h"
#include "isr.h"
#include "../drivers/video.h"
#include "../drivers/port.h"

isr_t interrupt_handlers[256];

void isr_install()
{
    set_id_gate(0, (uint32_t) isr0);
    set_id_gate(1, (uint32_t) isr1);
    set_id_gate(2, (uint32_t) isr2);
    set_id_gate(3, (uint32_t) isr3);
    set_id_gate(4, (uint32_t) isr4);
    set_id_gate(5, (uint32_t) isr5);
    set_id_gate(6, (uint32_t) isr6);
    set_id_gate(7, (uint32_t) isr7);
    set_id_gate(8, (uint32_t) isr8);
    set_id_gate(9, (uint32_t) isr9);
    set_id_gate(10, (uint32_t) isr10);
    set_id_gate(11, (uint32_t) isr11);
    set_id_gate(12, (uint32_t) isr12);
    set_id_gate(13, (uint32_t) isr13);
    set_id_gate(14, (uint32_t) isr14);
    set_id_gate(15, (uint32_t) isr15);
    set_id_gate(16, (uint32_t) isr16);
    set_id_gate(17, (uint32_t) isr17);
    set_id_gate(18, (uint32_t) isr18);
    set_id_gate(19, (uint32_t) isr19);
    set_id_gate(20, (uint32_t) isr20);
    set_id_gate(21, (uint32_t) isr21);
    set_id_gate(22, (uint32_t) isr22);
    set_id_gate(23, (uint32_t) isr23);
    set_id_gate(24, (uint32_t) isr24);
    set_id_gate(25, (uint32_t) isr25);
    set_id_gate(26, (uint32_t) isr26);
    set_id_gate(27, (uint32_t) isr27);
    set_id_gate(28, (uint32_t) isr28);
    set_id_gate(29, (uint32_t) isr29);
    set_id_gate(30, (uint32_t) isr30);
    set_id_gate(31, (uint32_t) isr31);
}

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

void isr_handler(registers_t *r)
{
    print_string(exception_messages[r->int_no]);
}