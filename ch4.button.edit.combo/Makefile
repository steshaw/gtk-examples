
CC = gcc

all: align button buttonlabel checkbox listbox radio toggle

align: align.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` align.c -o align

button: button.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` button.c -o button

buttonlabel: buttonlabel.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` buttonlabel.c -o buttonlabel

checkbox: checkbox.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` checkbox.c -o checkbox

listbox: listbox.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` listbox.c -o listbox

radio: radio.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` radio.c -o radio

toggle: toggle.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` toggle.c -o toggle

clean: 
	rm -f *.o align button buttonlabel checkbox listbox radio toggle
