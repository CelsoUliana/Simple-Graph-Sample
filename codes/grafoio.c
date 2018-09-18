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
    int n, m, dir, aux, sai, chega;
    char c, *p, *nome;
    double peso;
    
    fscanf("%c %c %d %d %s", &c, &c, &n, &m, p);
    
    if(g == NULL)
        g = create_graph(n, p, dir);
        
    else
        init_graph(g, n, p, dir); // & ou não?
    
    
    for(int i = 0; i < n; i++){
        fscanf("%c %d %lf %s", &c, &aux, &peso, nome);
        
        g -> vertices[i].peso = peso;
        g -> vertices[i].rotulo = nome;
    }
    
    for(int i = 0; i < m; i++){
        fscanf("%c %d %d %lf %s", &c, &sai, &chega, &peso, nome);
        conectarPeso(g, sai, chega, peso, nome);
    }
    
  
    return NULL;
}

// Salva um grafo no formato DIMACS
int salva_grafo_dimacs(const TGrafo *g, FILE* arquivo) {  
    char dir = g -> direcionado == 1 ? 'U' : 'D' ;
    int i;
    
    fprintf(arquivo, "G %c %d %d %s\n", dir, g -> n, g -> m, g -> nome);
    
    for(i = 0; i < g -> n; i++){
        fprintf(arquivo, "N %d %lf %s\n", i, g -> vertices[i].peso, g -> vertices[i].rotulo);
    }
    
    TGrafo *cp = g;
    
    if(cp -> vertices != NULL){
        for(i = 0; i < cp -> n; i++){
            TNoLista *aux = cp -> vertices[i].direto;
            
            while(aux != NULL){
                fprintf(arquivo, "E %d %d %lf %s", i, aux -> aresta.destino, aux -> aresta.peso, aux -> aresta.nome);
                
                int destino = aux -> aresta.destino;
                
                aux = aux -> prox;
                desconectar(cp, i, destino);
            }
            
        }
    }
    
    return 1;
}

// Salva um grafo no formato dot
int salva_grafo_dot(const TGrafo *g, FILE* arquivo) {
  // Coloque seu cÃ³digo aqui e substitua a linha abaixo por
  // return 1 se tudo der certo.
    return 0;
}