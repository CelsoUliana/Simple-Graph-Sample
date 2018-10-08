#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"

// Inicializa um grafo para conter n vértices
int init_graph(TGrafo *g, TId n, char *nome, int direcionado) {

    g -> vertices = (TVertice *) malloc (sizeof(TVertice[n]));

    if (g -> vertices == NULL)
        return 0;
        
    g -> n = n;
    g -> m = 0;
    g -> direcionado = direcionado;
    g -> nome = nome;

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

// Funcao preencher TNoLista.
void fill(TNoLista *cur, TId destino, TPeso peso, char *rotulo){
    cur -> aresta.destino = destino;
    cur -> aresta.rotulo = rotulo;
    cur -> aresta.peso = peso;
}

// Grafo sem vertices ou fora do limite.
int is_invalid(const TGrafo *g, TId v, TId u){
    return g -> n == 0 || g -> vertices == NULL || v >= g -> n || u >= g -> n;
}

// Inicializa um grafo para conter n vertices.
TGrafo * create_graph(TId n, char* nome, int direcionado) {

    TGrafo * g = (TGrafo *) malloc (sizeof(TGrafo));

    if (g == NULL)
        return NULL;

    if (init_graph(g, n, nome, direcionado))
        return g;

    else {
        free(g);
        return NULL;
    }
}

// Função auxiliar para liberar listas ligadas.
static int finaliza_lista(TNoLista *lista) {

    TNoLista *cur;

    while(lista != NULL){

        cur = lista;
        lista = lista -> prox;
        free(cur);
        cur -> prox = NULL;
    }

    lista = NULL;
    
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

TId grauSaidaVertice(const TGrafo *g, TId u){
    return g -> vertices[u].grauSaida;
}

TId grauEntradaVertice(const TGrafo *g, TId u){
    return g -> vertices[u].grauEntrada;
}

// Retorna soma de grau entrada e grau saida.
TId grauVertice(const TGrafo *g, TId u){
    return grauSaidaVertice(g, u) + grauEntradaVertice(g, u);
}

// Verifica se u, v eh uma aresta no grafo e retona sua referência, se 
// encontrada, ou NULL caso contrário.
// Retorna a referencia ao no antigo.
// Parametro direcao para escolhar a lista direta ou reversa.
TNoLista * aresta(const TGrafo *g, TId u, TId v, int direcao){

    if(is_invalid(g, u, v))
        return NULL;

    else{
        
        // Recebe a referencia a lista direita ou reversa, de acordo com parametro
        TNoLista *cur = direcao == 0 ? g -> vertices[u].direto : g -> vertices[u].reverso;
        TNoLista *next;

        if(cur == NULL){
            return NULL;
        }
        
        if(cur -> aresta.destino == v){
            return NULL;
        }
        
        next = cur -> prox;

        if(next == NULL){
            return NULL;
        }

        while(next -> aresta.destino != v && next -> prox != NULL){
            cur = next;
            next = next -> prox;
        }

        return cur;
    }
}

// Remove uma aresta do grafo. Retorna 0 se a aresta nao foi encontrada.
int desconectar(TGrafo *g, TId u, TId v){

    if(is_invalid(g, u, v))
        return 0;   
    
    else{
        //Remove a aresta U a V
        TNoLista *cur = aresta(g, u, v, 0);
         
        if(cur == NULL){
            if(g -> vertices[u].direto != NULL && g -> vertices[u].direto -> aresta.destino == v){
                g -> vertices[u].direto = cur = NULL;
            }
        }

        if(cur != NULL){
            if(cur -> prox != NULL){
                TNoLista *del = cur -> prox;
                cur -> prox = cur -> prox -> prox;
                free(del);
                del = cur = NULL;
            }
        }
        
        g -> vertices[u].grauSaida--;
        
        //Remove a aresta reversa V a U
        if(g -> direcionado){
    
            TNoLista *reverse_cur = aresta(g, v, u, 1);
        
            if(reverse_cur == NULL){
                if(g -> vertices[v].reverso != NULL && g -> vertices[v].reverso -> aresta.destino == u){
                    g -> vertices[v].reverso = reverse_cur = NULL;
                }
            }
        
            if(reverse_cur != NULL){
                if(reverse_cur -> prox != NULL){
                    TNoLista *del = reverse_cur -> prox;
                    reverse_cur -> prox = reverse_cur -> prox -> prox;
                    free(del);
                    del = reverse_cur = NULL;
                }
            }
        
            g -> vertices[v].grauEntrada--;
        }
        
        //Remove a aresta direita V a U
        else{

            TNoLista *reverse_cur = aresta(g, v, u, 0);

            if(reverse_cur == NULL){
                if(g -> vertices[v].direto != NULL && g -> vertices[v].direto -> aresta.destino == u){
                    free(reverse_cur);
                    g -> vertices[v].direto = reverse_cur = NULL;
                }
            }

            if(reverse_cur != NULL){
                if(reverse_cur -> prox != NULL){
                    TNoLista *reverse_del = reverse_cur -> prox;
                    reverse_cur -> prox = reverse_cur -> prox -> prox;
                    free(reverse_del);
                    reverse_del = reverse_cur = NULL;
                }
            }
        
            g -> vertices[v].grauSaida--;
        }   
    }
    
    g -> m--;
    return 1;
}

// Conecta dois vertices por uma aresta com peso.
const TAresta * conectarPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo){

    if(is_invalid(g, u, v))
        return NULL;

    else{
        
        // Adiciona a aresta de U a V
        TNoLista *cur = (TNoLista *) malloc (sizeof(TNoLista));
        fill(cur, v, peso, rotulo);
        
        TNoLista *ref = g -> vertices[u].direto;
        cur -> prox = ref;
        
        g -> vertices[u].direto = cur;
        g -> vertices[u].grauSaida++;
        
        // Adiciona a aresta reversa V a U.
        if(g -> direcionado){

            TNoLista *reverse_cur = (TNoLista *) malloc (sizeof(TNoLista));        
            fill(reverse_cur, u, peso, rotulo);
            
            TNoLista *reverse_ref = g -> vertices[v].reverso;  
            reverse_cur -> prox = reverse_ref;
            
            g -> vertices[v].reverso = reverse_cur;  
            g -> vertices[v].grauEntrada++;
        }
        
        // Adiciona aresta direita V a U
        else{
            
            TNoLista *reverse_cur = (TNoLista *) malloc (sizeof(TNoLista));     
            fill(reverse_cur, u, peso, rotulo);
            
            TNoLista *reverse_ref = g -> vertices[v].direto;
            reverse_cur -> prox = reverse_ref;

            g -> vertices[v].direto = reverse_cur;       
            g -> vertices[v].grauSaida++;
        }
        
        g -> m++;
    }
}

// Conecta dois vértices por uma aresta.
const TAresta * conectar(TGrafo *g, TId u, TId v) {
    return conectarPeso(g, u, v, 0, NULL);
}

// Altera o nome e o peso de uma aresta existente. Retorna NULL se não houver aresta.
TAresta * alteraPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo){
    
    if(is_invalid(g, u, v))
        return NULL;
    
    else{
        
        // Altera o peso da aresta U a V
        TNoLista *cur = aresta(g, u, v, 0);
        
        if(cur == NULL && g -> vertices[u].direto != NULL && g -> vertices[u].direto -> aresta.destino == v){
            g -> vertices[u].direto -> aresta.peso = peso;
            g -> vertices[u].direto -> aresta.rotulo = rotulo;
        }
        
        else if(cur != NULL){
            cur -> prox -> aresta.peso = peso;
            cur -> prox -> aresta.rotulo = rotulo;
        }
        
        // Altera o peso da aresta reversa V a U
        if(g -> direcionado){
        
            TNoLista *reverse_cur = aresta(g, v, u, 1);
            
            if(reverse_cur == NULL && g -> vertices[v].reverso != NULL && g -> vertices[v].reverso -> aresta.destino == u){
                g -> vertices[v].reverso -> aresta.peso = peso;
                g -> vertices[v].reverso -> aresta.rotulo = rotulo;
            }
        
            else if(reverse_cur != NULL){
                reverse_cur -> prox -> aresta.peso = peso;
                reverse_cur -> prox -> aresta.rotulo = rotulo;
            }
        }
        
        // Altera o peso da aresta direta V a U
        else{
            
            TNoLista *reverse_cur = aresta(g, v, u, 0);
            
            if(reverse_cur == NULL && g -> vertices[v].direto != NULL && g -> vertices[v].direto -> aresta.destino == u){
                g -> vertices[v].direto -> aresta.peso = peso;
                g -> vertices[v].direto -> aresta.rotulo = rotulo;
            }
            
            else if(reverse_cur != NULL){
                reverse_cur -> prox -> aresta.peso = peso;
                reverse_cur -> prox -> aresta.rotulo = rotulo;
            }
        }
    
    }
    
    return NULL;
}

// Funcao auxiliar de printar o grafo.
void debug_graph(TGrafo *g){

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
            printf("\t\tsai de %d e vai a %d\n", cur -> aresta.destino, i);
            cur = cur -> prox;

        }
    }
}