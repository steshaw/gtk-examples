
CC = gcc

minesweeper: minesweeper.c misc.c menu.c digits.h timer.c showmessage.c about.c
	$(CC) `gtk-config --cflags` `gtk-config --libs` minesweeper.c misc.c menu.c timer.c showmessage.c about.c -o minesweeper

clean: 
	rm -f *.o minesweeper
