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

void entraEspaco(Utilizador *utilizador);

void *comportamentoUtilizador(void *arg)
{
    Utilizador *utilizador = (Utilizador *)arg;

    while (1)
    {
        entraEspaco(utilizador);
        sleep(1);
    }
}

// jony
void entraEspaco(Utilizador *utilizador)
{
    // TODO entrar num espaco conforme os visitados anteriormente
    char buf[MAX_LEN];
    int randomEspaco = rand() % config->numeroEspacos;
    Espaco *espaco = &(config->espacos[randomEspaco]);

    if (espaco->numUtilizadores < espaco->lotacaoMaxima) // TODO adicionar trinco/semaforo
    {
        espaco->numUtilizadores++;

        snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s.\n", utilizador->id, espaco->nome);
        writen(sock_fd, buf, strlen(buf));

        int sair = 0;

        while (!sair)
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
    else
    {
        snprintf(buf, MAX_LEN, "Utilizador %i não entrou no espaço %s porque está cheio.\n", utilizador->id, espaco->nome);
        writen(sock_fd, buf, strlen(buf));
    }
}
