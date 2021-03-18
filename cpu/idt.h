/*Interrupt Descriptor Table
 * - Consists of 256 descriptor entries call gates
 * - Each gate is 64bits
 * - 3 types: interrupt traps and tasks
 * - trap and interrupt invoke handlers, with interrupts temporarily disabling hardware
 * - interrupt gates consist of:
 *      -offset(0-15)(48-63): 32 two bits split in half, 16 on front of gate and 16 on back
 *          -represents memory addrr
 *      -selector(16-31): 16 bit selector of code seg to jump to when invoking handler
 *      -Type(40-42): 3 bits indicating type(interrupt, task or trap). Interrupt is 110
 *      -D(43): 1 bit indicating wether code seg is 32 bit or not. set to 1 for true
 *      -DPL(45, 46): 2 bits representing priviledge level. set to 00
 *      -P(47): 1 bit indictating if gate is active. set to one for true
 *      -0(32-39)(44): some bits that always need to be set to zero for interrupt
 */


#pragma once

#include <stdint.h>

/* Segment selectors */
#define KERNEL_CS 0x08

/* How every interrupt gate (handler) is defined */
typedef struct {
    //all variables add up to 64bits
    uint16_t low_offset; /* Lower 16 bits of handler function address */
    uint16_t sel; /* Kernel segment selector */
    uint8_t always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    uint8_t flags;
    uint16_t high_offset; /* Higher 16 bits of handler function address */
} __attribute__((packed)) idt_gate_t;

/* A pointer to the array of interrupt handlers.
     * Assembly instruction 'lidt' will read it */
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256

void set_idt_gate(int n, uint32_t handler);

void load_idt();