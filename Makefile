# $@ = target file
# $< = first dependency
# $^ = all dependecny

C_SOURCES = $(wildcard kernel/src/*.c)
HEADERS = $(wildcard kernel/include/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o}
#first rule is the one exectued when no parameters
all: run

build/kernel.bin: build/kernel_entry.o build/kernel.o
	/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-binutils/2.36.1/bin/x86_64-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
build/kernel_entry.o: boot/kernel_entry.asm
	nasm $< -f elf -o $@
build/kernel.o: kernel/src/kernel.c
	gcc -fno-pie -m32 -ffreestanding -c $< -o $@
build/mbr.bin: boot/mbr.asm
	nasm $< -f bin -o $@
build/os-image.bin: build/mbr.bin build/kernel.bin
	cat $^ > $@
run: build/os-image.bin
	qemu-system-i386 -fda $<
#debug
kernel.elf: kernel.o ${OBJ_FILES}
	/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-binutils/2.36.1/bin/x86_64-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin -d guest_errors,int &
	/home/linuxbrew/.linuxbrew/Cellar/i386-elf-gdb/10.1/bin/i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
%.o: %.c ${HEADERS}
	/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-gcc/10.2.0/bin/x86_64-elf-gcc -g -m32 -ffreestanding -c $< -o $@ # -g for debugging
%.o: boot/%.asm
	nasm $< -f elf -o $@
%.bin: boot/%.asm
	nasm $< -f bin -o $@
%.dis: %.bin
	ndisasm -b 32 $< > $@
clean:
	$(RM) build/*.bin build/*.o build/*.dis build/*.elf boot/*.o boot/*.bin
