
CC = gcc -Wall

notebook: notebook.c notebook.c
	$(CC) `gtk-config --cflags` `gtk-config --libs` notebook.c -o notebook

clean: 
	rm -f *.o notebook
