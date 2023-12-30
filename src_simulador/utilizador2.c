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

pthread_mutex_t mutex;
pthread_cond_t condicaoCriacao;

// jony
void entraEspaco(Utilizador *utilizador)
{
    // 1-entrar num espaco random ou conforme os visitados anteriormente

    int random = rand() % config->numeroEspacos;
    Espaco *espaco = &(config->espacos[random]);

    // 2-verifica se tem espaco na fila
    if (espaco->numUtilizadores < espaco->lotacaoMaxima)
    {

        Utilizador *utilizadores = &espaco.utilizadores;

        for (int i = 0; i < espaco.lotacaoMaxima; i++)
        {
            if (utilizadores[i] == 0)
            {
                utilizadores[i] = utilizador;
                espaco.numDeUtilizadores++;
                break;
            }
        }
    }
    else
}

void saiParque(Utilizador *utilizador)
{
    int chanceSair = rand() % 100;
    if (chanceSair < 2)
    {
        char *buf = malloc(MAX_LINE);
        snprintf(buf, MAX_LINE, "Utilizador %i saiu do parque.\n\n", utilizador->id);
        writen(sock_monitor_fd, buf, strlen(buf));
        utilizadores[utilizador->id - 1] = 0;
        free(utilizador);
        pthread_exit(NULL);
    }
}

void *comportamentoUtilizador(void *arg)
{
    Utilizador *utilizador = (Utilizador *)arg;
    while (1)
    {
        // pthread_mutex_lock(&mutex);

        // visita as atrações que ainda não visitadas
        int atracaoNormal = rand() % 4;
        int atracaoVIP = rand() % 5;
        int todasVisitadas = 0;

        if (utilizador->atracaoAtual = "toalha")
        {

            if (utilizador->cargo == "VIP")
            {
                for (int i = 0; i < 5; i++)
                {
                    if (utilizador->atracoesVisitadas != NULL)
                    {
                        break;
                    }
                    todasVisitadas = 1;
                }
            }

            else if (utilizador->cargo == "NORMAL")
            {
                for (int i = 0; i < 4; i++)
                {
                    if (utilizador->atracoesVisitadas != NULL)
                    {
                        break;
                    }
                    todasVisitadas = 1;
                }
            }

            if (todasVisitadas == 0 && utilizador->cargo == "NORMAL")
            {

                if (atracaoNormal == 0 && utilizador->atracoesVisitadas[0] == NULL)
                {
                    entraCorrida(utilizador);
                }
                else if (atracaoNormal == 1 && utilizador->atracoesVisitadas[1] == NULL)
                {
                    entraRioLento(utilizador);
                }
                else if (atracaoNormal == 2 && utilizador->atracoesVisitadas[2] == NULL)
                {
                    entraPiscina(utilizador);
                }
                else if (atracaoNormal == 3 && utilizador->atracoesVisitadas[3] == NULL)
                {
                    entraTobogas(utilizador);
                }
            }
            else if (todasVisitadas == 1 && utilizador->cargo == "NORMAL")
            {
                if (atracaoNormal = 0)
                    entraCorrida(utilizador);
                else if (atracaoNormal = 1)
                {
                    entraRioLento(utilizador);
                }
                else if (atracaoNormal = 2)
                {
                    entraPiscina(utilizador);
                }
                else if (atracaoNormal = 3)
                {
                    entraTobogas(utilizador);
                }
                saiParque(utilizador); // chance de sair do parque
            }
            else if (todasVisitadas == 0 && utilizador->cargo == "VIP")
            {

                if (atracaoVIP == 0 && utilizador->atracoesVisitadas[0] == NULL)
                {
                    entraCorrida(utilizador);
                }
                else if (atracaoVIP == 1 && utilizador->atracoesVisitadas[1] == NULL)
                {
                    entraRioLento(utilizador);
                }
                else if (atracaoVIP == 2 && utilizador->atracoesVisitadas[2] == NULL)
                {
                    entraPiscina(utilizador);
                }
                else if (atracaoVIP == 3 && utilizador->atracoesVisitadas[3] == NULL)
                {
                    entraTobogas(utilizador);
                }
                else if (atracaoVIP == 4 && utilizador->atracoesVisitadas[4] == NULL)
                {
                    entraCabanas(utilizador);
                }
            }
            else if (todasVisitadas == 1 && utilizador->cargo == "VIP")
            {
                if (atracaoVIP = 0)
                    entraCorrida(utilizador);
                else if (atracaoVIP = 1)
                {
                    entraRioLento(utilizador);
                }
                else if (atracaoVIP = 2)
                {
                    entraPiscina(utilizador);
                }
                else if (atracaoVIP = 3)
                {
                    entraTobogas(utilizador);
                }
                else if (atracaoVIP = 4)
                {
                    entraCabanas(utilizador);
                }
                saiParque(utilizador); // chance de sair do parque
            }
        }

        sleep(1);
        // pthread_cond_signal(&condicaoCriacao);
        // pthread_mutex_unlock(&mutex);
    }
}
