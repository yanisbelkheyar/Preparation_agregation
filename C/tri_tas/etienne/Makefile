exo1: main.out tas.out
	gcc -o exo1 main.out tas.out

main.out: main.c tas.h
	gcc -Wall -c main.c -o main.out

tas.out: tas.c tas.h
	gcc -Wall -c tas.c -o tas.out

clean:
	rm -f exo1 main.out tas.out