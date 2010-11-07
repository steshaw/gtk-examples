
CC = gcc -Wall

all: colortest1 fonttest colordiag cursors fontsel

cursors.o: cursors.c 
	$(CC) `gtk-config --cflags` -c cursors.c 

button.o: button.c
	$(CC) `gtk-config --cflags` -c button.c 

colorbutton.o: colorbutton.c
	$(CC) `gtk-config --cflags` -g -c colorbutton.c 

dialog.o: dialog.c
	$(CC) `gtk-config --cflags` -c dialog.c 

DisplayFonts.o: DisplayFonts.c
	$(CC) `gtk-config --cflags` -c DisplayFonts.c 

colordiag.o: colordiag.c 
	$(CC) `gtk-config --cflags` -c colordiag.c 

colortest1: button.o dialog.o
	$(CC) `gtk-config --libs` dialog.o button.o -o colortest1

fonttest: DisplayFonts.o
	$(CC) `gtk-config --libs` DisplayFonts.o -o fonttest

colordiag: colordiag.o colorbutton.o
	$(CC) `gtk-config --libs` colordiag.o colorbutton.o -o colordiag

cursors: cursors.o 
	$(CC) `gtk-config --libs` cursors.o -o cursors

gfontsel.o: gfontsel.c
	$(CC) `gtk-config --cflags` -g -c gfontsel.c 

fontbutton.o: fontbutton.c
	$(CC) `gtk-config --cflags` -c fontbutton.c 

fontsel: gfontsel.o fontbutton.o
	$(CC) `gtk-config --libs` gfontsel.o fontbutton.o -o fontsel

clean: 
	rm -f *.o colortest1 fonttest colordiag cursors fontsel
