objet.o : objet.c objet.h
	gcc -c objet.c -g -o objet.o

objet : objet.o
	gcc objet.o -g -o objet

sad : sad.c objet.o
	gcc sad.c objet.o -g -o sad

		
clean :
	rm -f *.o
	rm -f objet
	rm -f sad