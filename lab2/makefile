CC=gcc
CFLAGS=-Wall -ansi -g 

all:menu_map

menu_map:menu_map.o
menu_map.o:menu_map.c

#tell make that "clean" is not a file name!
.PHONY: clean

clean:
	rm -f menu_map menu_map.o
