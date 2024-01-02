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
    int numVips;
    pthread_mutex_t mutexLotacaoFila;
    sem_t semaforoFila;
    sem_t semaforoVips;

    int bDuracao; // flag indicadora de o espaço tem duração
    int duracao;

    int bCorrida; // flag indicadora de o espaço ser uma corrida
    pthread_mutex_t mutexCorrida;
    sem_t semaforoCorrida;      // se fechar, corrida começa (contagem de threads que entraram)
    sem_t semaforoCorridaSaida; // se fechar, corrida acabou (contagem de threads que sairam)
    int *corredores;
} Espaco;

typedef struct
{
    int tempoSimulacao;
    int tempoMedioChegada;
    double probEntradaEspaco;
    double probSaidaEspaco;
    double probSaidaParque;
    double probVip;
    int lotacaoMaxima;
    pthread_t *utilizadores;
    int numeroEspacos;
    Espaco *espacos;
} Config;

typedef struct
{
    int id;
    int pos;  // posição no array de utilizadores, para controlo de threads
    int bVip; // flag indicadora de o utilizador ser vip
} Utilizador;

#endif
