
CC = gcc

form: combos.c \
      edit.c \
      combobox.c \
      menucombo.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` \
        combos.c \
        edit.c \
        combobox.c \
        menucombo.c \
        -o form

clean: 
	rm -f *.o form
