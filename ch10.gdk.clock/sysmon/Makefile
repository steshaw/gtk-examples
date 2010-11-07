
CC = gcc -g -Wall

gtksysmon: graph.o sys.o device.o frontend.o misc.o
	$(CC) `gtk-config --libs` frontend.o misc.o device.o graph.o sys.o -o gtksysmon

frontend.o: frontend.c
	$(CC) `gtk-config --cflags` -c frontend.c -o frontend.o

misc.o: misc.c
	$(CC) `gtk-config --cflags` -c misc.c -o misc.o

sys.o: sys.c 
	$(CC) `gtk-config --cflags` -c sys.c -o sys.o

graph.o: graph.c 
	$(CC) `gtk-config --cflags` -c graph.c -o graph.o

device.o: device.c device.h
	$(CC) `gtk-config --cflags` -c device.c -o device.o

clean: 
	rm -f *.o gtksysmon
