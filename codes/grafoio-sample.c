#include "grafoio.h"
#include <string.h>
#include <stdlib.h>

// Lê um grafo no formato DIMACS adaptado.
// g pode ser:
//   - Um grafo alocado, mas não inicializado, caso em que será
// inicializado para o tamanho obtido do arquivo.
//   - Um ponteiro NULL, caso em que o grafo serrá criado e retornado.
// TODO: contar arestas e conferir
TGrafo * le_grafo_dimacs(TGrafo *g, FILE* arquivo) {

    // coloque seu código aqui!
    return NULL;
}

// Salva um grafo no formato DIMACS
int salva_grafo_dimacs(const TGrafo *g, FILE* arquivo) {

    // Coloque seu código aqui e substitua a linha abaixo por
    // return 1 se tudo der certo.



    return 1;
}

// Salva um grafo no formato dot
int salva_grafo_dot(const TGrafo *g, FILE* arquivo) {
    // Coloque seu código aqui e substitua a linha abaixo por
    // return 1 se tudo der certo.
    return 0;
}

  
