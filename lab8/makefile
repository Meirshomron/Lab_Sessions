#format is target-name: target dependencies
#{-tab-}actions

# All Targets
# all: task1

# Tool invocations
# Executable "task0" depends on the files numbers.o and task1.o.
task1:  task1.o  
	gcc -m32 -g task1.o -o task1

# Depends on the source and header files

task1.o: task1.c
	gcc -g -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task1.c -o task1.o
	

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
	rm -f task1 task1.o