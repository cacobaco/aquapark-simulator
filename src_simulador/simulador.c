#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "config.h"
#include "socket_client.h"
// #include "utilizador.h"

void main(int argc, char const *argv[])
{
    srand(time(NULL));

    loadConfig();
    openSocket();

    int tempoTotal = 0;
    int tempoEsperado = 0;

    printConfig();

    while (tempoTotal < config->tempoSimulacao)
    {
        double chance = tempoEsperado / (double)config->tempoMedioChegada;
        double random = rand() / (double)RAND_MAX;

        printf("tempoEsperado: %d\n", tempoEsperado);
        printf("chance: %f\n", chance);
        printf("random: %f\n", random);

        if (random <= chance)
        {
            printf("novo user\n");
            tempoEsperado = 0;
        }

        sleep(1);
        tempoTotal++;
        tempoEsperado++;
    }

    fputs("Fim da simulação.\n", stdout);
    fclose(stdout);

    closeSocket();
    freeConfig();
    exit(0);
}
