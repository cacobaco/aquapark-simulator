#ifndef STRUCTS_H
#define STRUCTS_H

#include <semaphore.h>
#include <pthread.h>

typedef struct
{
    char *nome;
    int lotacaoMaxima;
    pthread_mutex_t mutexLotacao;
    sem_t semaforoEntrada;

    int bTemFila;
    int lotacaoMaximaFila;
    pthread_mutex_t mutexLotacaoFila;
    sem_t semaforoFila;

    int bTemDuracao;
    int duracao;

    // int bTemFila;
    // int lotacaoFila;
    // pthread_mutex_t mutexLotacaoFila;
    // sem_t semaforoEntrada;
    // int bTemDuracao;
    // int duracao;
    // int bTemIntervalo;
    // int intervalo;
    // int bAguardar;                 // flag para os intervalos
    // pthread_mutex_t mutexAguardar; // mutex para os intervalos
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
