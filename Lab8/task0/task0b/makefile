hexeditplus: hexeditplus.o 
	gcc -g -m32 -Wall -o hexeditplus hexeditplus.o

hexeditplus.o: hexeditplus.c hexeditplus.h
	gcc -g -m32 -Wall -c -o hexeditplus.o hexeditplus.c

.PHONY: clean

clean:
	rm -rf ./*.o hexeditplus