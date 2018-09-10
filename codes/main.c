#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafoio.h"

enum COMANDOS {
    DESCONHECIDO=0,
    COPIAR=1,
    DESENHAR=2,
    GRAUMAX=3,
    GRAUMIN=4
};

/* 
Descobrir o grau mínimo do grafo e listar todos os vértices
com o grau mínimo.
Se orientado, considerar a soma dos graus de entrada e saída.
*/
void lista_grau_min(const TGrafo *g, FILE *saida) {
    // Inicializa o grau com um número grande demais (nenhum vértice é vizinho
    // de todos). Se for orientado pode haver uma aresta chegando e uma saindo
    // de cada vértice.
    TId gmin = 2 * g -> n;
    TId i;

    // Procura por vértices de grau menor.
    for (i = 0; i < g->n; i++) {

        TId grauv = g -> vertices[i].grauEntrada + g -> vertices[i].grauSaida;

        if (grauv < gmin)
            gmin = grauv;
    }
  
    // Agora, imprime-os.
    fprintf(saida, "Grau mínimo: %d. Vértices: ", gmin);

    for (i = 0; i < g -> n; i++) {

        TId grauv = g -> vertices[i].grauEntrada + g->vertices[i].grauSaida;

        if (grauv == gmin)
            fprintf(saida, "%d ", i);
    }

    fprintf(saida, ".\n");
}

/* 
Descobrir o grau máximo do grafo e listar todos os vértices
com o grau máximo.
Se orientado, considerar a soma dos graus de entrada e saída.
*/
void lista_grau_max(const TGrafo *g, FILE *saida) {
}

// Imprime mensagem de uso, usada em caso de problema nos argumentos.
void imprime_uso(char **argv) {
    fprintf(stderr, "Uso: %s comando entrada.gr [saida.gr]\n", argv[0]);
    fprintf(stderr, "Comandos:\n\tcopiar : Copia o grafo de entrada para saída.\n"
	  "\tdesenhar : Gera um desenho do grafo no formato dot no arquivo de saída.\n"
	  "\tgraumax : Imprime os vértices com grau máximo.\n"
	  "\tgraumin : Imprime os vértices com grau mínimo.\n");
}

// Aqui tudo começa.
int main (int argc, char **argv) {

    if (argc < 3) {
        imprime_uso(argv);
        return 1;
    }

    // A string de comando digitada pelo usuário
    char *scomando = argv[1];
    int comando = DESCONHECIDO;

    // Identifica um dos comandos reconhecidos
    if (strcmp(scomando, "copiar") == 0)
        comando=COPIAR;
    else if (strcmp(scomando, "desenhar") == 0)
        comando=DESENHAR;
    else if (strcmp(scomando, "graumax") == 0)
        comando=GRAUMAX;
    else if (strcmp(scomando, "graumin") == 0)
        comando=GRAUMIN;

    // Verifica se o comando é conhecido
    if (comando == 0) {
        fprintf(stderr, "Comando desconhecido: %s .\n", argv[1]);
        imprime_uso(argv);
        return 1;
    }

    // Verifica se o comando tem os argumentos corretos
    if (((comando == COPIAR) || (comando == DESENHAR)) && argc < 4) {
        fprintf(stderr, "Comando requer um arquivo de saída.\n");
        imprime_uso(argv);
        return 1;
    }

    // Abre aquivo de entrada para leitura.
    FILE *entrada = fopen(argv[2], "r");
    if (entrada == NULL) {
        fprintf(stderr, "Falha ao abrir arquivo de entrada: %s .\n", argv[2]);
        return 2;
    }

    // Le o grafo de entrada no formado DIMACS.
    TGrafo *g = NULL;
    int res;
    g = le_grafo_dimacs(g, entrada);
    fclose(entrada);

    if (g == NULL) {
        fprintf(stderr, "Falha ao ler grafo.\n");
        return 2;
    }

    // Abre aquivo de saída, se necessário.
    FILE *saida;
    if (comando == COPIAR || comando == DESENHAR) {
        saida = fopen(argv[3], "w");
        if (saida == NULL) {
        fprintf(stderr, "Falha ao abrir arquivo de saída: %s.\n", argv[3]);
        destroi_grafo(g);
        return 3;
        }
    } 

    else {
        saida = stdout;
    }

    // Executa o comando
    switch (comando) {
    case COPIAR:
        res = salva_grafo_dimacs(g, saida);
        fclose(saida);
        break;
    case DESENHAR:
        res = salva_grafo_dot(g, saida);
        fclose(saida);
        break;
    case GRAUMAX:
        lista_grau_max(g, saida);
        res = 1;
        break;
    case GRAUMIN:
        lista_grau_min(g, saida);
        res=1;
        break;
    }

    // Libera o grafo.
    destroi_grafo(g);
    g = NULL;

    if (!res) {
        fprintf(stderr, "Falha ao executar comando.\n");
        return 4;
    }

    return 0;
}
