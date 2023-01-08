all: task2

task2: task2.o
	gcc -g -m32 -Wall -o task2 task2.o

task2.o: task2.c
	gcc -g -m32 -ansi -Wall -c -o task2.o task2.c

.PHONY: clean

clean:
	rm -f task2.o task2