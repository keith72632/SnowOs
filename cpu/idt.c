/*Interrupt Descriptor Table
 * - Each interrupt consists of 256 descriptor entries call gates
 * - Each gate is 64bits
 * - 3 types: interrupt traps and tasks
 * - trap and interrupt invoke handlers, with interrupts temporarily disabling hardware
 * - interrupt gates consist of:
 *		-offset(0-15)(48-63): 32 two bits split in half, 16 on front of gate and 16 on back
 *      	-represents memory addrr
 * 		-selector(16-31): 16 bit selector of code seg to jump to when invoking handler
 *		-Type(40-42): 3 bits indicating type(interrupt, task or trap). Interrupt is 110
 *		-D(43): 1 bit indicating wether code seg is 32 bit or not. set to 1 for true
 *		-DPL(45, 46): 2 bits representing priviledge level. set to 00
 * 		-P(47): 1 bit indictating if gate is active. set to one for true
 *		-0(32-39)(44): some bits that always need to be set to zero for interrupt
 */

#include "idt.h"
#include "../kernel/utils/utils.h"
/*256 gates of IDT*/
idt_gate_t idt[256];

idt_register_t idt_reg;

/*These two helper fucntions will take 32 bit memory addess(IDT offset) and split it*/
#define low_16(address) (uint16_t)((address) & 0xFFFF)  //0b1111111111111111
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS; //0x08 OR 0b1000
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void load_idt() {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    asm ("lidt (%0)" : : "r" (&idt_reg));
}

