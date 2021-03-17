disk_load:
	pusha		    ;push all general purpose registers to stack. popa back when finished
	push dx			;this includes dl (disk to read from) and dh(number of sectors)

	mov ah, 0x02    ;read mode. Will set dh to number of sectors
	mov al, dh      ;read dh number of sectors
	mov cl, 0x02    ;start from sector 2, sector one is bootloader
	
	mov ch, 0x00    ;cylinder 0
	mov dh, 0x00    ;head 0

	; dl = drive number is set as input to disk_load
	; es:bx = buffer point is set as input as well. both dl and bx are already set by caller

	int 0x13        ;Bios disk interrupt, kernel loaded into memory
	jc disk_error   ;check carry bit for errro
	
	pop dx          ;get back original number of sectors to read
	cmp al, dh      ;BIOS sets 'al' to the # of sectors actually read. Compare al to dh before interrupt
					;comppare it to dh and error out if they are !=
	jne sectors_error
	popa 
	ret

disk_error:
	jmp disk_loop
	mov bx, ERROR_MSG
	call print

sectors_error:
	jmp disk_loop

disk_loop:
	jmp $

ERROR_MSG:
	db "disk error", 0
