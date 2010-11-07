
CC = gcc

PROGS = sortlist tree hash message list mem

all: $(PROGS)

sortlist: sortlist.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` sortlist.c -o sortlist 

tree: tree.c 
	$(CC) `gtk-config --cflags` `gtk-config --libs` tree.c -o tree 

hash: hash.c
	$(CC) `gtk-config --cflags` `gtk-config --libs` hash.c -o hash 

message: message.c
	$(CC) `gtk-config --cflags` `gtk-config --libs` message.c -o message 

list: list.c
	$(CC) `gtk-config --cflags` `gtk-config --libs` list.c -o list 

mem: mem.c
	$(CC) `gtk-config --cflags` `gtk-config --libs` mem.c -o mem 

IGNORED = *.o $(PROGS)

.PHONY:
ignored:
	@echo $(IGNORED)

.PHONY:
clean:
	rm -f *.o $(PROGS)
