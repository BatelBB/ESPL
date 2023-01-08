hexeditplus: hexeditplus.o 
	gcc -g -m32 -Wall -o hexeditplus hexeditplus.o

hexeditplus.o: hexeditplus.c hexeditplus.h units.o
	gcc -g -m32 -Wall -c -o hexeditplus.o hexeditplus.c

units.o : units.c
	gcc -g -m32 -Wall -c -o units.o units.c

.PHONY: clean

clean:
	rm -rf ./*.o hexeditplus