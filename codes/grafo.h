#ifndef _GRAFO_H
#define _GRAFO_H

typedef int TId;
typedef double TPeso;

struct SAresta {
    TId destino;
    TPeso peso;
    char *rotulo;
};

typedef struct SAresta TAresta;

struct ListaAdj {
    struct ListaAdj *prox;
    TAresta aresta;
};

typedef struct ListaAdj TNoLista;

struct SVertice {
    TId grauSaida;
    TId grauEntrada;
    TPeso peso;
    char *rotulo;
  // Para grafos não orientados, a primeira lista abaixo contém todos os 
  // vizinhos e a segunda lista é vazia.
  // Arestas saindo do vértice, se direcionado.
    TNoLista *direto;
  // Arestas chegando no vértice, se direcionado.
    TNoLista *reverso;
};

typedef struct SVertice TVertice;

struct SGrafo {
    int direcionado;
    TId n;
    TId m;
    char *nome;
    TVertice *vertices;
};

typedef struct SGrafo TGrafo;

// Inicializa um grafo para conter n vértices e m arestas.
int init_graph(TGrafo *g, TId n, TId m, int direcionado);

// Desaloca todas as estruturas relacionadas ao grafo.
int finaliza_grafo(TGrafo *g);

// Cria um grafo para conter n vértices e m arestas.
TGrafo * cria_grafo(TId n, TId m, int direcionado);

// Desaloca todo o grafo, inclusive a estrutura principal.
void destroi_grafo(TGrafo *g);

const char *getNome(const TGrafo *g);

TId contaVertices(const TGrafo *g);

TId contaArestas(const TGrafo *g);

TId grauVertice(const TGrafo *g, TId u);

TId grauSaidaVertice(const TGrafo *g, TId u);

TId grauEntradaVertice(const TGrafo *g, TId u);

// Verifica se u, v é uma aresta no grafo e retona sua referência, se 
// encontrada, ou NULL caso contrário.
TNoLista * aresta(const TGrafo *g, TId u, TId v);

// Remove uma aresta do grafo. Retorna falso se a aresta não foi encontrada.
int desconectar(TGrafo *g, TId u, TId v);

// Conecta dois vértices por uma aresta
const TAresta * conectar(TGrafo *g, TId u, TId v);

// Conecta dois vértices por uma aresta com peso.
// Se a aresta já existir, falha, retornando NULL.
const TAresta * conectarPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo);

// Altera o peso de uma aresta existente. Retorna NULL se não houver aresta.
TAresta * alteraPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo);

#endif