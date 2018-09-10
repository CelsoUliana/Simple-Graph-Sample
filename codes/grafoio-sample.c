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
    char c, *p;
    
    fscanf("%c %c %d %d %s", &c, &c, &n, &m, p);
    
    if(g == NULL)
        g = create_graph(n, m, dir);
        
    else
        init_graph(&g, n, m, dir); // & ou não?
    
    // continua lendo o arquivo.
  
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
    
    for(i = 0; i < g -> n; i++){
        // arestas para cada vertice.
    }   
    
    return 1;
}

// Salva um grafo no formato dot
int salva_grafo_dot(const TGrafo *g, FILE* arquivo) {
  // Coloque seu cÃ³digo aqui e substitua a linha abaixo por
  // return 1 se tudo der certo.
    return 0;
}
