# $@ = target file
# $< = first dependency
# $^ = all dependecny

C_SOURCES = $(wildcard kernel/src/*.c drivers/*.c)
HEADERS = $(wildcard drivers/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o}
GDB = /home/linuxbrew/.linuxbrew/Cellar/i386-elf-gdb/10.1/bin/i386-elf-gdb
#first rule is the one exectued when no parameters
all: run

kernel.bin: kernel_entry.o ${OBJ_FILES}
	/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-binutils/2.36.1/bin/x86_64-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
kernel_entry.o: boot/kernel_entry.asm
	nasm $< -f elf -o $@
kernel.o: kernel/src/kernel.c 
	gcc -fno-pie -m32 -ffreestanding -c $< -o $@
mbr.bin: boot/mbr.asm
	nasm $< -f bin -o $@
os-image.bin: mbr.bin kernel.bin
	cat $^ > $@
run: os-image.bin
	qemu-system-i386 -fda $<
#debug
kernel.elf: kernel_entry.o ${OBJ_FILES}
	/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-binutils/2.36.1/bin/x86_64-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
%.o: %.c ${HEADERS}
	/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-gcc/10.2.0/bin/x86_64-elf-gcc -g -m32 -ffreestanding -c $< -o $@ # -g for debugging
%.o: boot/%.asm
	nasm $< -f elf -o $@
%.bin: boot/%.asm
	nasm $< -f bin -o $@
%.dis: %.bin
	ndisasm -b 32 $< > $@
clean:
	$(RM) *.bin *.o *.dis *.elf boot/*.o boot/*.bin drivers/*.o kernel/src/*.o
