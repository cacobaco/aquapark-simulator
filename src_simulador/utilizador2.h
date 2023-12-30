#ifndef UTILIZADOR_H
#define UTILIZADOR_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "structs.h"

#define LOTACAO 100
#define MAX_LINE 512
#define MAX_CORRIDA 50
#define MAX_PISCINA 50
#define MAX_CABANAS 50
#define MAX_RIOLENTO 50
#define MAX_TOGOBA 50

pthread_t utilizadores[LOTACAO];
Utilizador *corrida[MAX_CORRIDA];
Utilizador *piscina[MAX_PISCINA];
Utilizador *cabanas[MAX_CABANAS];
Utilizador *rioLento[MAX_RIOLENTO];
Utilizador *toboga[MAX_TOGOBA];

int numUtilizadoresNaCorrida = 0;
int numUtilizadoresNaPiscina = 0;
int numUtilizadoresNasCabanas = 0;
int numUtilizadoresNoToboga = 0;
int numUtilizadoresNoRioLento = 0;

void entraParque();
void *comportamentoUtilizador(void *arg);

#endif
