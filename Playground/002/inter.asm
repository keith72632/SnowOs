
extern handler_fun
extern kernel_start

global main


global temp1
global temp2
section .text
main:

stub:
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0
    push byte 0

    push esp
    call handler_fun ; takes aregument in a sort of FIFO manner. will print top of stack first
    popa 

temp1:
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push byte 1
    push esp
    call handler_fun
    popa 

temp2:
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push byte 2
    push esp
    call handler_fun
    popa

end:
    call kernel_start
    mov eax, 1
    int 0x80
section .data
CNTR db 0