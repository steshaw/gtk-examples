
CC = gcc -Wall

all: webtraffic 

bitmaps.o: bitmaps.c 
	$(CC) `gtk-config --cflags` -c -g bitmaps.c 

filesel.o: filesel.c 
	$(CC) `gtk-config --cflags` -c -g filesel.c 

progressbar.o: progressbar.c 
	$(CC) `gtk-config --cflags` -c -g progressbar.c 

parselog.o: parselog.c 
	$(CC) `gtk-config --cflags` -c -g parselog.c 

misc.o: misc.c 
	$(CC) `gtk-config --cflags` -c -g misc.c 

frontend.o: frontend.c 
	$(CC) `gtk-config --cflags` -c -g frontend.c 

notebook.o: notebook.c 
	$(CC) `gtk-config --cflags` -c -g notebook.c 

#bitmaps: bitmaps.o 
#	$(CC) `gtk-config --libs` -g bitmaps.o -o bitmaps

webtraffic: parselog.o misc.o frontend.o notebook.o progressbar.o filesel.o bitmaps.o 
	$(CC) `gtk-config --libs` -g bitmaps.o parselog.o misc.o progressbar.o filesel.o frontend.o notebook.o -o webtraffic

clean: 
	rm -f *.o webtraffic
