
CC = gcc -Wall

all: molecule 

molecule.o: molecule.c
	$(CC) `gtk-config --cflags` -c -g molecule.c

matrix3d.o: matrix3d.c
	$(CC) `gtk-config --cflags` -c -g matrix3d.c

frontend.o: frontend.c
	$(CC) `gtk-config --cflags` -c -g frontend.c

misc.o: misc.c
	$(CC) `gtk-config --cflags` -c -g misc.c

filesel.o: filesel.c
	$(CC) `gtk-config --cflags` -c -g filesel.c

molecule: molecule.o matrix3d.o frontend.o misc.o filesel.o
	$(CC) -Wall -g `gtk-config --libs` molecule.o matrix3d.o frontend.o misc.o filesel.o -o molecule 

clean: 
	rm -f *.o molecule
