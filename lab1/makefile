CC=gcc
CFLAGS=-Wall -ansi -g 

all:encoder

encoder:encoder.o
encoder.o:encoder.c

#tell make that "clean" is not a file name!
.PHONY: clean

clean:
	rm -f encoder encoder.o
 
