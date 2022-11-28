all:  task0

task0: task0.o
	gcc -m32 -g -Wall -o task0 task0.o

task0.o: task0.s 
	nasm -g -f elf -w+all -o task0.o task0.s 

.PHONY: clean  

clean:
	rm -f *.o task0