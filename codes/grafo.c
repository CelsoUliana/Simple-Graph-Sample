#include <stdlib.h>
#include <stdio.h>
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
        g -> vertices[i].direto = NULL;
        g -> vertices[i].reverso = NULL;
    }

}

// Desaloca todo o grafo, inclusive a estrutura principal.
void destroi_grafo(TGrafo *g) {
    finaliza_grafo(g);

    free(g -> vertices);
    g -> vertices = NULL;

    free(g);
    g = NULL;
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
TNoLista * aresta(const TGrafo *g, TId u, TId v){

    if(g -> n == 0 || g -> vertices == NULL)
        return NULL;

    else{

        TNoLista *aux = g -> vertices[u].direto;

        //printf("%d - %d\n", g -> vertices[u].direto -> aresta.destino, v);


        if(aux == NULL){
            printf("é nulo\n");
            return NULL;
        }

        if(aux -> aresta.destino == v){
            return aux;
        }

        if(aux -> prox == NULL){
            return NULL;
        }


        while(aux -> prox -> aresta.destino != v || aux -> prox -> prox != NULL){
            aux = aux -> prox;
        }

        //printf("%d\n", aux -> aresta.destino);

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

    printf("show");

    if(!g -> n || g -> vertices == NULL)
        return NULL;

    else{

        printf("show");

        TNoLista *cur = (TNoLista *) malloc (sizeof(TNoLista));

        cur -> aresta.destino = v;
        cur -> aresta.peso = peso;
        cur -> aresta.rotulo = rotulo;

        TNoLista *ref = g -> vertices[u].direto;

        cur -> prox = ref;

        g -> vertices[u].direto = cur;

        g -> m++;

    }
}

// Altera o peso de uma aresta existente. Retorna NULL se não houver aresta.
TAresta * alteraPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo){

    TNoLista *cur = aresta(g, u, v);

    if(cur != NULL){
        if(cur -> prox == NULL){
            cur -> aresta.peso = peso;
            cur -> aresta.rotulo = rotulo;
        }
        else if(cur -> prox -> aresta.destino != v){
            cur -> aresta.peso = peso;
            cur -> aresta.rotulo = rotulo;
        }
        else{
            cur -> prox -> aresta.peso = peso;
            cur -> prox -> aresta.rotulo = rotulo;
        }

    }

    return NULL;
}

void print_graph(TGrafo *g){

    if(g == NULL){
        printf("Grafo nulo.\n");
        return;
    }

    printf("Inicio do grafo:\n");
    printf("nome do grafo: %s\n", g -> nome);
    printf("quantidade de vertices: %d\n", g -> n);
    printf("quantidade de arestas: %d\n", g -> m);
    printf("direcionado: %d\n", g -> direcionado);

    int i;
    for (i = 0; i < g -> n; i++) {
        printf("\tvertice: %d\n", i);
        printf("\trotudo: %s\n", g -> vertices[i].rotulo);
        printf("\tgrau saida: %d\n", g -> vertices[i].grauSaida);
        printf("\tgrau entrada: %d\n", g -> vertices[i].grauEntrada);
        printf("\tpeso: %lf\n", g -> vertices[i].peso);
        printf("\tLista de arestas direta: \n");

        TNoLista *cur = g -> vertices[i].direto;

        while(cur != NULL){
            printf("\t\tnome aresta: %s\n", cur -> aresta.rotulo);
            printf("\t\tpeso aresta: %lf\n", cur -> aresta.peso);
            printf("\t\tsai de %d e vai a %d\n", i, cur -> aresta.destino);
            cur = cur -> prox;
        }

        cur = g -> vertices[i].reverso;
        printf("\tLista de arestas reverso: \n");

        while(cur != NULL){

            printf("\t\tnome aresta: %s\n", cur -> aresta.rotulo);
            printf("\t\tpeso aresta: %lf\n", cur -> aresta.peso);
            printf("\t\tsai de %d e vai a %d\n", i, cur -> aresta.destino);
            cur = cur -> prox;

        }
    }
}

int main(){


    TGrafo *g;

    g = create_graph(5, 0, 0);

    conectar(g, 0, 1);
    conectar(g, 0, 2);
    conectar(g, 0, 3);


    //alteraPeso(g, 0, 1, 5.0, "nome bacana");
    //alteraPeso(g, 0, 2, 10.0, "nome bacana2");
    //alteraPeso(g, 0, 3, 10000.0, "nome bacana2123123");


    TNoLista *aux = aresta(g, 0, 1);

    if(aux != NULL){
        printf("achou %d\n", aux -> prox -> aresta.destino);
    }

    print_graph(g);

    destroi_grafo(g);


}