
CC = gcc -Wall

filesel: filesel.c filesel.c
	$(CC) `gtk-config --cflags` `gtk-config --libs` filesel.c -o filesel

clean: 
	rm -f *.o filesel
