#include <stdint.h>
#include "idt.h"


#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)


idt_gate_t idt[256];
idt_register_t idt_reg;

void set_id_gate(int n, uint32_t handler)
{
    idt[n].low_offset = low_16(handler);
    idt[n].selector = 0x08; //see GDT
    idt[n].always0 = 0;
    //0x8E = 1 00 0 1 or 110
    //       P DPL 0 D  TYPE
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void load_idt()
{
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) -1;
    //dont make mistake of loading &idt == alwayys load &idt_reg
    asm volatile("lidt (%0)": : "r"(&idt_reg));
}