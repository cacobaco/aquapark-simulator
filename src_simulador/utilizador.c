#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utilizador.h"
#include "structs.h"
#include "socket_client.h"
#include "util.h"

#define MAX_LINE 512

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
            char *buf = malloc(MAX_LINE);
            snprintf(buf, MAX_LINE, "Utilizador %i saiu do parque.\n\n", utilizador->id);
            writen(sock_monitor_fd, buf, strlen(buf));
            utilizadores[utilizador->id - 1] = 0;
            pthread_exit(NULL);
        }
        sleep(1);
        // pthread_cond_signal(&condicaoCriacao);
        // pthread_mutex_unlock(&mutex);
    }
}

void *criaUtilizador(int id, int numUtilizadores)
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
    char *buf = malloc(MAX_LINE);
    snprintf(buf, MAX_LINE, "Utilizador %d de cargo %s entrou no parque.\nEstão %i utilizadores no parque.\n\n", utilizador->id, utilizador->cargo, numUtilizadores + 1);
    writen(sock_monitor_fd, buf, strlen(buf));
}
