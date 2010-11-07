
CC = gcc 

all: graphit

main.o: main.c 
	$(CC) `gtk-config --cflags` -c -g main.c

gtkgraph.o: gtkgraph.c 
	$(CC) `gtk-config --cflags` -c -g gtkgraph.c

graphit: gtkgraph.o main.o
	$(CC) `gtk-config --libs` -g gtkgraph.o main.o -o graphit 

clean: 
	rm -f *.o graphit
