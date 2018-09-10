#include <stdlib.h>
#include "grafo.h"

// Inicializa um grafo para conter n vértices e m arestas.
int init_graph(TGrafo *g, TId n, TId m, int direcionado) {

    g -> vertices = (TVertice *) malloc (sizeof(TVertice[n]));

      if (g -> vertices == NULL)
        return 0;
      g -> n = n;
      g -> m = 0;
      g -> direcionado = direcionado;
      g -> nome = NULL;

      int i;
      for (i = 0; i < n; i++) {
        g -> vertices[i].rotulo = NULL;
        g -> vertices[i].direto = NULL;
        g -> vertices[i].reverso = NULL;
        g -> vertices[i].grauEntrada = 0;
        g -> vertices[i].grauSaida = 0;
        g -> vertices[i].peso = 0.0;
      }

      return 1;
}

// Inicializa um grafo para conter n vértices e m arestas.
TGrafo * create_graph(TId n, TId m, int direcionado) {

      TGrafo * g = (TGrafo *) malloc (sizeof(TGrafo));

      if (g == NULL)
        return NULL;

      if (init_graph(g, n, m, direcionado))
        return g;

      else {
        free(g);
        return NULL;
      }
}

// Função auxiliar para liberar listas ligadas
static int finaliza_lista(TNoLista *lista) {

    TNoLista *cur;

    while(lista != NULL){

        cur = lista;
        lista = lista -> prox;
        free(cur);

    }

    return 1;
}

// Desaloca todas as estruturas relacionadas ao grafo.
int finaliza_grafo(TGrafo *g) {

    int i;
    for (i = 0; i < g -> n; i++) {
        finaliza_lista(g -> vertices[i].direto);
        finaliza_lista(g -> vertices[i].reverso);
      }

}

// Desaloca todo o grafo, inclusive a estrutura principal.
void destroi_grafo(TGrafo *g) {
  finaliza_grafo(g);
  free(g);
}

const char *getNome(const TGrafo *g) {
  return g -> nome;
}

TId contaVertices(const TGrafo *g) {
  return g -> n;
}

TId contaArestas(const TGrafo *g) {
  return g -> m;
}

/* SUA IMPLEMENTAÇÃO A PARTIR DAQUI */ 

TId grauVertice(const TGrafo *g, TId u){
    return grauSaidaVertice(g, u) + grauEntradaVertice(g, u);
}

TId grauSaidaVertice(const TGrafo *g, TId u){
    return g -> vertices[u].grauSaida;
}

TId grauEntradaVertice(const TGrafo *g, TId u){
    return g -> vertices[u].grauEntrada;
}

// Verifica se u, v é uma aresta no grafo e retona sua referência, se 
// encontrada, ou NULL caso contrário.
// Retorna a referencia ao nó antigo.
const TAresta * aresta(const TGrafo *g, TId u, TId v){

    if(!g -> n || g -> vertices == NULL)
        return NULL;

    else{

        TNoLista *aux = g -> vertices[v].direto;

        while(aux -> prox -> aresta.destino != u && aux -> prox -> prox != NULL){
            aux = aux -> prox;
        }

        return aux;

    }
}

// Remove uma aresta do grafo. Retorna falso se a aresta não foi encontrada.
int desconectar(TGrafo *g, TId u, TId v){

    if(g -> direcionado){

        TNoLista *cur = aresta(g, u, v);

        TNoLista *del = cur;

        cur -> prox = cur -> prox -> prox;

        free(del);

        g -> m--;
    }

    else{
        TNoLista *cur = aresta(g, u, v);
        TNoLista *reverse_cur = aresta(g, v, u);

        TNoLista *del = cur;
        TNoLista *reverse_del = reverse_cur;

        cur -> prox = cur -> prox -> prox;
        reverse_cur -> prox = reverse_cur -> prox -> prox;

        free(del);
        free(reverse_cur);

        g -> m = g -> m - 2;
    }
}

// Conecta dois vértices por uma aresta
const TAresta * conectar(TGrafo *g, TId u, TId v) {
  return conectarPeso(g, u, v, 0, "");
}

// Conecta dois vértices por uma aresta com peso.
const TAresta * conectarPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo){

    if(!g -> n || g -> vertices == NULL)
        return NULL;

    else{

        TNoLista *cur = (TNoLista *) malloc (sizeof(TNoLista));

        cur -> aresta.destino = u;
        cur -> aresta.peso = peso;
        cur -> aresta.rotulo = rotulo;

        TNoLista *ref = g -> vertices[v].direto;

        cur -> prox = ref;
        g -> m++;

    }
}

// Altera o peso de uma aresta existente. Retorna NULL se não houver aresta.
const TAresta * alteraPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo){

    const TAresta *cur = aresta(g, v, u);

    if(cur != NULL){
        cur -> peso = peso;
        cur -> rotulo = rotulo;
    }

    return cur;
}

void print_graph(Tgrafo *g){

  printf("Inicio do grafo:\n");
  printf("nome do grafo: %s\n", g -> nome);
  printf("quantidade de vertices: %d\n", g -> n);
  printf("quantidade de arestas: %d\n", g -> m);
  printf("direcionado: %d\n", g -> direcionado);

  int i;
  for (i = 0; i < g -> n; i++) {
    printf("vertice: %d\n", i);
    printf("rotudo: %s\n", g -> vertices[i].rotudo);
    printf("grau saida: %d\n", g -> vertices[i].grauSaida);
    printf("grau entrada: %d\n", g -> vertices[i].grauEntrada);
    printf("peso: %lf\n", g -> vertices[i].peso);
    printf("Lista de arestas direta: \n");

    TNoLista *cur = vertices[i].direto;

    printf("nome aresta: %s\n", cur -> aresta.rotulo);
    printf("peso aresta: %lf", cur -> aresta.peso);
    printf("sai de %d e vai a %d\n", i, cur -> aresta.destino);

    cur = vertices[i].reverso;

    printf("Lista de arestas reverso: \n");

    printf("nome aresta: %s\n", cur -> aresta.rotulo);
    printf("peso aresta: %lf", cur -> aresta.peso);
    printf("sai de %d e vai a %d\n", i, cur -> aresta.destino);
  }
}