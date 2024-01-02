#ifndef STRUCTS_H
#define STRUCTS_H

#include <semaphore.h>
#include <pthread.h>

typedef struct
{
    char *nome;
    int lotacaoMaxima;
    int lotacao;
    pthread_mutex_t mutexLotacao;

    int bFila; // flag indicadora de o espaço tem fila
    int lotacaoMaximaFila;
    int lotacaoFila;
    pthread_mutex_t mutexLotacaoFila;
    sem_t semaforoFila;

    int bDuracao; // flag indicadora de o espaço tem duração
    int duracao;

    int bCorrida; // flag indicadora de o espaço ser uma corrida
    pthread_mutex_t mutexCorrida;
    sem_t semaforoCorrida;
    sem_t semaforoCorridaSaida; // se fechar, corrida acabou
    int *corredores;
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
} Utilizador;

#endif
