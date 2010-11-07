
CC = gcc

all: frontend

frontend: frontend.c combobox.c misc.c
	$(CC) `gtk-config --cflags` `gtk-config --libs` frontend.c combobox.c misc.c -o frontend 

clean: 
	rm -f *.o frontend
