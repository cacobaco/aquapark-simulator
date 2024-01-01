#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "utilizador.h"
#include "structs.h"
#include "config.h"
#include "socket_client.h"
#include "util.h"

int userId = 0;

void freeUtilizador(Utilizador *utilizador);
void *comportamento(void *arg);
int entraEspaco(Utilizador *utilizador, Espaco *espaco);
void saiEspaco(Utilizador *utilizador, Espaco *espaco);

/*
 * função que cria um utilizador assim como o seu thread
 */
void criarUtilizador()
{
    for (int i = 0; i < config->lotacaoMaxima; i++)
    {
        if (config->utilizadores[i] == 0)
        {
            userId++;
            Utilizador *utilizador = malloc(sizeof(Utilizador));
            utilizador->id = userId;
            utilizador->pos = i;

            pthread_create(&(config->utilizadores[utilizador->pos]), NULL, comportamento, (void *)utilizador);

            char *buf = malloc(MAX_LEN);
            snprintf(buf, MAX_LEN, "Utilizador %d entrou no parque.\n", utilizador->id);
            writen(sock_fd, buf, strlen(buf));
            break;
        }
    }
}

/*
 * função que liberta a memória alocada para um utilizador
 */
void freeUtilizador(Utilizador *utilizador)
{
    free(utilizador);
}

/*
 * função que simula o comportamento de um utilizador
 */
void *comportamento(void *arg)
{
    Utilizador *utilizador = (Utilizador *)arg;

    while (1)
    {
        double random = rand() / (double)RAND_MAX;

        if (random <= config->probSaidaParque)
        {
            break;
        }

        random = rand() / (double)RAND_MAX;

        if (random <= config->probEntradaEspaco)
        {
            // TODO entrar num espaco conforme os visitados anteriormente
            int randomEspaco = rand() % config->numeroEspacos;
            Espaco *espaco = &(config->espacos[randomEspaco]);

            if (entraEspaco(utilizador, espaco))
            {
                while (1) // comportamento no espaço
                {
                    sleep(1);

                    double random = rand() / (double)RAND_MAX;

                    if (random <= config->probSaidaEspaco)
                    {
                        saiEspaco(utilizador, espaco);
                        break;
                    }
                }
            }
        }

        sleep(1);
    }

    char buf[MAX_LEN];
    snprintf(buf, MAX_LEN, "Utilizador %i saiu do parque.\n", utilizador->id);
    writen(sock_fd, buf, strlen(buf));

    config->utilizadores[utilizador->pos] = 0;
    freeUtilizador(utilizador);
    pthread_exit(NULL);
    return NULL;
}

/*
 * função que simula a entrada e saída de um utilizador num espaço
 */
int entraEspaco(Utilizador *utilizador, Espaco *espaco)
{
    char buf[MAX_LEN];

    if (espaco->bTemFila)
    {
        int lotacaoFila = 0;

        if (sem_trywait(&(espaco->semaforoFila)))
        { // fila cheia, não entra
            sem_getvalue(&(espaco->semaforoFila), &lotacaoFila);
            lotacaoFila = espaco->lotacaoMaximaFila - lotacaoFila;

            snprintf(buf, MAX_LEN, "Utilizador %i não entrou no espaço %s porque a fila está cheia (%i/%i).\n", utilizador->id, espaco->nome, lotacaoFila, espaco->lotacaoMaximaFila);
            writen(sock_fd, buf, strlen(buf));

            return 0;
        }

        sem_getvalue(&(espaco->semaforoFila), &lotacaoFila);
        lotacaoFila = espaco->lotacaoMaximaFila - lotacaoFila;

        snprintf(buf, MAX_LEN, "Utilizador %i entrou na fila do espaço %s (%i/%i).\n", utilizador->id, espaco->nome, lotacaoFila, espaco->lotacaoMaximaFila);
        writen(sock_fd, buf, strlen(buf));

        sem_post(&(espaco->semaforoFila)); // libertar um lugar na fila

        int lotacao = 0;

        sem_getvalue(&(espaco->semaforoEntrada), &lotacao);
        lotacao = espaco->lotacaoMaxima - lotacao;

        snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, lotacao, espaco->lotacaoMaxima);
        writen(sock_fd, buf, strlen(buf));

        return 1;
    }

    int lotacao = 0;

    pthread_mutex_lock(&(espaco->mutexLotacao)); // fechar trinco antes de aceder à lotação

    if (sem_trywait(&(espaco->semaforoEntrada)))
    { // espaço cheio
        sem_getvalue(&(espaco->semaforoEntrada), &lotacao);
        lotacao = espaco->lotacaoMaxima - lotacao;

        snprintf(buf, MAX_LEN, "Utilizador %i não entrou no espaço %s porque está cheio (%i/%i).\n", utilizador->id, espaco->nome, lotacao, espaco->lotacaoMaxima);
        writen(sock_fd, buf, strlen(buf));

        pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder à lotação

        return 0;
    }

    sem_getvalue(&(espaco->semaforoEntrada), &lotacao);
    lotacao = espaco->lotacaoMaxima - lotacao;

    snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, lotacao, espaco->lotacaoMaxima);
    writen(sock_fd, buf, strlen(buf));

    pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder à lotação

    return 1;
}

void saiEspaco(Utilizador *utilizador, Espaco *espaco)
{
    char buf[MAX_LEN];

    sem_post(&(espaco->semaforoEntrada));

    int lotacao = 0;
    sem_getvalue(&(espaco->semaforoEntrada), &lotacao);
    lotacao = espaco->lotacaoMaxima - lotacao;

    snprintf(buf, MAX_LEN, "Utilizador %i saiu do espaço %s (%i/%i).\n", utilizador->id, espaco->nome, lotacao, espaco->lotacaoMaxima);
    writen(sock_fd, buf, strlen(buf));
}
