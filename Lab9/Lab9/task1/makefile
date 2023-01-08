all: task1

task1: task1.o
	gcc -g -m32 -Wall -o task1 task1.o

task1.o: task1.c
	gcc -g -m32 -ansi -Wall -c -o task1.o task1.c

.PHONY: clean

clean:
	rm -f task1.o task1