
CC = gcc 

all: animate

animate: animate.c flower.h driver.h man.h woman.h bike.h
	$(CC) `gtk-config --cflags` `gtk-config --libs` -g animate.c -o animate 

clean: 
	rm -f *.o animate
