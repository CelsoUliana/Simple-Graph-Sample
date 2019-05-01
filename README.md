# Simple-Graph-Sample
A simple graph sample implementation in C.

Can be used for simple imports in .gr format(dot) and simple exports .viz(graphviz) or copy (dot).
Can also be used for getting degrees or min distance.

Compile or make:

```
make 

or

gcc -c -g -o main.o main.c
gcc -c -g -o graphio.o graphio.c
gcc -c -g -o graph.o graph.c
gcc -o prim *.o
```

Run:

```./prim [Command]```
