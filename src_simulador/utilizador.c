#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utilizador.h"
#include "structs.h"
#include "config.h"
#include "socket_client.h"
#include "util.h"

int userId = 0;

void freeUtilizador(Utilizador *utilizador);
void *comportamentoUtilizador(void *arg);
void entraEspaco(Utilizador *utilizador, Espaco *espaco);

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

            pthread_create(&(config->utilizadores[utilizador->pos]), NULL, comportamentoUtilizador, (void *)utilizador);

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
void *comportamentoUtilizador(void *arg)
{
    Utilizador *utilizador = (Utilizador *)arg;
    char buf[MAX_LEN];

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

            if (espaco->numUtilizadores < espaco->lotacaoMaxima) // TODO adicionar trinco/semaforo
            {
                entraEspaco(utilizador, espaco);
            }
            else
            {
                snprintf(buf, MAX_LEN, "Utilizador %i não entrou no espaço %s porque está cheio.\n", utilizador->id, espaco->nome);
                writen(sock_fd, buf, strlen(buf));
            }
        }

        sleep(1);
    }

    snprintf(buf, MAX_LEN, "Utilizador %i saiu do parque.\n", utilizador->id);
    writen(sock_fd, buf, strlen(buf));

    config->utilizadores[utilizador->pos] = 0;
    free(utilizador);
    pthread_exit(NULL);
    return NULL;
}

/*
 * função que simula a entrada e saída de um utilizador num espaço
 */
void entraEspaco(Utilizador *utilizador, Espaco *espaco)
{
    char buf[MAX_LEN];

    espaco->numUtilizadores++;

    snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s.\n", utilizador->id, espaco->nome);
    writen(sock_fd, buf, strlen(buf));

    while (1)
    {
        sleep(1);

        double random = rand() / (double)RAND_MAX;

        if (random <= config->probSaidaEspaco)
        {
            break;
        }
    }

    espaco->numUtilizadores--;

    snprintf(buf, MAX_LEN, "Utilizador %i saiu do espaço %s.\n", utilizador->id, espaco->nome);
    writen(sock_fd, buf, strlen(buf));
}
