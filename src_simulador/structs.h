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

typedef struct
{
    int id;
    int pos; // posição no array de utilizadores, para controlo de threads
    // const char *cargo;
} Utilizador;

#endif
