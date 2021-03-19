OBJECTS = func.o main.o
CC = gcc
CFLAGS = -std=c11 -m32 -Wall -Wextra -Werror -c
AS = nasm
ASFLAGS = -f elf

all: $(OBJECTS)
	gcc -m32 $(OBJECTS) -o main

run: all
	./main

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) main
