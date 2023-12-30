#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "config.h"
#include "socket_client.h"
#include "utilizador.h"
#include "util.h"

void main(int argc, char const *argv[])
{
    srand(time(NULL));

    loadConfig();
    printConfig();
    openSocket();

    int tempoTotal = 0;
    int tempoEsperado = 0;

    while (tempoTotal < config->tempoSimulacao)
    {
        sleep(1);
        tempoTotal++;
        tempoEsperado++;

        double probabilidade = tempoEsperado / (double)config->tempoMedioChegada;
        double random = rand() / (double)RAND_MAX;

        printf("Tempo total: %d\n", tempoTotal);
        printf("Tempo esperado: %d\n", tempoEsperado);
        printf("Probabilidade: %f\n", probabilidade);
        printf("Random: %f\n", random);

        if (random <= probabilidade)
        {
            criarUtilizador();
            tempoEsperado = 0;
        }
    }

    closeSocket();
    freeConfig();
    exit(0);
}
