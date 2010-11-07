
CC = gcc

all: packtable packbox

packtable : packtable.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` packtable.c -o packtable 
packbox : packbox.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` packbox.c -o packbox 
clean: 
	rm -f *.o packtable packbox
