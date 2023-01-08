all: task3

task3: task3.o
	gcc -g -m32 -Wall -o task3 task3.o

task3.o: task3.c
	gcc -g -m32 -ansi -Wall -c -o task3.o task3.c

.PHONY: clean

clean:
	rm -f task3.o task3
