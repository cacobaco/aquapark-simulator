#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "utilizador.h"
#include "structs.h"

#define LOTACAO 100

pthread_t utilizadores[LOTACAO];
pthread_mutex_t mutex;
pthread_cond_t condicaoCriacao;

// jony
void entraParque()
{
    srand(time(NULL)); // gerar seed "aleatória"

    int id = 0;
    while (1)
    {
        int numUtilizadores = 0;
        for (int i = 0; i < LOTACAO; i++)
        {
            if (utilizadores[i] != 0)
            {
                numUtilizadores++;
            }
        }

        criaUtilizador(++id, numUtilizadores);
        int espera = rand() % 10;
        sleep(espera);
    }
}

void *comportamentoUtilizador(void *arg)
{
    Utilizador *utilizador = (Utilizador *)arg;
    while (1)
    {
        // pthread_mutex_lock(&mutex);
        int chanceSair = rand() % 100;
        if (chanceSair < 2)
        {
            printf("Utilizador %d saiu no parque.\n\n", utilizador->id);
            utilizadores[utilizador->id] = 0;
            pthread_exit(NULL);
        }
        sleep(1);
        // pthread_cond_signal(&condicaoCriacao);
        // pthread_mutex_unlock(&mutex);
    }
}

void *criaUtilizador(int id, int arg2)
{
    // pthread_mutex_lock(&mutex);
    // pthread_cond_wait(&condicaoCriacao, &mutex);
    Utilizador *utilizador = malloc(sizeof(Utilizador));

    utilizador->id = id;

    int chance = rand() % 100;
    if (chance < 10)
    {
        utilizador->cargo = "VIP";
    }
    else
    {
        utilizador->cargo = "Normal";
    }

    pthread_create(&utilizadores[id - 1], NULL, comportamentoUtilizador, (void *)utilizador);
    printf("Utilizador %d de cargo %s entrou no parque.\n", utilizador->id, utilizador->cargo);
    printf("Estão %d utilizadores no parque.\n", arg2 + 1);
    printf("Tempo médio de chegada:\n");
    printf("Tempo médio de saida:\n\n");

    // pthread_mutex_unlock(&mutex);
}
