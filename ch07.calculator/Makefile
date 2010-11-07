
CC = gcc -Wall

all: calculator

calculator: calculator.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` calculator.c -o calculator

clean: 
	rm -f *.o calculator
