/*
**   Celso Antonio Uliana Junior,
**   April 2019
*/
#ifndef _GRAPH_IO_H
#define _GRAPH_IO_H

#include <stdio.h>
#include "graph.h"

// Read a graph in DIMACS format.
// g can be:
//      - A allocated graph, but not initialized, in which case
// It will be initialized for the size obtained in the file.
//      - A NULL pointer, in whick case the graph will be
// Created and returned.
TGraph * readGraphDimacs(TGraph *g, FILE* handler);

// Auxiliary function to debug readGraphDimacs(...) in console.
// In witch case the correct arguments must be suplied in console.
TGraph * debugReadGraphDimacs(TGraph *g);

// Save a graph in format DIMACS, returns 1 if success or 0 if failure.
int saveGraphDimacs(const TGraph *g, FILE* handler);

// Auxiliary function to debug the export to dimacs format in console.
int debugSaveGraphDimacs(const TGraph *g);

// Save a graph in dot format, return 1 if success or 0 if failure.
// For now only works in undirected graph(?)
int saveGraphDot(const TGraph *g, FILE* handler);

// Auxiliary function to debug the export to dot format in console.
// For now only works in undirected graph(?)
int debugSaveGraphDot(const TGraph *g);

#endif