
CC = gcc

all: editable

editable: editable.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` editable.c -o editable

clean:
	rm -f *.o editable

