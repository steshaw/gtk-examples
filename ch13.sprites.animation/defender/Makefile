
CC = gcc 

all: defender

defender.o: defender.c mutant.h
	$(CC) `gtk-config --cflags` -Wall -c -g defender.c 

animate.o: animate.c 
	$(CC) `gtk-config --cflags` -Wall -c -g animate.c 

keys.o: keys.c 
	$(CC) `gtk-config --cflags` -Wall -c -g keys.c 

defender: animate.o defender.o keys.o
	$(CC) `gtk-config --libs` -g animate.o keys.o defender.o -o defender

clean: 
	rm -f *.o defender
