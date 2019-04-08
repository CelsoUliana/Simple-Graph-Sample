CC = gcc
FLAGS = -c -g -o
vpath src
vpath inc

all: prim

prim: main.o graphio.o graph.o
	$(CC) -o prim *.o

main.o: main.c graphio.h
	$(CC) $(FLAGS) main.o main.c

graphio.o: graphio.c graphio.h
	$(CC) $(FLAGS) graphio.o graphio.c

graph.o: graph.c graph.h
	$(CC) $(FLAGS) graph.o graph.c

clean:
	rm -f *.o *.exe