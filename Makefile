# $@ = target file
# $< = first dependency
# $^ = all dependecny

C_SOURCES = $(wildcard kernel/src/*.c drivers/*.c cpu/*.c kernel/utils/*.c shell/*.c)
HEADERS = $(wildcard drivers/*.h cpu/*.h kernel/utils/*.h shell/*.h) cpu/isr.h
OBJ_FILES = ${C_SOURCES:.c=.o cpu/interrupt.o}
GDB = /home/linuxbrew/.linuxbrew/Cellar/i386-elf-gdb/10.1/bin/i386-elf-gdb
LINK = /usr/local/i386elfgcc/bin/i386-elf-ld
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
CC_FLAGS = -g -m32 -ffreestanding -c -std=gnu99
#first rule is the one exectued when no parameters
all: run

kernel.bin: kernel_entry.o ${OBJ_FILES}
	${LINK} -o $@ -Ttext 0x1000 $^ --oformat binary
kernel_entry.o: boot/kernel_entry.asm
	nasm $< -felf32 -o $@
mbr.bin: boot/mbr.asm
	nasm $< -f bin -o $@
os-image.bin: mbr.bin kernel.bin
	cat $^ > $@
#-fda before bin file to set dl 0x00, -boot after bin file to set dl as 0x80
run: os-image.bin
	qemu-system-i386 -fda $<
iso: os-image.bin
	#rm isodir/boot/os-image.bin
	cp os-image.bin isodir/boot/
	grub-mkrescue -o os-image.iso isodir
	qemu-system-i386 -cdrom os-image.iso
#debug
kernel.elf: kernel_entry.o ${OBJ_FILES}
	${LINK} -m elf_i386 -o $@ -Ttext 0x1000 $^
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
%.o: %.c ${HEADERS}
	${CC} ${CC_FLAGS} $< -o $@ # -g for debugging
%.o: %.asm
	nasm $< -f elf -o $@
%.bin: boot/%.asm
	nasm $< -f bin -o $@
%.dis: %.bin
	ndisasm -b 32 $< > $@
clean:
	$(RM) *.bin *.o *.dis *.elf boot/*.o boot/*.bin drivers/*.o kernel/src/*.o cpu/*.o shell/*.o kernel/utils/*.o *.bin *.iso isodir/boot/os-image.bin
