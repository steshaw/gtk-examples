
CC = gcc

progressbar: progressbar.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` progressbar.c main.c -o progressbar

clean: 
	rm -f *.o progressbar
