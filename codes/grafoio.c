#include "grafoio.h"
#include <string.h>
#include <stdlib.h>


// LÃª um grafo no formato DIMACS adaptado.
// g pode ser:
//   - Um grafo alocado, mas nÃ£o inicializado, caso em que serÃ¡
// inicializado para o tamanho obtido do arquivo.
//   - Um ponteiro NULL, caso em que o grafo serrÃ¡ criado e retornado.
// TODO: contar arestas e conferir
TGrafo * le_grafo_dimacs(TGrafo *g, FILE* arquivo) {
    int n, m, dir;
    char d, *p = (char*) malloc (255 * sizeof(char));;

    
    fscanf(arquivo, "%c %c %d %d %s\n", &d, &d, &n, &m, p);
    
    dir = d == 'U' ? 0 : 1;
    
    if(g == NULL)
        g = create_graph(n, p, dir);
        
    else
        init_graph(g, n, p, dir);
    
    for(int i = 0; i < n; i++){
        int aux = 0;
        double peso = 0;
        char c, *nome = (char*) malloc(255 * sizeof(char));;
        
        fscanf(arquivo, "%c %d %lf %s\n", &c, &aux, &peso, nome);
        g -> vertices[i].peso = peso;
        g -> vertices[i].rotulo = nome;
    }
    
    
    
    for(int i = 0; i < m; i++){
        int sai, chega;
        char c, *nome = (char*) malloc(255 * sizeof(char));
        float peso = 0;
        
        fscanf(arquivo, "%c %d %d %f %s\n", &c, &sai, &chega, &peso, nome);
        conectarPeso(g, sai, chega, peso, nome);
    }
    
    return g;
}

TGrafo * debug_le_grafo_dimacs(TGrafo *g) {
    int n, m, dir;
    char d, *p = (char*) malloc (255 * sizeof(char));;

    
    scanf("%c %c %d %d %s\n", &d, &d, &n, &m, p);
    
    dir = d == 'U' ? 0 : 1;
    
    if(g == NULL)
        g = create_graph(n, p, dir);
        
    else
        init_graph(g, n, p, dir); // & ou não?
    
    for(int i = 0; i < n; i++){
        int aux = 0;
        double peso = 0;
        char c, *nome = (char*) malloc(255 * sizeof(char));;
        
        scanf("%c %d %lf %s\n", &c, &aux, &peso, nome);
        g -> vertices[i].peso = peso;
        g -> vertices[i].rotulo = nome;
    }
    
    
    
    for(int i = 0; i < m; i++){
        int sai, chega;
        char c, *nome = (char*) malloc(255 * sizeof(char));
        float peso = 0;
        
        scanf("%c %d %d %f %s\n", &c, &sai, &chega, &peso, nome);
       // printf("%d %d %s\n", sai, chega, nome);
        conectarPeso(g, sai, chega, peso, nome);
    }
    
    return g;
}

// Salva um grafo no formato DIMACS
int salva_grafo_dimacs(TGrafo *g, FILE* arquivo) {  
    char dir = g -> direcionado == 1 ? 'D' : 'U' ;
    int i;
    
    fprintf(arquivo, "G %c %d %d %s\n", dir, g -> n, g -> m, g -> nome);
    
    for(i = 0; i < g -> n; i++){
        fprintf(arquivo, "N %d %lf %s\n", i, g -> vertices[i].peso, g -> vertices[i].rotulo);
    }
    
    
    if(g -> vertices != NULL){
        
        int vis[g -> n][g -> n];
        
        memset(vis, 0, sizeof vis); 
        
        for(i = 0; i < g -> n; i++){
            TNoLista *aux = g -> vertices[i].direto;
            
            while(aux != NULL){
                if(!vis[i][aux -> aresta.destino] || !vis[aux -> aresta.destino][i]){
                    fprintf(arquivo, "E %d %d %lf %s\n", i, aux -> aresta.destino, aux -> aresta.peso, aux -> aresta.rotulo);
                    vis[i][aux -> aresta.destino] = vis[aux -> aresta.destino][i] = 1;
                }
                
                aux = aux -> prox;
            }
        }
    }
    
    return 1;
}

int debug_grafo_dimacs(TGrafo *g) {  
    char dir = g -> direcionado == 1 ? 'D' : 'U' ;
    int i;
    
    printf("G %c %d %d %s\n", dir, g -> n, g -> m, g -> nome);
    
    for(i = 0; i < g -> n; i++){
        printf("N %d %lf %s\n", i, g -> vertices[i].peso, g -> vertices[i].rotulo);
    }
    
    
    if(g -> vertices != NULL){
        
        int vis[g -> n][g -> n];
        
        memset(vis, 0, sizeof vis); 
        
        for(i = 0; i < g -> n; i++){
            TNoLista *aux = g -> vertices[i].direto;
            
            while(aux != NULL){
                if(!vis[i][aux -> aresta.destino] || !vis[aux -> aresta.destino][i]){
                    printf("E %d %d %lf %s\n", i, aux -> aresta.destino, aux -> aresta.peso, aux -> aresta.rotulo);
                    vis[i][aux -> aresta.destino] = vis[aux -> aresta.destino][i] = 1;
                }
                
                aux = aux -> prox;
            }
            
        }
    }
    
    return 1;
}

// Por enquanto só funciona com não orientado
int debug_grafo_dot(const TGrafo *g) {
    
    if(g -> direcionado)
        printf("digraph %s {\n", g -> nome);
    else
        printf("graph %s {\n", g -> nome);
        
    printf(" graph [fontsize = 36,\n\tlabel=\"\\n\\n%s - Autor: Celso A. Uliana Junior -- Sep 2018\",\n \tlabelloc=t]\n", g -> nome);
    
    for(int i = 0; i < g -> n; i++){
        printf(" V%d [label=\"%s\\n%lf\"]\n", i, g -> vertices[i].rotulo, g -> vertices[i].peso);
    }
    
    int vis[g -> n][g -> n];
        
    memset(vis, 0, sizeof vis); 
        
    for(int i = 0; i < g -> n; i++){
        TNoLista *aux = g -> vertices[i].direto;
            
        while(aux != NULL){
            if(!vis[i][aux -> aresta.destino] || !vis[aux -> aresta.destino][i]){
                if(g -> direcionado)
                    printf(" V%d -> V%d [label=\"%s\\n%lf\"]\n", i, aux -> aresta.destino, aux -> aresta.rotulo, aux -> aresta.peso);
                else
                    printf(" V%d -- V%d [label=\"%s\\n%lf\"]\n", i, aux -> aresta.destino, aux -> aresta.rotulo, aux -> aresta.peso);
                vis[i][aux -> aresta.destino] = vis[aux -> aresta.destino][i] = 1;
            }
                
            aux = aux -> prox;
        }
            
    }
    
    printf("}\n");
        
    return 1;
}

// Salva um grafo no formato dot
// Por enquanto só funciona com não orientado
int salva_grafo_dot(const TGrafo *g, FILE* arquivo) {
    
    if(g -> direcionado)
        fprintf(arquivo, "digraph %s {\n", g -> nome);
    else
        fprintf(arquivo, "graph %s {\n", g -> nome);
        
    
    fprintf(arquivo, " graph [fontsize = 36,\n\tlabel=\"\\n\\n%s - Autor: Celso A. Uliana Junior -- Sep 2018\",\n \tlabelloc=t]\n", g -> nome);
    
    for(int i = 0; i < g -> n; i++){
        fprintf(arquivo, " V%d [label=\"%s\\n%lf\"]\n", i, g -> vertices[i].rotulo, g -> vertices[i].peso);
    }
    
    int vis[g -> n][g -> n];
        
    memset(vis, 0, sizeof vis); 
        
    for(int i = 0; i < g -> n; i++){
        TNoLista *aux = g -> vertices[i].direto;
            
        while(aux != NULL){
            if(!vis[i][aux -> aresta.destino] || !vis[aux -> aresta.destino][i]){
                if(g -> direcionado)
                    fprintf(arquivo, " V%d -> V%d [label=\"%s\\n%lf\"]\n", i, aux -> aresta.destino, aux -> aresta.rotulo, aux -> aresta.peso);
                else
                    fprintf(arquivo, " V%d -- V%d [label=\"%s\\n%lf\"]\n", i, aux -> aresta.destino, aux -> aresta.rotulo, aux -> aresta.peso);
                vis[i][aux -> aresta.destino] = vis[aux -> aresta.destino][i] = 1;
            }
                
            aux = aux -> prox;
        }
            
    }
    
    fprintf(arquivo, "}\n");
    
    return 1;
}