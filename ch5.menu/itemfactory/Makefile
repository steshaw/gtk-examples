
CC = gcc

all: itemfa

itemfa: menu.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` menu.c -o itemfa 

clean: 
	rm -f *.o itemfa
