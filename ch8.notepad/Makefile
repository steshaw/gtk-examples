all: notepad

# Compiler settings
CC = gcc -Wall -O `gtk-config --cflags`

main.o: main.c 
	$(CC) -c -g main.c 

notepad.o: notepad.c 
	$(CC) -c -g notepad.c 

filesel.o: filesel.c 
	$(CC) -c -g filesel.c 

about.o: about.c 
	$(CC) -c -g about.c 

menu.o: menu.c 
	$(CC) -c -g menu.c 

progressbar.o: progressbar.c 
	$(CC) -c -g progressbar.c 

search.o: search.c 
	$(CC) -c -g search.c 

rtfreadr.o: rtfreadr.c 
	$(CC) -c -g rtfreadr.c 

rtfactn.o: rtfactn.c 
	$(CC) -c -g rtfactn.c 

# Object modules
OBJS = main.o  notepad.o  filesel.o  about.o  menu.o  misc.o  progressbar.o  search.o  rtfreadr.c  rtfactn.c  showmessage.o

notepad:  main.o notepad.o filesel.o about.o menu.o misc.o progressbar.o search.o rtfreadr.o rtfactn.o showmessage.o 
	gcc `gtk-config --libs` main.o notepad.o filesel.o about.o menu.o misc.o progressbar.o search.o rtfreadr.o rtfactn.o showmessage.o -o notepad


