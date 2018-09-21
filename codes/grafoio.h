#ifndef _GRAFO_IO_H
#define _GRAFO_IO_H

#include <stdio.h>
#include "grafo.h"

// Le um grafo no formato DIMACS.
// g pode ser:
//   - Um grafo alocado, mas não inicializado, caso em que será
// inicializado para o tamanho obtido do arquivo.
//   - Um ponteiro NULL, caso em que o grafo serrá criado e retornado.
TGrafo * le_grafo_dimacs(TGrafo *g, FILE* arquivo);

// Debug a leitura dimacs
TGrafo * debug_le_grafo_dimacs(TGrafo *g)

// Salva um grafo no formato DIMACS
int salva_grafo_dimacs(const TGrafo *g, FILE* arquivo);

// Salva um grafo no formato dot
int salva_grafo_dot(const TGrafo *g, FILE* arquivo);

// Debug a saida dimacs
int debug_grafo_dimacs(TGrafo *g);

// Debug a saida dot
int debug_grafo_dot(const TGrafo *g);

#endif