print_hex:
    pusha 
    mov cx, 0

hex_loop:
    cmp cx, 4
    je end
    ;convert last char of dx to ascii
    mov ax, dx
    and ax, 0x00f
    add al, 0x30 ; convert to ascii N
    cmp al, 0x39
    jle step2
    add al, 7   ;'A' is ascii 65 instead of 58, so 65-58 = 7

step2:
    ;get correct position of the string to plce our ASCII char
    ;bx <- base addr + string length - index of char
    mov bx, HEX_OUT + 5; base + length
    sub bx, cx
    mov [bx], al      ;copy ascii char on al to postion pointer to by bx
    ror dx, 4

    add cx, 1
    jmp hex_loop

end:
    mov bx, HEX_OUT
    call print

    popa 
    ret

HEX_OUT:
    db '0x0000', 0
