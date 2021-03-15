[bits 32]
;constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
    pusha 
    mov edx, VIDEO_MEMORY     ;edx start of vid mem

print_string_pm_loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK

    cmp al, 0
    je print_done

    mov [edx], ax             ;stores char and attributes at current character cell

    add ebx, 1                ;increment ebx to the next char in string
    add edx, 2                ;move to next character cell in vid mem

    jmp print_string_pm_loop

print_done:
    popa 
    ret
