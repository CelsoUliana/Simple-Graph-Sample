/*
**   Celso Antonio Uliana Junior,
**   April 2019
*/
#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int TId;
typedef double TWeight;

struct SEdge {
    TId destination;
    TWeight weight;
    char * label;
};

typedef struct SEdge TEdge;

struct ListAdj {
    struct ListAdj * next;
    TEdge edge;
};

typedef struct ListAdj TNodeList;

struct SVertex {
    TId exitDegree;
    TId entryDegree;
    TWeight weight;
    char * label;

    // For undirected graphs, first list below contains all the neighbors
    // and second list is empty.
    // Edges exiting from vertex if directed.

    TNodeList * direct;

    // Edges arriving at vertex, if directed.

    TNodeList * reverse;
};

typedef struct SVertex TVertex;

struct SGraph {
    // directed = 1 for directed graph or 0 for undirected
    // n = number of vertexes
    // m = number of edges

    int directed;
    TId n;
    TId m;
    char * name;
    TVertex * vertices;
};

typedef struct SGraph TGraph;

// Returns the number of edges in G, It is a dynamic number
// Calculated in runtime when edges are connected or disconnected.
TId countEdges(const TGraph * g);

// Returns the number of vertices in G, It is a static number set up
// When graph is created.
TId countVertices(const TGraph * g);

// Returns the degree of vertex located at u,
// It is increase or decreased in runtime and put in a variable with the same name. 
TId vertexDegree(const TGraph * g, TId u);

// Returns the exit degree of vertex located at u,
// It is increase or decreased in runtime and put in a variable with the same name. 
TId vertexExitDegree(const TGraph * g, TId u);

// Returns the entry degree of vertex located at u,
// It is increase or decreased in runtime and put in a variable with the same name. 
TId vertexEntryDegree(const TGraph * g, TId u);

// Fill a node edge with its destination, weight(optional) and label(optional).
void fillNodeList(TNodeList * cur, TId destination, TWeight weight, char * label);

// Returns graph name if available.
const char *getName(const TGraph * g);

// Verify if u, v is invalid for connection or disconnection. (graph without vertices or out of limit)
int isInvalid(const TGraph * g, TId v, TId u);

// Create a graph to contains n vertices.
TGraph * createGraph(TId n, char * name, int directed);

// Init a graph to contains n vertices.
int initGraph(TGraph * g, TId n, char *name, int directed);

// Deallocates all graph, including the main structure
void destroyGraph(TGraph * g);

// Deallocates all structures related to the graph.
int finalizeGraphStructures(TGraph * g);

// Deallocates all neighbors edges from a vertex.
int finalizeEdgeList(TNodeList * list);

// Verify if u, v is an edge in the graph and return his referencee, if
// Found, or NULL otherwise.
// Returns the reference to the edge before the found one.
// If there is only one edge, It will return NULL so, the responsability
// to check is in connect or disconnect.
TNodeList * edge(const TGraph * g, TId u, TId v, int directed);

// Remove an edge u, v of the graph. Returns false if edge was not found.
int disconnect(TGraph * g, TId u, TId v);

// Connect two vertices by an edge.
const TEdge * connect(TGraph * g, TId u, TId v);

// Connect two vertices by an edge with a weight.
// If the edge already exists, return NULL;
const TEdge * connectWeight(TGraph * g, TId u, TId v, TWeight weight, char * label);

// Change the weight of an existing edge. Return NULL if not found.
TEdge * changeWeight(TGraph * g, TId u, TId v, TWeight weight, char * label);

// Auxiliary function to print all graph in console.
void debugGraph(TGraph * g);

#endif