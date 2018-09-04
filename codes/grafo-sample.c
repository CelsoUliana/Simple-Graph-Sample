#include <stdlib.h>
#include "grafo.h"

// Inicializa um grafo para conter n vértices e m arestas.
int inicializa_grafo(TGrafo *g, TId n, TId m, int direcionado) {
  g->vertices = (TVertice *) malloc (sizeof(TVertice[n]));
  if (g->vertices == NULL)
    return 0;
  g->n = n;
  g->m = 0;
  g->direcionado = direcionado;
  g->nome = NULL;
  int i;
  for (i=0; i < n; i++) {
    g->vertices[i].rotulo = NULL;
    g->vertices[i].direto = NULL;
    g->vertices[i].reverso = NULL;
    g->vertices[i].grauEntrada = 0;
    g->vertices[i].grauSaida = 0;
    g->vertices[i].peso = 0.0;
  }
  return 1;
}

// Inicializa um grafo para conter n vértices e m arestas.
TGrafo * cria_grafo(TId n, TId m, int direcionado) {
  TGrafo * g = (TGrafo *) malloc (sizeof(TGrafo));
  if (g == NULL)
    return NULL;
  if (inicializa_grafo(g, n, m, direcionado))
    return g;
  else {
    free(g);
    return NULL;
  }
}

// Função auxiliar para liberar listas ligadas
static int finaliza_lista(TNoLista *lista) {
}

// Desaloca todas as estruturas relacionadas ao grafo.
int finaliza_grafo(TGrafo *g) {
}

// Desaloca todo o grafo, inclusive a estrutura principal.
void destroi_grafo(TGrafo *g) {
  finaliza_grafo(g);
  free(g);
}

const char *getNome(const TGrafo *g) {
  return g->nome;
}

TId contaVertices(const TGrafo *g) {
  return g->n;
}

TId contaArestas(const TGrafo *g) {
  return g->m;
}

/* SUA IMPLEMENTAÇÃO A PARTIR DAQUI */ 

TId grauVertice(const TGrafo *g, TId u);

TId grauSaidaVertice(const TGrafo *g, TId u);

TId grauEntradaVertice(const TGrafo *g, TId u);

// Verifica se u, v é uma aresta no grafo e retona sua referência, se 
// encontrada, ou NULL caso contrário.
const TAresta * aresta(const TGrafo *g, TId u, TId v);

// Remove uma aresta do grafo. Retorna falso se a aresta não foi encontrada.
int desconectar(TGrafo *g, TId u, TId v);

// Conecta dois vértices por uma aresta
const TAresta * conectar(TGrafo *g, TId u, TId v) {
  return conectarPeso(g, u, v, 0, "");
}

// Conecta dois vértices por uma aresta com peso.
const TAresta * conectarPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo);

// Altera o peso de uma aresta existente. Retorna NULL se não houver aresta.
const TAresta * alteraPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo);
