all:virusDetector

virusDetector: task1c.o
	gcc -g -Wall -o virusDetector task1c.o

task1c.o: task1c.c
	gcc -g -Wall -ansi -c -o task1c.o task1c.c

#tell make that "clean" is not a file name!
.PHONY: clean

clean:
	rm -f *.o virusDetector
