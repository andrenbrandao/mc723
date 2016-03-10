all: primo2

primo: main2.o calc_primo2.o
	gcc -O3 main2.o calc_primo2.o -o primo2

main.o: main2.c
	gcc -O3 -c main2.c

calc_primo.o: calc_primo2.c
	gcc -O3 -c calc_primo2.c

clean:
	rm *.o primo2
