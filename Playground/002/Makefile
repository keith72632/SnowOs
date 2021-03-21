OBJ = ${Playground/002/*.o}
C_SOURCE = ${Playground/002/*.c}
ASS_SPURCE = ${Playground/002/*.asm}

all: main

func.o: *.asm
	nasm -f elf32 $^ -o $@
main.o: *.c
	gcc -g -m32 $^ -c -o $@
main: func.o main.o
	gcc -m32 $^ -o $@
clean:
	rm *.o main
