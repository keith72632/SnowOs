
extern handler_fun
extern kernel_start

global main


global temp1
global temp2
section .text
main:

stub:
    mov eax, dword 10
    mov ebx, dword 11
    mov ecx, dword 12
    push eax
    push ebx
    push ecx
    push esp
    call handler_fun ; takes aregument in a sort of FIFO manner. will print top of stack first
    popa 

temp1:
    push byte 70
    push byte 71
    push esp
    call handler_fun
    popa 

temp2:
    push byte 80
    push byte 81
    push esp
    call handler_fun
    popa

end:
    call kernel_start
    mov eax, 1
    int 0x80
section .data
CNTR db 0