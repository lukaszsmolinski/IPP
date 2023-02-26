CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2

.PHONY: all clean

all: labyrinth

labyrinth: main.o bitset.o input.o maze.o vector.o
	$(CC) -o $@ $^

bitset.o: bitset.c bitset.h vector.h utils.h
input.o: input.c input.h maze.h bitset.h vector.h utils.h
main.o: main.c input.h maze.h bitset.h vector.h
maze.o: maze.c maze.h bitset.h vector.h utils.h
vector.o: vector.c vector.h utils.h

clean:
	rm -f *.o labyrinth
