start: main.o util.o start.o
	ld -m elf_i386 start.o main.o util.o -o start 

main.o: main.c util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

start.o: start.s
	nasm -f elf32 start.s -o start.o


clean:
	rm start.o util.o main.o start 