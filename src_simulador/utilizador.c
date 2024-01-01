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
                    if (espaco->bTemDuracao)
                    {
                        sleep(espaco->duracao);
                        saiEspaco(utilizador, espaco);
                        break;
                    }
                    else
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
        pthread_mutex_lock(&(espaco->mutexLotacao));     // fechar trinco antes de aceder e modificar a lotação
        pthread_mutex_lock(&(espaco->mutexLotacaoFila)); // fechar trinco antes de aceder e modificar a lotação da fila

        if (espaco->lotacaoFila <= 0 && espaco->lotacao < espaco->lotacaoMaxima)
        {
            espaco->lotacao++;

            snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
            writen(sock_fd, buf, strlen(buf));

            pthread_mutex_unlock(&(espaco->mutexLotacaoFila)); // abrir trinco depois de aceder e modificar a lotação da fila
            pthread_mutex_unlock(&(espaco->mutexLotacao));     // abrir trinco depois de aceder e modificar a lotação

            return 1;
        }

        pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder a lotação

        if (espaco->lotacaoFila >= espaco->lotacaoMaximaFila)
        {
            snprintf(buf, MAX_LEN, "Utilizador %i não entrou no espaço %s porque a fila está cheia (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacaoFila, espaco->lotacaoMaximaFila);
            writen(sock_fd, buf, strlen(buf));

            pthread_mutex_unlock(&(espaco->mutexLotacaoFila)); // abrir trinco depois de aceder e modificar a lotação da fila

            return 0;
        }

        espaco->lotacaoFila++;

        snprintf(buf, MAX_LEN, "Utilizador %i entrou na fila do espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacaoFila, espaco->lotacaoMaximaFila);
        writen(sock_fd, buf, strlen(buf));

        pthread_mutex_unlock(&(espaco->mutexLotacaoFila)); // abrir trinco depois de aceder e modificar a lotação da fila

        sem_wait(&(espaco->semaforoEntrada)); // esperar que o semáforo abra

        pthread_mutex_lock(&(espaco->mutexLotacao));     // fechar trinco antes de aceder e modificar a lotação
        pthread_mutex_lock(&(espaco->mutexLotacaoFila)); // fechar trinco antes de aceder e modificar a lotação da fila

        espaco->lotacaoFila--;
        espaco->lotacao++;

        snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
        writen(sock_fd, buf, strlen(buf));

        pthread_mutex_unlock(&(espaco->mutexLotacaoFila)); // abrir trinco depois de aceder e modificar a lotação da fila
        pthread_mutex_unlock(&(espaco->mutexLotacao));     // abrir trinco depois de aceder e modificar a lotação

        return 1;
    }

    pthread_mutex_lock(&(espaco->mutexLotacao)); // fechar trinco antes de modificar e aceder a lotação

    if (espaco->lotacao >= espaco->lotacaoMaxima)
    {
        snprintf(buf, MAX_LEN, "Utilizador %i não entrou no espaço %s porque está cheio (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
        writen(sock_fd, buf, strlen(buf));

        pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação

        return 0;
    }

    espaco->lotacao++;

    snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
    writen(sock_fd, buf, strlen(buf));

    pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação

    return 1;
}

void saiEspaco(Utilizador *utilizador, Espaco *espaco)
{
    pthread_mutex_lock(&(espaco->mutexLotacao)); // fechar trinco antes de modificar a lotação
    espaco->lotacao--;

    char buf[MAX_LEN];

    if (espaco->bTemDuracao)
    {
        snprintf(buf, MAX_LEN, "Utilizador %i andou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
    }
    else
    {
        snprintf(buf, MAX_LEN, "Utilizador %i saiu do espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
    }

    writen(sock_fd, buf, strlen(buf));

    if (espaco->bTemFila)
    {
        pthread_mutex_lock(&(espaco->mutexLotacaoFila)); // fechar trinco antes de aceder a lotação da fila

        if (espaco->lotacaoFila > 0)
        {
            sem_post(&(espaco->semaforoEntrada)); // abrir semáforo para o próximo utilizador entrar
        }

        pthread_mutex_unlock(&(espaco->mutexLotacaoFila)); // abrir trinco depois de aceder a lotação da fila
    }

    pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
}
