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
                    if (espaco->bDuracao)
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
 * função que simula a entrada de um utilizador num espaço
 */
int entraEspaco(Utilizador *utilizador, Espaco *espaco)
{
    char buf[MAX_LEN];

    if (espaco->bFila)
    {
        pthread_mutex_lock(&(espaco->mutexLotacao));     // fechar trinco antes de aceder e modificar a lotação
        pthread_mutex_lock(&(espaco->mutexLotacaoFila)); // fechar trinco antes de aceder e modificar a lotação da fila

        if (espaco->lotacaoFila <= 0 && espaco->lotacao < espaco->lotacaoMaxima)
        { // se não houver ninguém na fila e o espaço não estiver cheio, entra logo
            espaco->lotacao++;

            snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
            writen(sock_fd, buf, strlen(buf));

            pthread_mutex_unlock(&(espaco->mutexLotacaoFila)); // abrir trinco depois de aceder e modificar a lotação da fila

            if (espaco->bCorrida)
            {
                espaco->corredores[espaco->lotacao - 1] = utilizador->id; // guardar id do utilizador no array de corredores

                if (espaco->lotacao >= espaco->lotacaoMaxima)
                {                                                // se o espaço estiver cheio, começa a corrida
                    pthread_mutex_lock(&(espaco->mutexCorrida)); // fechar trinco antes de aceder e modificar a corrida

                    snprintf(buf, MAX_LEN, "Corrida no espaço %s começou.\n", espaco->nome);
                    writen(sock_fd, buf, strlen(buf));

                    for (int i = 0; i < espaco->lotacaoMaxima; i++)
                    { // baralhar corredores
                        int temp = espaco->corredores[i];
                        int random = rand() % espaco->lotacao;
                        espaco->corredores[i] = espaco->corredores[random];
                        espaco->corredores[random] = temp;
                    }

                    for (int i = 0; i < espaco->lotacaoMaxima - 1; i++)
                    {
                        sem_post(&(espaco->semaforoCorrida)); // abrir semáforo da corrida
                    }

                    pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
                }
                else
                {
                    if (espaco->lotacao <= 1)
                    {
                        sem_destroy(&(espaco->semaforoCorridaSaida)); // destruir semáforo da corrida
                        sem_init(&(espaco->semaforoCorridaSaida), 0, espaco->lotacaoMaxima);

                        sem_destroy(&(espaco->semaforoCorrida)); // destruir semáforo da corrida
                        sem_init(&(espaco->semaforoCorrida), 0, 0);
                    }

                    pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação

                    sem_wait(&(espaco->semaforoCorrida)); // esperar que o semáforo da corrida abra
                }
            }
            else
            {
                pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
            }

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

        printf("%i 0\n", utilizador->id);

        sem_wait(&(espaco->semaforoFila)); // esperar que o semáforo da fila abra

        printf("%i 1\n", utilizador->id);

        if (espaco->bCorrida)
        {
            printf("%i 1.1\n", utilizador->id);
            pthread_mutex_lock(&(espaco->mutexCorrida)); // fechar trinco antes de aceder e modificar a corrida
        }

        printf("%i 2\n", utilizador->id);
        pthread_mutex_lock(&(espaco->mutexLotacao)); // fechar trinco antes de aceder e modificar a lotação
        printf("%i 3\n", utilizador->id);
        pthread_mutex_lock(&(espaco->mutexLotacaoFila)); // fechar trinco antes de aceder e modificar a lotação da fila
        printf("%i 4\n", utilizador->id);

        espaco->lotacaoFila--;
        espaco->lotacao++;

        snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
        writen(sock_fd, buf, strlen(buf));

        pthread_mutex_unlock(&(espaco->mutexLotacaoFila)); // abrir trinco depois de aceder e modificar a lotação da fila

        if (espaco->bCorrida)
        {
            espaco->corredores[espaco->lotacao - 1] = utilizador->id; // guardar id do utilizador no array de corredores

            if (espaco->lotacao >= espaco->lotacaoMaxima)
            { // se o espaço estiver cheio, começa a corrida
                snprintf(buf, MAX_LEN, "Corrida no espaço %s começou.\n", espaco->nome);
                writen(sock_fd, buf, strlen(buf));

                for (int i = 0; i < espaco->lotacaoMaxima; i++)
                { // baralhar corredores
                    int temp = espaco->corredores[i];
                    int random = rand() % espaco->lotacao;
                    espaco->corredores[i] = espaco->corredores[random];
                    espaco->corredores[random] = temp;
                }

                for (int i = 0; i < espaco->lotacaoMaxima - 1; i++)
                {
                    sem_post(&(espaco->semaforoCorrida)); // abrir semáforo da corrida
                }

                pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
            }
            else
            {
                if (espaco->lotacao <= 1)
                {
                    while (!sem_trywait(&(espaco->semaforoCorridaSaida)))
                    {                                              // se o semáforo da corrida da saída fechar, significa que a corrida acabou
                        sem_post(&(espaco->semaforoCorridaSaida)); // abrir semáforo da corrida
                        sleep(1);
                    }

                    sem_destroy(&(espaco->semaforoCorridaSaida)); // destruir semáforo da corrida
                    sem_init(&(espaco->semaforoCorridaSaida), 0, espaco->lotacaoMaxima);

                    sem_destroy(&(espaco->semaforoCorrida)); // destruir semáforo da corrida
                    sem_init(&(espaco->semaforoCorrida), 0, 0);
                }

                pthread_mutex_unlock(&(espaco->mutexCorrida)); // abrir trinco depois de aceder e modificar a corrida
                pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação

                sem_wait(&(espaco->semaforoCorrida)); // esperar que o semáforo da corrida abra
            }
        }
        else
        {
            pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
        }

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

    if (espaco->bCorrida)
    {
        pthread_mutex_lock(&(espaco->mutexCorrida)); // fechar trinco antes de aceder e modificar a corrida
    }

    espaco->lotacao++;

    snprintf(buf, MAX_LEN, "Utilizador %i entrou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
    writen(sock_fd, buf, strlen(buf));

    if (espaco->bCorrida)
    {
        espaco->corredores[espaco->lotacao - 1] = utilizador->id; // guardar id do utilizador no array de corredores

        if (espaco->lotacao >= espaco->lotacaoMaxima)
        { // se o espaço estiver cheio, começa a corrida
            snprintf(buf, MAX_LEN, "Corrida no espaço %s começou.\n", espaco->nome);
            writen(sock_fd, buf, strlen(buf));

            for (int i = 0; i < espaco->lotacaoMaxima; i++)
            { // baralhar corredores
                int temp = espaco->corredores[i];
                int random = rand() % espaco->lotacao;
                espaco->corredores[i] = espaco->corredores[random];
                espaco->corredores[random] = temp;
            }

            for (int i = 0; i < espaco->lotacaoMaxima - 1; i++)
            {
                sem_post(&(espaco->semaforoCorrida)); // abrir semáforo da corrida
            }

            pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
        }
        else
        {
            if (espaco->lotacao <= 1)
            {
                sem_destroy(&(espaco->semaforoCorrida)); // destruir semáforo da corrida
                sem_init(&(espaco->semaforoCorrida), 0, 0);
            }

            pthread_mutex_unlock(&(espaco->mutexCorrida)); // abrir trinco depois de aceder e modificar a corrida
            pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação

            sem_wait(&(espaco->semaforoCorrida)); // esperar que o semáforo da corrida abra
        }
    }
    else
    {
        pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
    }

    return 1;
}

/*
 * função que simula a saída de um utilizador de um espaço
 */
void saiEspaco(Utilizador *utilizador, Espaco *espaco)
{
    pthread_mutex_lock(&(espaco->mutexLotacao)); // fechar trinco antes de modificar a lotação
    espaco->lotacao--;

    char buf[MAX_LEN];

    if (espaco->bCorrida)
    {
        if (espaco->lotacao <= 0)
        {
            snprintf(buf, MAX_LEN, "Corrida no espaço %s acabou.\n", espaco->nome);
            writen(sock_fd, buf, strlen(buf));

            for (int i = 0; i < espaco->lotacaoMaxima; i++)
            {
                snprintf(buf, MAX_LEN, "Utilizador %i ficou em %i na corrida no espaço %s (%i/%i).\n", espaco->corredores[i], i + 1, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
                writen(sock_fd, buf, strlen(buf));
            }

            for (int i = 0; i < espaco->lotacaoMaxima - 1; i++)
            {
                sem_post(&(espaco->semaforoCorrida)); // abrir semáforo da corrida
            }

            pthread_mutex_unlock(&(espaco->mutexCorrida)); // abrir trinco depois de aceder e modificar a corrida
            pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
        }
        else
        {
            pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação

            sem_wait(&(espaco->semaforoCorrida)); // esperar que o semáforo da corrida abra
        }
    }
    else if (espaco->bDuracao)
    {
        snprintf(buf, MAX_LEN, "Utilizador %i andou no espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
        writen(sock_fd, buf, strlen(buf));

        pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
    }
    else
    {
        snprintf(buf, MAX_LEN, "Utilizador %i saiu do espaço %s (%i/%i).\n", utilizador->id, espaco->nome, espaco->lotacao, espaco->lotacaoMaxima);
        writen(sock_fd, buf, strlen(buf));

        pthread_mutex_unlock(&(espaco->mutexLotacao)); // abrir trinco depois de aceder e modificar a lotação
    }

    if (espaco->bFila)
    {
        pthread_mutex_lock(&(espaco->mutexLotacaoFila)); // fechar trinco antes de aceder a lotação da fila

        if (espaco->lotacaoFila > 0)
        {
            sem_post(&(espaco->semaforoFila)); // abrir semáforo da fila
        }

        pthread_mutex_unlock(&(espaco->mutexLotacaoFila)); // abrir trinco depois de aceder a lotação da fila

        if (espaco->bCorrida)
        {
            sem_wait(&(espaco->semaforoCorridaSaida)); // esperar que o semáforo da fila abra
        }
    }
}
