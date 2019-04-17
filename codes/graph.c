/*
**   Celso Antonio Uliana Junior,
**   April 2019
*/
#include "graph.h"

TId countEdges(const TGraph * g) {
    return g -> m;
}

TId countVertices(const TGraph * g) {
    return g -> n;
}

TId vertexDegree(const TGraph * g, TId u){
    return vertexExitDegree(g, u) + vertexEntryDegree(g, u);
}

TId vertexExitDegree(const TGraph * g, TId u){
    return g -> vertices[u].exitDegree;
}

TId vertexEntryDegree(const TGraph * g, TId u){
    return g -> vertices[u].entryDegree;
}

void fillNodeList(TNodeList * cur, TId destination, TWeight weight, char * label){
    cur -> edge.label = label;
    cur -> edge.weight = weight;
    cur -> edge.destination = destination;
}

const char * getName(const TGraph * g) {
    return g -> name;
}

int isInvalid(const TGraph * g, TId v, TId u){
    return g -> n == 0 || g -> vertices == NULL || v >= g -> n || u >= g -> n;
}

TGraph * createGraph(TId n, char * name, int directed) {

    TGraph * g = (TGraph *) malloc (sizeof(TGraph));

    if (g == NULL)
        return NULL;

    if (initGraph(g, n, name, directed))
        return g;

    else {
        free(g);
        return NULL;
    }
}

int initGraph(TGraph * g, TId n, char * name, int directed) {
    g -> vertices = (TVertex *) malloc (sizeof(TVertex[n]));

    if (g -> vertices == NULL)
        return 0;
        
    g -> n = n;
    g -> m = 0;
    g -> name = name;
    g -> directed = directed;

    int i;
    for (i = 0; i < n; i++) {
        g -> vertices[i].label = NULL;
        g -> vertices[i].direct = NULL;
        g -> vertices[i].reverse = NULL;
        g -> vertices[i].weight = 0.0;
        g -> vertices[i].exitDegree = 0;
        g -> vertices[i].entryDegree = 0;
    }

    return 1;
}

void destroyGraph(TGraph * g) {
    finalizeGraphStructures(g);

    free(g -> vertices);
    g -> vertices = NULL;

    free(g);
    g = NULL;
}

int finalizeGraphStructures(TGraph * g) {

    int i;
    for (i = 0; i < g -> n; i++) {
        finalizeEdgeList(g -> vertices[i].direct);
        finalizeEdgeList(g -> vertices[i].reverse);
        g -> vertices[i].direct = NULL;
        g -> vertices[i].reverse = NULL;
    }

}

int finalizeEdgeList(TNodeList * list) {

    TNodeList * cur;

    while(list != NULL){
        cur = list;
        list = list -> next;
        cur -> next = NULL;
        free(cur);
    }

    list = cur = NULL;

    return 1;
}

TNodeList * edge(const TGraph * g, TId u, TId v, int directed){

    if(isInvalid(g, u, v))
        return NULL;

    else{
        
        // Recebe a referencia a lista direita ou reversa, de acordo com parametro
        TNodeList * cur = directed == 0 ? g -> vertices[u].direct : g -> vertices[u].reverse;
        TNodeList * next;

        if(cur == NULL){
            return NULL;
        }
        
        if(cur -> edge.destination == v){
            return NULL;
        }
        
        next = cur -> next;

        if(next == NULL){
            return NULL;
        }

        while(next -> edge.destination != v && next -> next != NULL){
            cur = next;
            next = next -> next;
        }

        return cur;
    }
}

int disconnect(TGraph * g, TId u, TId v){

    if(isInvalid(g, u, v))
        return 0;   
    
    else{
        // Remove the edge u, v.
        TNodeList * cur = edge(g, u, v, 0);
         
        if(cur == NULL){
            if(g -> vertices[u].direct != NULL 
            && g -> vertices[u].direct -> edge.destination == v){
                g -> vertices[u].direct = cur = NULL;
            }
        }

        if(cur != NULL){
            if(cur -> next != NULL){
                TNodeList * del = cur -> next;
                cur -> next = cur -> next -> next;
                free(del);
                del = cur = NULL;
            }
        }
        
        g -> vertices[u].exitDegree--;
        
        // Remove the reverse edge v, u.
        if(g -> directed){
    
            TNodeList * reverse_cur = edge(g, v, u, 1);
        
            if(reverse_cur == NULL){
                if(g -> vertices[v].reverse != NULL 
                && g -> vertices[v].reverse -> edge.destination == u){
                    g -> vertices[v].reverse = reverse_cur = NULL;
                }
            }
        
            if(reverse_cur != NULL){
                if(reverse_cur -> next != NULL){
                    TNodeList * del = reverse_cur -> next;
                    reverse_cur -> next = reverse_cur -> next -> next;
                    free(del);
                    del = reverse_cur = NULL;
                }
            }
        
            g -> vertices[v].entryDegree--;
        }
        
        // Remove the direct edge v, u.
        else{

            TNodeList * reverse_cur = edge(g, v, u, 0);

            if(reverse_cur == NULL){
                if(g -> vertices[v].direct != NULL 
                && g -> vertices[v].direct -> edge.destination == u){
                    free(reverse_cur);
                    g -> vertices[v].direct = reverse_cur = NULL;
                }
            }

            if(reverse_cur != NULL){
                if(reverse_cur -> next != NULL){
                    TNodeList * reverse_del = reverse_cur -> next;
                    reverse_cur -> next = reverse_cur -> next -> next;
                    free(reverse_del);
                    reverse_del = reverse_cur = NULL;
                }
            }
        
            g -> vertices[v].exitDegree--;
        }   
    }
    
    g -> m--;
    return 1;
}

const TEdge * connect(TGraph * g, TId u, TId v) {
    return connectWeight(g, u, v, 0, NULL);
}

const TEdge * connectWeight(TGraph * g, TId u, TId v, TWeight weight, char * label){

    if(isInvalid(g, u, v))
        return NULL;

    else{
        
        // Add the direct edge u, v.
        TNodeList * cur = (TNodeList *) malloc (sizeof(TNodeList));
        fillNodeList(cur, v, weight, label);
        
        TNodeList * ref = g -> vertices[u].direct;
        cur -> next = ref;
        
        g -> vertices[u].direct = cur;
        g -> vertices[u].exitDegree++;
        
        // Add the reverse edge v, u.
        if(g -> directed){

            TNodeList * reverse_cur = (TNodeList *) malloc (sizeof(TNodeList));        
            fillNodeList(reverse_cur, u, weight, label);
            
            TNodeList * reverse_ref = g -> vertices[v].reverse;  
            reverse_cur -> next = reverse_ref;
            
            g -> vertices[v].reverse = reverse_cur;  
            g -> vertices[v].entryDegree++;
        }
        
        // Add the direct edge v, u.
        else{
            
            TNodeList * reverse_cur = (TNodeList *) malloc (sizeof(TNodeList));     
            fillNodeList(reverse_cur, u, weight, label);
            
            TNodeList * reverse_ref = g -> vertices[v].direct;
            reverse_cur -> next = reverse_ref;

            g -> vertices[v].direct = reverse_cur;       
            g -> vertices[v].exitDegree++;
        }
        
        g -> m++;
    }
}

TEdge * changeWeight(TGraph * g, TId u, TId v, TWeight weight, char * label){
    
    if(isInvalid(g, u, v))
        return NULL;
    
    else{
        
        // Change the weight of direct edge u, v.
        TNodeList * cur = edge(g, u, v, 0);
        
        if(cur == NULL && g -> vertices[u].direct != NULL 
        && g -> vertices[u].direct -> edge.destination == v){
            g -> vertices[u].direct -> edge.weight = weight;
            g -> vertices[u].direct -> edge.label = label;
        }
        
        else if(cur != NULL){
            cur -> next -> edge.weight = weight;
            cur -> next -> edge.label = label;
        }
        
        // Change the weight of reverse edge v, u.
        if(g -> directed){
        
            TNodeList * reverse_cur = edge(g, v, u, 1);
            
            if(reverse_cur == NULL && g -> vertices[v].reverse != NULL 
            && g -> vertices[v].reverse -> edge.destination == u){
                g -> vertices[v].reverse -> edge.weight = weight;
                g -> vertices[v].reverse -> edge.label = label;
            }
        
            else if(reverse_cur != NULL){
                reverse_cur -> next -> edge.weight = weight;
                reverse_cur -> next -> edge.label = label;
            }
        }
        
        // Chance the weight of direct edge v, u.
        else{
            
            TNodeList * reverse_cur = edge(g, v, u, 0);
            
            if(reverse_cur == NULL && g -> vertices[v].direct != NULL 
            && g -> vertices[v].direct -> edge.destination == u){
                g -> vertices[v].direct -> edge.weight = weight;
                g -> vertices[v].direct -> edge.label = label;
            }
            
            else if(reverse_cur != NULL){
                reverse_cur -> next -> edge.weight = weight;
                reverse_cur -> next -> edge.label = label;
            }
        }
    
    }
    
    return NULL;
}

void debugGraph(TGraph * g){

    if(g == NULL){
        printf("Null Graph.\n");
        return;
    }

    printf("Start of graph:\n");
    printf("graph's name: %s\n", g -> name);
    printf("number of vertices: %d\n", g -> n);
    printf("number of edges: %d\n", g -> m);
    printf("directed: %d\n", g -> directed);

    int i;
    for (i = 0; i < g -> n; i++) {
        printf("\tTVertex: %d\n", i);
        printf("\tlabel: %s\n", g -> vertices[i].label);
        printf("\tExit degree: %d\n", g -> vertices[i].exitDegree);
        printf("\tEntry degree: %d\n", g -> vertices[i].entryDegree);
        printf("\tweight: %lf\n", g -> vertices[i].weight);
        printf("\tList of direct edges: \n");

        TNodeList * cur = g -> vertices[i].direct;

        while(cur != NULL){
            printf("\t\tEdge name: %s\n", cur -> edge.label);
            printf("\t\tEdge weight: %lf\n", cur -> edge.weight);
            printf("\t\texits from %d e goes to %d\n", i, cur -> edge.destination);
            cur = cur -> next;
        }

        cur = g -> vertices[i].reverse;
        printf("\tList of reverse edges: \n");

        while(cur != NULL){

            printf("\t\tEdge name: %s\n", cur -> edge.label);
            printf("\t\tEdge weight: %lf\n", cur -> edge.weight);
            printf("\t\texist from %d e goes to %d\n", cur -> edge.destination, i);
            cur = cur -> next;

        }
    }
}