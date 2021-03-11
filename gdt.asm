;;; gdt_start and gdt_end labels aare used to comppute size

;null segment descriptor
gdt_start:
	dq 0x0

;code segemnt descriptor 
gdt_code:
	dw 0xffff          ;seg len, bits 0-15
	dw 0x0             ;seg base, bits 0-15
	db 0x0             ;seg base, bits 16-23
	db 10011010b       ;flags (8bits)
	db 11001111b       ;flags (4bits) + seg len, bits 16-19
	db 0x0             ;seg base, bits 24-31

;data segmenet descriptor
gdt_data:
	dw 0xffff          ;same as code
	dw 0x0             ;''
	db 0x0             ;''
	db 10010010b       ;flags (8bits)
	db 11001111b       ;flags (4-bits)+segment length, bits 16-19
	db 0x0             ;same as code

gdt_end:

;GDT descriptor
gdt_descriptor:
	dw gdt_end-gdt_start -1; size (16 bit)
	dd gdt_start           ;address (32 bit)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
