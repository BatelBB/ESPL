main : main.o numbers.o add.o
	gcc -g -m32 -Wall -o main main.o numbers.o add.o

main.o : main.c add.o
	gcc -m32 -g -Wall -c -o main.o main.c


numbers.o : numbers.c 
	gcc -m32 -g -Wall -c -o numbers.o numbers.c

add.o : 
	nasm -g -f elf -w+all -o add.o add.s


.PHONY: clean 

 #Clean the build directory 

clean:
	rm -f *.o main

 