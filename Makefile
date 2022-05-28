.phony: build run clean

all: build run

build: darby_structures.h darby_functions.h darby.c
	gcc darby.c -o darby.o -g -Wall

run:
	./darby.o

mem-test: build
	valgrind --leak-check=full -s ./darby.o

clean:
	rm -f *.o *.out out *.in in *.ref *.debug