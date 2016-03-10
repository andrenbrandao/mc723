all: primo

primo: main.o calc_primo.o
	gcc -O3 main.o calc_primo.o -o primo

main.o: main.c
	gcc -O3 -c main.c

calc_primo.o: calc_primo.c
	gcc -O3 -c calc_primo.c

clean:
	rm *.o primo
