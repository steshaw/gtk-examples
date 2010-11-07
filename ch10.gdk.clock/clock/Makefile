
CC = gcc -Wall

all: clockx dclockx

clockx: clock.c 
	$(CC) -Wall `gtk-config --cflags` `gtk-config --libs` clock.c -o clockx 

dclockx: dblclock.c 
	$(CC) -Wall `gtk-config --cflags` `gtk-config --libs` dblclock.c -o dclockx 

clean: 
	rm -f *.o clockx dclockx
