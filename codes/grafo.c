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

// Remove uma aresta do grafo. Retorna falso se a aresta nao foi encontrada.
int desconectar(TGrafo *g, TId u, TId v){

    if(is_invalid(g, u, v))
        return 0;
    
    if(g -> direcionado){

        TNoLista *cur = aresta(g, u, v, 0);
        TNoLista *reverse_cur = aresta(g, v, u, 1);
        
        if(reverse_cur == NULL){
            if(g -> vertices[v].reverso != NULL && g -> vertices[v].reverso -> aresta.destino == u){
                g -> vertices[v].reverso = reverse_cur = NULL;
            }
        }
        
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
        
        if(reverse_cur != NULL){
            if(reverse_cur -> prox != NULL){
                TNoLista *del = reverse_cur -> prox;
                reverse_cur -> prox = reverse_cur -> prox -> prox;
                free(del);
                del = reverse_cur = NULL;
            }
        }
        
        g -> vertices[v].grauEntrada--;
        g -> vertices[u].grauSaida--;
    }

    // Nesse caso, eh necessário remover o par de arestas que simboliza o direcionamento.
    else{

        TNoLista *cur = aresta(g, u, v, 0);
        TNoLista *reverse_cur = aresta(g, v, u, 0);

        if(cur == NULL){
            if(g -> vertices[u].direto != NULL && g -> vertices[u].direto -> aresta.destino == v){
                free(cur);
                g -> vertices[u].direto = cur = NULL;
            }
        }

        if(reverse_cur == NULL){
            if(g -> vertices[v].direto != NULL && g -> vertices[v].direto -> aresta.destino == u){
                free(reverse_cur);
                g -> vertices[v].direto = reverse_cur = NULL;
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

        if(reverse_cur != NULL){
            if(reverse_cur -> prox != NULL){
                TNoLista *reverse_del = reverse_cur -> prox;
                reverse_cur -> prox = reverse_cur -> prox -> prox;
                free(reverse_cur);
                reverse_del = reverse_cur = NULL;
            }
        }
        
        g -> vertices[u].grauSaida--;
        g -> vertices[v].grauSaida--;
    }
    
    g -> m--;
    return 1;
}

// Conecta dois vertices por uma aresta com peso.
const TAresta * conectarPeso(TGrafo *g, TId u, TId v, TPeso peso, char *rotulo){

    if(is_invalid(g, u, v))
        return NULL;

    else{
        
        // Adiciona aresta de u a v e a aresta reversa v a u.
        if(g -> direcionado){
            TNoLista *cur = (TNoLista *) malloc (sizeof(TNoLista));
            TNoLista *reverse_cur = (TNoLista *) malloc (sizeof(TNoLista));
            
            fill(cur, v, peso, rotulo);
            fill(reverse_cur, u, peso, rotulo);
            
            TNoLista *ref = g -> vertices[u].direto;
            TNoLista *reverse_ref = g -> vertices[v].reverso;
            
            cur -> prox = ref;
            reverse_cur -> prox = reverse_ref;
            
            g -> vertices[u].direto = cur;
            g -> vertices[v].reverso = reverse_cur;
            
            g -> vertices[u].grauSaida++;
            g -> vertices[v].grauEntrada++;
        }
        
        // Adiciona aresta de u a v e v a u ambos na lista direta.
        else{
            
            TNoLista *cur = (TNoLista *) malloc (sizeof(TNoLista));
            TNoLista *reverse_cur = (TNoLista *) malloc (sizeof(TNoLista));
            
            fill(cur, v, peso, rotulo);
            fill(reverse_cur, u, peso, rotulo);
            
            TNoLista *ref = g -> vertices[u].direto;
            TNoLista *reverse_ref = g -> vertices[v].direto;

            cur -> prox = ref;
            reverse_cur -> prox = reverse_ref;

            g -> vertices[u].direto = cur;
            g -> vertices[v].direto = reverse_cur;
            
            g -> vertices[u].grauSaida++;
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
    
    if(g -> direcionado){
        
        TNoLista *cur = aresta(g, u, v, 0);
        TNoLista *reverse_cur = aresta(g, v, u, 1);
        
        if(cur == NULL && g -> vertices[u].direto != NULL && g -> vertices[u].direto -> aresta.destino == v){
            g -> vertices[u].direto -> aresta.peso = peso;
            g -> vertices[u].direto -> aresta.rotulo = rotulo;
        }
        
        else{
            cur -> prox -> aresta.peso = peso;
            cur -> prox -> aresta.rotulo = rotulo;
        }
        
        if(reverse_cur == NULL && g -> vertices[v].reverso != NULL && g -> vertices[v].reverso -> aresta.destino == u){
            g -> vertices[v].reverso -> aresta.peso = peso;
            g -> vertices[v].reverso -> aresta.rotulo = rotulo;
        }
        
        else{
            reverse_cur -> prox -> aresta.peso = peso;
            reverse_cur -> prox -> aresta.rotulo = rotulo;
        }
        
        
    }
    
    else{
        
        TNoLista *cur = aresta(g, u, v, 0);
        TNoLista *reverse_cur = aresta(g, v, u, 0);

        if(cur == NULL && g -> vertices[u].direto != NULL && g -> vertices[u].direto -> aresta.destino == v){
            g -> vertices[u].direto -> aresta.peso = peso;
            g -> vertices[u].direto -> aresta.rotulo = rotulo;
        }
        
        else{
            cur -> prox -> aresta.peso = peso;
            cur -> prox -> aresta.rotulo = rotulo;
        }
        
        if(reverse_cur == NULL && g -> vertices[v].direto != NULL && g -> vertices[v].direto -> aresta.destino == u){
            g -> vertices[v].direto -> aresta.peso = peso;
            g -> vertices[v].direto -> aresta.rotulo = rotulo;
        }
        
        else{
            reverse_cur -> prox -> aresta.peso = peso;
            reverse_cur -> prox -> aresta.rotulo = rotulo;
        }
    
    }
    
    return NULL;
}

// Funcao auxiliar de printar o grafo.
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
            printf("\t\tsai de %d e vai a %d\n", cur -> aresta.destino, i);
            cur = cur -> prox;

        }
    }
}

int main(){
    //main de teste para grafo.c

    TGrafo *g;

    g = create_graph(5, NULL, 1);

    conectar(g, 0, 1);
    conectar(g, 0, 2);
    conectar(g, 0, 3);
    conectar(g, 4, 0);
    
    
    //desconectar(g, 0, 1);
    //desconectar(g, 0, 2);
    //desconectar(g, 0, 3);
    //desconectar(g, 4, 9);

    //alteraPeso(g, 0, 1, 5.0, "nome bacana");
    //alteraPeso(g, 0, 2, 10.0, "nome bacana2");
    //alteraPeso(g, 0, 3, 10000.0, "nome bacana2123123");
    //alteraPeso(g, 4, 0, 123123123.0, "julinho sdds");


    TNoLista *aux = aresta(g, 0, 7, 0);

    if(aux != NULL){
        printf("achou %d\n", aux -> prox -> aresta.destino);
    }
    else{
        if(g -> vertices[0].direto != NULL && g -> vertices[0].direto -> aresta.destino == 7){
            printf("achou %d\n", g -> vertices[0].direto -> aresta.destino);
        }
    }

    print_graph(g);

    destroi_grafo(g);
}