#include <stdio.h>
#include <stdint.h>

#define lower_16(handler) (uint16_t)((handler) & 0xffff)
#define higher_16(handler) (uint16_t)(((handler) >> 16) &0xffff)

typedef struct{
    uint16_t low;
    uint16_t selector;
    uint8_t all0;
    uint8_t flagss;
    uint16_t high;
}__attribute__((packed)) gate_t;


typedef struct {
    /* Data segment selector */
    uint32_t ds; 
    /* Pushed by pusha. */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; 
    
    /* Interrupt number and error code (if applicable) */
    uint32_t int_no, err_code; 
    /* Pushed by the processor automatically */
    uint32_t eip, cs, eflags, useresp, ss; 
} registers_t;


extern void temp1();
extern void temp2();

gate_t gates[2];

void handler_fun(registers_t *reg){
    printf("Ds:            %d\n", reg->ds);

    printf("edi:           %d\n", reg->edi);
    printf("esi:           %d\n", reg->esi);
    printf("\n");
    /*
    printf("ebp:           %d\n", reg->ebp);
    printf("esp:           %d\n", reg->esp);
    printf("ebx:           %d\n", reg->ebx);
    printf("edx:           %d\n", reg->edx);
    printf("ecx:           %d\n", reg->ecx);
    printf("eax:           %d\n", reg->eax);

    printf("Int_no:        %d\n", reg->int_no);
    printf("err_code:      %d\n", reg->err_code);


    printf("eip:           %d\n", reg->eip);
    printf("err_code:      %d\n", reg->cs);

    printf("eflags:        %d\n", reg->eflags);
    printf("useresp:       %d\n", reg->useresp);
    printf("ss:            %d\n", reg->ss);
*/

}

void set_gate(int n, uint32_t handler)
{
    gates[n].low = lower_16(handler);
    gates[n].selector = 8;
    gates[n].all0 = 0;
    gates[n].flagss = 0x8e;
    gates[n].high = higher_16(handler);
}

void install()
{
    set_gate(1, (uint32_t)temp1);
    set_gate(2, (uint32_t)temp2);
}

void print_gate_info(gate_t *gates)
{
    printf("First gate info:\n");
    printf("%d\n", gates[0].low);
    printf("%d\n", gates[0].selector);
    printf("%d\n", gates[0].all0);
    printf("%d\n", gates[0].flagss);
    printf("%d\n", gates[0].high);
}


int kernel_start()
{
    install();
    print_gate_info(gates);
    uint32_t n1 = 0xF0E639BB; //4041619899
    
    uint16_t low = n1 & 0xffff;
    uint16_t high = (n1 >> 16) & 0xffff;
    printf("lower 16 bits of 174d: %x\n", low);
    printf("higher 16 bits of 174d: %x\n", high);

    return 0;
}