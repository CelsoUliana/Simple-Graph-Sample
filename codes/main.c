/*
**   Celso Antonio Uliana Junior,
**   April 2019
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphio.h"

enum COMMANDS {
    UNKNOWN         = 0,
    COPY            = 1,
    DRAW            = 2,
    MAXDEGREE       = 3,
    MINDEGREE       = 4
};

/* 
Find the minimum degree of the graph and list all vertices
with the minimum degree.
If graph is directed, consider the sum of exit and entry degrees.
*/
void findMinimumDegree(const TGraph *g, FILE *handler) {
    // Initialize the degree with a number too big 
    // (none vertex is neighbor to all). If graph is directed can be an edge
    // exiting and other arriving at each vertex. 
    TId gmin = 2 * g -> n;
    TId i;

    // Search for vertices of lower degree.
    for (i = 0; i < g -> n; i++) {

        TId degreev = vertexDegree(g, i);

        if (degreev < gmin)
            gmin = degreev;
    }
  
    fprintf(handler, "Minimum degree: %d. Vertices: ", gmin);

    for (i = 0; i < g -> n; i++) {

        TId degreev = vertexDegree(g, i);

        if (degreev == gmin)
            fprintf(handler, "%d ", i);
    }

    fprintf(handler, ".\n");
}

/* 
Find the maximum degree of the graph and list all vertices
with the maximum degree.
If graph is directed, consider the sum of exit and entry degrees.
*/
void findMaximumDegree(const TGraph *g, FILE *handler) {
    // Initialize the degree with a number too low (the minimum).
    // If graph is directed can be an edge
    // exiting and other arriving at each vertex. 
    TId gmax = 0;
    TId i;

    for(i = 0; i < g -> n; i++){
       
        TId degreev = vertexDegree(g, i);

        if(degreev > gmax)
            gmax = degreev;
    }

    fprintf(handler, "Maximum degree: %d. Vertices: ", gmax);

    for (i = 0; i < g -> n; i++) {

        TId degreev = vertexDegree(g, i);

        if (degreev == gmax)
            fprintf(handler, "%d ", i);
    }

    fprintf(handler, ".\n");
}

// Print message of use, when there is a problem in args.
void printUse(char **argv) {
    fprintf(stderr, "\nUsage: %s command input.gr [output.gr]\n", argv[0]);
    fprintf(stderr, "Commands:\n\tcopy      : Copy the input graph to output.\n"
	  "\tdraw      : generate a draw of the graph in format dot in exit file.\n"
	  "\tmaxdegree : print the vertices with the maximum degree.\n"
	  "\tmindegree : print the vertices with the minumum degree.\n");
}

int main (int argc, char **argv) {

    if (argc < 3) {
        printUse(argv);
        return 1;
    }

    // Command string typed by user.
    char *scommand = argv[1];
    int command = UNKNOWN;

    // Identify the known commands.
    if (strcmp(scommand, "copy") == 0)
        command = COPY;
    else if (strcmp(scommand, "draw") == 0)
        command = DRAW;
    else if (strcmp(scommand, "maxdegree") == 0)
        command = MAXDEGREE;
    else if (strcmp(scommand, "mindegree") == 0)
        command = MINDEGREE;

    // Verify if the command is known.
    if (command == 0) {
        fprintf(stderr, "unknown command: %s .\n", argv[1]);
        printUse(argv);
        return 1;
    }

    // Verify if command has correct arguments.
    if (((command == COPY) || (command == DRAW)) && argc < 4) {
        fprintf(stderr, "Command requires an exit file.\n");
        printUse(argv);
        return 1;
    }

    // Open the input file for reading.
    FILE *input = fopen(argv[2], "r");
    if (input == NULL) {
        fprintf(stderr, "Failure to open input file. %s .\n", argv[2]);
        return 2;
    }

    // Read the entry graph in DIMACS format.
    TGraph *g = NULL;
    int res;
    g = readGraphDimacs(g, input);
    fclose(input);

    if (g == NULL) {
        fprintf(stderr, "Failure in reading graph.\n");
        return 2;
    }

    // Open output file, if necessary.
    FILE *output;
    if (command == COPY || command == DRAW) {
        output = fopen(argv[3], "w");
        if (output == NULL) {
            fprintf(stderr, "Failure to open output file. %s.\n", argv[3]);
            destroyGraph(g);
            return 3;
        }
    } 

    else {
        output = stdout;
    }

    // Execute the command.
    switch (command) {
    case COPY:
        res = saveGraphDimacs(g, output);
        fclose(output);
        break;
    case DRAW:
        res = saveGraphDot(g, output);
        fclose(output);
        break;
    case MAXDEGREE:
        findMaximumDegree(g, output);
        res = 1;
        break;
    case MINDEGREE:
        findMinimumDegree(g, output);
        res = 1;
        break;
    }

    // Free the graph
    destroyGraph(g);
    g = NULL;

    if (!res) {
        fprintf(stderr, "Failure to execute command.\n");
        return 4;
    }

    return 0;
}