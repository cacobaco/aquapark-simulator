#ifndef STRUCTS_H
#define STRUCTS_H

#include <pthread.h>

typedef struct
{
    char *nome;
    int lotacaoMaxima;
    int numUtilizadores;
    // Utilizador *utilizadores;
    // const char *cargo;
} Espaco;

typedef struct
{
    int tempoSimulacao;
    int tempoMedioChegada;
    double probEntradaEspaco;
    double probSaidaEspaco;
    double probSaidaParque;
    int lotacaoMaxima;
    pthread_t *utilizadores;
    int numeroEspacos;
    Espaco *espacos;
} Config;

typedef struct // jony
{
    int id;
    // const char *cargo;
} Utilizador;

#endif
